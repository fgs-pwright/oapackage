#include <string>


#include "arraytools.h"
#include "mathtools.h"
#include "tools.h"

#include "graphtools.h"

#ifdef RPACKAGE
#define printf notallowed
#endif


/* Interface to Nauty code
 *
 */

namespace nauty
{
#include "nauty.h"
/* MAXN=0 is defined by nauty.h, which implies dynamic allocation */

template<class Type>
std::vector<Type> uniquevec ( const std::vector<Type> &v )
{
	std::vector<Type> w = v;
// fixme: better option for this?
	typename std::vector< Type >::iterator last = std::unique ( w.begin(), w.end() );
	w.erase ( last,  w.end() );
	return w;

}

/// set colors in nauty format
void setcolors ( std::vector<int> colors, int *lab, int *ptn )
{
	const int verbose=0;
	const int n = colors.size();
	std::vector<int> ucols =uniquevec ( colors );
	if ( verbose )
		printf ( "setcolors: found %d/%d unique colors\n" , ( int ) ucols.size(), n );
	if ( verbose ) {
		display_vector ( colors );
		printf ( "\n" );
	}
	int x=-1;
	for ( size_t i=0; i<ucols.size(); i++ ) {
		if ( verbose )
			printf ( "color %d: %d\n", ( int ) i, ucols[i] );
		for ( size_t j=0; j<colors.size(); j++ ) {
			if ( colors[j]==ucols[i] ) {
				x++;
				lab[x]=j;
				ptn[x]=1;
			}
		}
		ptn[x]=0;
	}
	if ( verbose ) {
		printf ( "lab and ptn:\n" );
		print_perm ( lab, n );
		print_perm ( ptn, n );
	}
}


std::vector<int> reduceNauty ( const array_link &G, std::vector<int> colors, int verbose )
{


	//for(size_t j=0; j<colors.size(); j++) colors[j]=j;
	if ( verbose ) {
		printf ( "reduceNauty: %d vertices\n", G.n_rows );
		printf ( "  colors: " ); display_vector ( colors ); printf ( "\n" );
	}
	if ( ( int ) colors.size() !=G.n_rows || G.n_rows!=G.n_columns ) {
		myprintf ( "reduceNauty: input sizes not valid" );
		return std::vector<int>();
	}

	int nvertices=G.n_rows;

	/* DYNALLSTAT declares a pointer variable (to hold an array when it
	   is allocated) and a size variable to remember how big the array is.
	   Nothing is allocated yet.  */

	DYNALLSTAT ( graph,g,g_sz );
	DYNALLSTAT ( graph,canong,canong_sz );
	DYNALLSTAT ( int,lab,lab_sz );
	DYNALLSTAT ( int,ptn,ptn_sz );
	DYNALLSTAT ( int,orbits,orbits_sz );
	static DEFAULTOPTIONS_GRAPH ( options );
	statsblk stats;

	int m,v;
	set *gv;

	/* Default options are set by the DEFAULTOPTIONS_GRAPH macro above.
	   Here we change those options that we want to be different from the
	   defaults.  writeautoms=TRUE causes automorphisms to be written. */

	options.writeautoms = TRUE;
	options.writeautoms = FALSE;

options.getcanon=true;

	int n = nvertices;


	/* The nauty parameter m is a value such that an array of
	   m setwords is sufficient to hold n bits.  The type setword
	   is defined in nauty.h.  The number of bits in a setword is
	   WORDSIZE, which is 16, 32 or 64.  Here we calculate
	   m = ceiling(n/WORDSIZE). */

	m = SETWORDSNEEDED ( n );

	/* The following optional call verifies that we are linking
	   to compatible versions of the nauty routines. */

	nauty_check ( WORDSIZE,m,n,NAUTYVERSIONID );

	/* Now that we know how big the graph will be, we allocate
	 * space for the graph and the other arrays we need. */

	DYNALLOC2 ( graph,g,g_sz,m,n,"malloc" );
	DYNALLOC2 ( graph,canong,canong_sz,m,n,"malloc" );

	DYNALLOC1 ( int,lab,lab_sz,n,"malloc" );
	DYNALLOC1 ( int,ptn,ptn_sz,n,"malloc" );
	DYNALLOC1 ( int,orbits,orbits_sz,n,"malloc" );

	EMPTYGRAPH ( g,m,n );

	for ( int ix=0; ix<nvertices; ix++ ) {
		for ( int iy=0; iy<nvertices; iy++ ) {
			if ( G.at ( ix,iy ) >0 ) {

				ADDONEEDGE ( g, ix, iy, m );
			}
		}
	}

	//for ( v = 0; v < n; ++v )
	//	ADDONEEDGE ( g,v, ( v+1 ) %n,m );


	setcolors ( colors, lab, ptn );
	options.defaultptn=false;

	if ( verbose>=2 ) {
		printf ( "options.defaultptn: %d\n", options.defaultptn );
		printf ( " lab: \n " );	print_perm ( lab, n );
		printf ( " ptn: \n " ); print_perm ( ptn, n );
	}

	if ( verbose )
		printf ( "reduceNauty: calling densenauty\n" );

	densenauty ( g,lab,ptn,orbits,&options,&stats,m,n,canong );
	if ( verbose>=2 ) {
		printf ( "Generators for Aut(C[%d]):\n",n );

		printf ( "order = " );
		writegroupsize ( stdout,stats.grpsize1,stats.grpsize2 );
		printf ( "\n" );
	}




	std::vector<int> tr ( nvertices );
	std::copy ( lab, lab+nvertices, tr.begin() );

	// TODO: use colors in graph
	//  delete allocated variables
	DYNFREE ( g,g_sz );
	DYNFREE ( canong,canong_sz );
	DYNFREE ( lab,lab_sz );
	DYNFREE ( ptn,ptn_sz );
	DYNFREE ( orbits,orbits_sz );

	return tr;
}




} // end of nauty namespace


array_transformation_t reduceOAnauty ( const array_link &al, int verbose )
{
	std::pair<array_link, std::vector<int> > Gc = array2graph ( al,  verbose );

	array_link &G = Gc.first;
	std::vector<int> &colors = Gc.second;

	if ( verbose>=2 ) {
		printf ( "graph:\n" );
		G.showarray();
	}

	std::vector<int> tr = nauty::reduceNauty ( G, colors );
	tr = invert_permutation(tr);

	arraydata_t arrayclass = arraylink2arraydata ( al );
	array_transformation_t ttm = oagraph2transformation ( tr, arrayclass, verbose );

	return ttm;
}


template<class IntType>
/// helper function
std::vector<int> indexvector ( const std::vector<IntType> s )
{
	//printf("indexvector: input "); print_perm(s);
	int n = std::accumulate ( s.begin(), s.end(), 0 );

	std::vector<int> v ( n );

	int x=0;
	for ( int i=0; i<s.size(); i++ ) {
		for ( int j=0; j<s[i]; j++ ) {
			v[x]=i;
			x++;
		}
	}
	//printf("indexvector: "); print_perm(v);
	return v;
}

/**  Convert orthogonal array to graph representation
 *
 *   The conversion method is as in Ryan and Bulutoglu.
 *   The resulting graph is bi-partite.
 *   The graph representation can be used for isomorphism testing.
*/
std::pair<array_link, std::vector<int> >  array2graph ( const array_link &al, int verbose )
{
	arraydata_t arrayclass = arraylink2arraydata ( al );
	int nrows = al.n_rows;
	int ncols = al.n_columns;
	const std::vector<int> s = arrayclass.getS();

	int nRowVertices = nrows;
	int nColumnLevelVertices = std::accumulate ( s.begin(), s.end(), 0 );
	int nColVertices = ncols;

	int nVertices = nrows + ncols + nColumnLevelVertices; // number of vertices in the graph
	int colOffset = nrows;

	std::vector<int> vertexOffsets ( s.size() +1 );

	std::vector<int> cs = cumsum0 ( s );

	for ( size_t j=0; j<s.size(); j++ )
		vertexOffsets[j] = colOffset + ncols + cs[j];

	std::vector<int> colors ( nVertices );

	// row vertices: color 0
	for ( int i=0; i<nrows; i++ )
		colors[i]=0;
	// column vertices: color 1
	for ( int i=0; i<ncols; i++ )
		colors[i+nrows]=1;


	// other vertices: color 2+...
	//std::vector<int> v = indexvector ( s );
	for ( int i=0; i<nColumnLevelVertices; i++ )
		colors[i+nrows+ncols]=2; //+v[i];


	if ( verbose )
		printf ( "array2graph: generating graph of size %d=%d+%d+%d\n", nVertices, nrows, ncols, nColumnLevelVertices );
	array_link G ( nVertices, nVertices, 0 ); // graph
	G.setconstant ( 0 );

//    im = np.zeros((nVertices, nVertices))  # incidence matrix


	for ( int r=0; r<nrows; r++ ) {
		for ( int c=0; c<ncols; c++ ) {
			int idx = al.at ( r, c ) + vertexOffsets[c];
			G.at ( r, idx )  = 1;
			G.at ( idx, r )  = 1;
		}
	}


	if ( nColVertices > 0 ) {
		int colidx = 2;
		for ( int col =0; col<ncols; col++ ) {
			for ( int sx=0; sx<s[col]; sx++ ) {
				int sel = vertexOffsets[col] + sx;
				G.at ( colOffset+col, sel ) = colidx; // = colidx;
				G.at ( sel, colOffset+col ) = colidx; // = colidx;
			}
		}
	}

	// The non-row vertices do not have any connections to other non-row vertices.

	int ss = std::accumulate ( s.begin(), s.end(), 0 ); // std::accumulate<int>(s.begin(), s.end(), 0);

	return std::pair<array_link, std::vector<int> > ( G, colors );
}

/// apply a vertex permutation to a graph
array_link transformGraph ( const array_link &G, const std::vector<int> tr, int verbose )
{
	array_link H = G;

	//printfd("transformGraph: tr "); display_vector(tr); printf("\n");
	for ( int i=0; i<H.n_rows; i++ ) {
		for ( int j=0; j<H.n_columns; j++ ) {
			int ix=tr[i];
			int jx=tr[j];
			H.at ( ix,jx ) = G._at ( i,j );;
		}
	}
	return H;
}

/// From a relabelling of the graph return the corresponding array transformation
array_transformation_t oagraph2transformation ( const std::vector<int> &pp, const arraydata_t &arrayclass, int verbose )
{
	if (arrayclass.ismixed() ) {
		printfd("ERROR: oagraph2transformation not implemented for mixed-level designs\n");
			array_transformation_t ttr ( arrayclass );
return ttr;
	}
	///invert the labelling transformation
	std::vector<int> ppi = invert_permutation(pp);

	// extract colperms and rowperm and levelperms from pp
	array_transformation_t ttr ( arrayclass );

	if ( verbose ) {
		printf ( "labelling2transformation: class %s\n", arrayclass.idstr().c_str() );
		printf ( "labelling2transformation: pp  " );
		display_vector ( pp );
		printf ( "\n" );
		printf ( "labelling2transformation: ppi " );
		display_vector ( ppi );
		printf ( "\n" );
	}
	const int N= arrayclass.N;
	std::copy ( pp.begin(), pp.begin() +N, ttr.rperm );
	int rmin=pp.size();
	for ( int j=0; j<N; j++ )
		rmin = std::min ( rmin, ( int ) ttr.rperm[j] );
	for ( int i=0; i<N; i++ )
		ttr.rperm[i] -= rmin;
	ttr=ttr.inverse();

	if ( verbose ) {
		printf ( "labelling2transformation: rowperm " );
		print_perm ( ttr.rperm, N );
	}

	int ncols=arrayclass.ncols;
	array_transformation_t ttc ( arrayclass );
	std::vector<int> colperm ( arrayclass.ncols );
	std::copy ( pp.begin() +N, pp.begin() +N+ncols, colperm.begin() );
	if (verbose>=2) {
	printfd ( "colperm: " );
	display_vector ( colperm );
	printf ( "\n" );
	}
	colperm = sorthelper ( colperm );
	colperm = invert_permutation(colperm);
	ttc.setcolperm ( colperm );

	if ( verbose ) {
		printfd ( "labelling2transformation: colperm " );
		display_vector ( colperm );
		printf ( "\n" );
	}

	std::vector<int> s = arrayclass.getS();

	int ns = std::accumulate ( s.begin(), s.end(), 0 );
	array_transformation_t ttl ( arrayclass );
	//ttl=ttl.inverse();

	std::vector<int> lvlperm ( ns );
	std::copy ( pp.begin() +N+ncols, pp.begin() +N+ncols+ns, lvlperm.begin() );
	//std::copy ( ppi.begin() +N+ncols, ppi.begin() +N+ncols+ns, lvlperm.begin() );
	lvlperm=sorthelper ( lvlperm );

	std::vector<int>cs=cumsum0 ( s );

	//lp = []
	for ( int ii=0; ii<ncols; ii++ ) {
		std::vector<int> ww ( lvlperm.begin() +cs[ii], lvlperm.begin() +cs[ii+1] ); //  = lvlperm[cs[ii]:cs[ii + 1]]

		//FIXME: level permutations in reduction....

		//ww=sorthelper ( ww );

		//std::vector<int> ww(s[ii]);
		indexsort is ( ww );

		if (verbose>=2) {
		printfd ( "index sorted: " );
		display_vector ( is.indices );
		printf ( "\n" );
		}
		ww=is.indices;

		//for(int j=0; j<

		//printf("ii %d: ww ", ii); display_vector(ww); printf("\n");
		//printf("ww "); display_vector(ww); printf("\n");
		//ww = ww - ww.min();        ww = np.argsort(ww)
		//lp.append(ww)
		if ( verbose>=1 ) {
			printfd ( "oagraph2transformation: lvlperm %d: ",ii );
			display_vector ( ww );
			printf ( "\n" );
			fflush ( 0 );
		}
		ttl.setlevelperm ( ii, ww );
	}

	ttl=ttl.inverse();
	array_transformation_t tt = ttr*ttc*ttl;

	return tt;
}


/// unittest: return 1 if all tests are good
int unittest_nautynormalform(const array_link &al, int verbose)
{
		arraydata_t arrayclass = arraylink2arraydata ( al );

	if (verbose>=2) {
	printf("unittest_nautynormalform: testing on array\n");
		al.showarray();
	}
	
	array_link alr1 = al.randomperm();
	array_link alr2 = al.randomperm();
	
	array_transformation_t ttx1 = reduceOAnauty ( alr1, 0 );
	array_link alx1 = ttx1.apply ( alr1 );

		array_transformation_t ttx2 = reduceOAnauty ( alr2, 0 );
	array_link alx2 = ttx2.apply ( alr2 );

	
	/*
	std::pair<array_link, std::vector<int> > Gc = array2graph ( alr,  verbose>=2 );	
		std::vector<int> tr = nauty::reduceNauty ( Gc.first, Gc.second );
		if (verbose) {
		printfd ( "canon: " ); display_vector ( tr ); printf ( "\n" );
		}
		//return 0;
		
		//Gc.first.showarray();
		std::vector<int> tri = invert_permutation(tr);
		array_link Gm = transformGraph ( Gc.first, tri, 1);
		printf("G minimal\n"); Gm.showarray();
		//printf("j %d\n", j);
		
		array_transformation_t ttm = oagraph2transformation ( tri, arrayclass, verbose );
		array_link alm = ttm.apply(alr);

	std::pair<array_link, std::vector<int> > Gcm = array2graph ( alm,  verbose>=2 );	
	*/
	
	if(alx1 != alx2 ) {
		printfd("unittest_nautynormalform: error: transformed graphs unequal!\n");
		
		printf("alx1: \n");alx1.showarray();
		printf("alx2: \n"); alx2.showarray();
		
		return 0;
	}
	
	return 1;
}


// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4; 
