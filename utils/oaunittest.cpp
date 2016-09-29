
/** \file oaunittest.cpp

C++ program: oaunittest

oaunittest: run some tests on the code

Author: Pieter Eendebak <pieter.eendebak@gmail.com>
Copyright: See LICENSE.txt file that comes with this distribution
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <algorithm>

#include "arraytools.h"
#include "arrayproperties.h"
#include "anyoption.h"
#include "tools.h"
#include "extend.h"
#include "Deff.h"
#include "conference.h"

#include "graphtools.h"

#include "Eigen/Dense"

#ifdef HAVE_BOOST
#include <string>
#include <boost/filesystem.hpp>
#endif

template <class Type>
void permute ( Type *source, Type *target, const std::vector<int> p ) {
    for ( size_t i=0; i<p.size(); i++ )
        target[i]=source[p[i]];
}

/// check transformation inverse. return 0 if test is good
<<<<<<< HEAD
int checkConferenceInverse ( const array_link &al )
{
	conference_transformation_t T1(al);
	T1.randomize();
=======
int checkConferenceInverse ( const array_link &al ) {
    conference_transformation_t T1 ( al );
    T1.randomize();
>>>>>>> 0019c2ab85111f901159982b3686d30e2c5f93ad

    conference_transformation_t T1i = T1.inverse();
    conference_transformation_t II = T1i * T1;

    myassert ( II.isIdentity(), "unittest error: inverse of conference matrix transformation\n" );
    myassert ( al == II.apply ( al ), "unittest error: inverse of conference matrix transformation\n" );

    return 0;
}

/// check composition operator. returns 0 if test id good
<<<<<<< HEAD
int checkConferenceComposition ( const array_link &al, int verbose=0 )
{
	conference_transformation_t T1(al);
	T1.randomize();

	conference_transformation_t T2(al);
	T2.randomize();

	conference_transformation_t T3 = T2 * T1;

	array_link al2 = T2.apply ( al );
	array_link al12 = T1.apply ( al2 );
	array_link al3 = T3.apply ( al );

	if ( verbose ) {
		printfd("checkTransformationComposition: transforms\n");
	T1.show();
	T2.show();
	T3.show();
		printfd("checkTransformationComposition: arrays\n");
		al.showarray();
		al2.showarray();
		al12.showarray();
		al3.showarray();
	}

	myassert ( al3==al12, "unittest error: composition of conference transformations\n" );

	return 0;
=======
int checkConferenceComposition ( const array_link &al, int verbose=0 ) {
    conference_transformation_t T1 ( al );
    T1.randomize();

    conference_transformation_t T2 ( al );
    T2.randomize();

    conference_transformation_t T3 = T2 * T1;

    array_link al2 = T2.apply ( al );
    array_link al12 = T1.apply ( al2 );
    array_link al3 = T3.apply ( al );

    if ( verbose ) {
        printfd ( "checkTransformationComposition: transforms\n" );
        T1.show();
        T2.show();
        T3.show();
        printfd ( "checkTransformationComposition: arrays\n" );
        al.showarray();
        al2.showarray();
        al12.showarray();
        al3.showarray();
    }

    myassert ( al3==al12, "unittest error: composition of conference transformations\n" );

    return 0;
>>>>>>> 0019c2ab85111f901159982b3686d30e2c5f93ad
}

/// check transformation inverse. return 0 if test is good
int checkTransformationInverse ( const array_link &al ) {
    arraydata_t adataX = arraylink2arraydata ( al );
    array_transformation_t T1 ( &adataX );
    T1.randomize();

    array_transformation_t T1i = T1.inverse();
    array_transformation_t II = T1i * T1;

    myassert ( II.isIdentity(), "unittest error: inverse of array transformation\n" );

    return 0;
}
/// check composition operator. returns 0 if test id good
int checkTransformationComposition ( const array_link &al, int verbose=0 ) {
    arraydata_t adataX = arraylink2arraydata ( al );

    array_transformation_t T1 ( &adataX );
    T1.randomize();
    array_transformation_t T2 ( &adataX );
    T2.randomize();
    array_transformation_t T3;
    T3= T1 * T2;

    //T2.reset(); T2.randomize();
    array_link al2 = T2.apply ( al );

    //T1.show(); al2.showarray(); exit(0);

    array_link al12 = T1.apply ( al2 );
    array_link al3 = T3.apply ( al );

    if ( verbose ) {
        printfd ( "checkTransformationComposition\n" );
        T1.show();
        T2.show();
        T3.show();
        al.showarray();
        al2.showarray();
        al12.showarray();
        al3.showarray();
    }

    myassert ( al3==al12, "unittest error: composition of array transformations\n" );

    return 0;
}

enum {UGOOD, UERROR};

/** unittest for oapackage
 *
 * Returns UGOOD if all tests are ok.
 *
 */
int oaunittest ( int verbose, int writetests=0, int randval = 0 ) {
    double t0=get_time_ms();
    const char *bstr = "OA unittest";
    cprintf ( verbose, "%s: start\n", bstr );

    srand ( randval );

    int allgood=UGOOD;

    initncombscache ( 20 );

    /* constructors */
    {
        cprintf ( verbose,"%s: interaction matrices\n", bstr );

<<<<<<< HEAD
		Eigen::MatrixXd dm = m1-m2;
		int sum = dm.sum();

		myassert ( sum==0, "unittest error: construction of interaction matrices\n" );


	}

#ifdef OADEV
	/* conference matrices */
	{
		cprintf ( verbose,"%s: conference matrices\n", bstr );

		int N=4;
		conference_t ctype ( N, N );

		arraylist_t kk;
		array_link al = ctype.create_root();
		kk.push_back ( al );

		for ( int extcol=2; extcol<N; extcol++ ) {
			kk = extend_conference ( kk, ctype,  0 );
		}
		myassert ( kk.size() ==1, "unittest error: conference matrices for N=4\n" );
	}

	{
		cprintf ( verbose,"%s: random transformation for conference matrices\n", bstr );

		array_link al = exampleArray ( 19,1 );
		conference_transformation_t T ( al );
		//T.randomizerowflips();
		T.randomize();

		conference_transformation_t Ti = T.inverse();
		array_link alx = Ti.apply ( T.apply ( al ) );

		if ( 0 ) {
			printf ( "input array:\n" );
			al.showarray();
			T.show();
			printf ( "transformed array:\n" );
			T.apply ( al ).showarray();
			Ti.show();
			alx.showarray();
		}

		myassert ( alx==al, "transformation of conference matrix\n" )	;
	}
#endif

	/* constructors */
	{
		cprintf ( verbose,"%s: constructors\n", bstr );

		array_transformation_t t;
		conference_transformation_t ct;

	}

	/* J-characteristics */
	{
		cprintf ( verbose,"%s: J-characteristics\n", bstr );

		array_link al=exampleArray ( 8,1 );

		const int mm[]= {-1,-1,0, 0, 8,16,0,-1};

		for ( int jj=2; jj<7; jj++ ) {
			std::vector<int> jx = al.Jcharacteristics ( jj );
			int j5max = vectormax ( jx, 0 );
			if ( verbose>=2 ) {
				printf ( "oaunittest: jj %d: j5max %d\n", jj, j5max );
			}

			if ( j5max!=mm[jj] ) {
				printfd ( "j5max %d (should be %d)\n", j5max, mm[jj] );
				allgood=UERROR;
				return allgood;
			}
		}
	}
	{
		cprintf ( verbose, "%s: array transformations\n", bstr );

		const int N=9;
		const int t = 3;
		arraydata_t adataX ( 3, N, t, 4 );

		array_link al ( adataX.N,adataX.ncols,-1 );
		al.create_root ( adataX );

		if ( checkTransformationInverse ( al ) )
			allgood=UERROR;

		if ( checkTransformationComposition ( al ) )
			allgood=UERROR;

		al = exampleArray ( 5, 1 );
		if ( checkTransformationInverse ( al ) )
			allgood=UERROR;

		if ( checkTransformationComposition ( al ) )
			allgood=UERROR;


		for (int i=0; i<15; i++) {
			al=exampleArray(18,0);
		if ( checkConferenceComposition ( al ) )
			allgood=UERROR;
		if ( checkConferenceInverse ( al ) )
			allgood=UERROR;
			al=exampleArray(19,0);
		if ( checkConferenceComposition ( al ) )
			allgood=UERROR;
		if ( checkConferenceInverse ( al ) )
			allgood=UERROR;

		}

	}

	{
		cprintf ( verbose, "%s: rank \n", bstr );

		const int idx[10]={0, 1, 2, 3, 4, 6, 7, 8, 9};
		const int rr[10]= {4,11,13,18,16, 4, 4,29,29};
		for ( int ii=0; ii<9; ii++ ) {
			array_link al = exampleArray ( idx[ii], 0 );
			myassert(al.is2level(), "unittest error: input array is not 2-level\n");

			int r = arrayrankColPiv ( array2xf ( al ) );

			int r3 =  ( array2xf ( al ) ).rank();
			myassert(r==r3, "unittest error: rank of array");

			if ( verbose>=2 ) {
				al.showarray();
				printf ( "unittest: rank of array %d: %d\n", idx[ii], r );
			}

			//array2xf ( al ) .showarray();
			myassert ( rr[ii]==r, "unittest error: rank of example matrix\n" );
		}

	}

	{
		cprintf ( verbose, "%s: Doptimize \n", bstr );
		const int N=40;
		const int t = 0;
		arraydata_t arrayclass ( 2, N, t, 6 );
		std::vector<double> alpha ( 3 );
		alpha[0]=1;
		alpha[1]=1;
		alpha[2]=0;
		int niter=5000;
		double t00 =get_time_ms();
		DoptimReturn rr =  Doptimize ( arrayclass, 10, alpha, 0, DOPTIM_AUTOMATIC, niter );

		array_t ss[7] = {3,3,2,2,2,2,2};
		arraydata_t arrayclassmixed ( ss, 36, t, 5 );
		rr =  Doptimize ( arrayclassmixed, 10, alpha, 0, DOPTIM_AUTOMATIC, niter );

		cprintf ( verbose, "%s: Doptimize time %.3f [s] \n", bstr, get_time_ms() - t00 );
	}


	{
		cprintf ( verbose, "%s: J-characteristics for conference matrix\n", bstr );

		array_link al = exampleArray(19, 0);
		std::vector<int> j2 = Jcharacteristics_conference( al, 2 );
		std::vector<int> j3 = Jcharacteristics_conference( al, 3 );

			myassert ( j2[0]==0, "j2 value incorrect");
			myassert ( j2[1]==0, "j2 value incorrect");
			myassert ( std::abs(j3[0])==1, "j3 value incorrect");

			if (verbose>=2) {
		al.showarray();
		printf("j2: "); display_vector(j2); printf("\n");
		printf("j3: "); display_vector(j3); printf("\n");
			}
	}

	{
// test PEC sequence
		cprintf ( verbose, "%s: PEC sequence\n", bstr );
		for ( int ii=0; ii<5; ii++ ) {
			array_link al  = exampleArray ( ii, 0 );
			std::vector<double> pec = PECsequence ( al );
			printf ( "oaunittest: PEC for array %d: ", ii );
			display_vector ( pec );
			printf ( " \n" );
		}
	}


	{
		cprintf ( verbose, "%s: D-efficiency test\n", bstr );
		//  D-efficiency near-zero test
		{
			array_link al = exampleArray ( 14 );
			double D= al.Defficiency();
			std::vector<double> dd = al.Defficiencies();
			printf ( "D %f, D (method 2) %f\n", D, dd[0] );
			assert ( fabs ( D-dd[0] ) <1e-4 );
		}
		{
			array_link al = exampleArray ( 15 );
			double D= al.Defficiency();
			std::vector<double> dd = al.Defficiencies();
			printf ( "D %f, D (method 2) %f\n", D, dd[0] );
			assert ( fabs ( D-dd[0] ) <1e-4 );
			assert ( fabs ( D-0.335063 ) < 1e-3 );
		}

	}

	arraydata_t adata ( 2, 20, 2, 6 );
	OAextend oaextendx;
	oaextendx.setAlgorithm ( ( algorithm_t ) MODE_ORIGINAL, &adata );

	std::vector<arraylist_t> aa ( adata.ncols+1 );
	printf ( "OA unittest: create root array\n" );
	create_root ( &adata, aa[adata.strength] );

	/** Test extend of arrays **/
	{
		cprintf ( verbose, "%s: extend arrays\n", bstr );

		setloglevel ( SYSTEM );

		for ( int kk=adata.strength; kk<adata.ncols; kk++ ) {
			aa[kk+1] = extend_arraylist ( aa[kk], adata, oaextendx );
			printf ( "  extend: column %d->%d: %ld->%ld arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size() );

		}

		if ( aa[adata.ncols].size() != 75 ) {
			printf ( "extended ?? to %d arrays\n", ( int ) aa[adata.ncols].size() );
		}
		myassert ( aa[adata.ncols].size() ==75, "number of arrays is incorrect" );

		aa[adata.ncols].size();
		setloglevel ( QUIET );
	}



	{ /** Test dof **/
		cprintf ( verbose, "%s: test delete-one-factor reduction\n", bstr );

		array_link al = exampleArray ( 4 );
		cprintf ( verbose>=2, "LMC: \n" );
		al.reduceLMC();
		cprintf ( verbose>=2, "DOP: \n" );
		al.reduceDOP();
	}

	arraylist_t lst;

	{ /** Test different methods **/
		cprintf ( verbose, "%s: test 2 different methods\n", bstr );

		const int s = 2;
		arraydata_t adata ( s, 32, 3, 10 );
		arraydata_t adata2 ( s, 32, 3, 10 );
		OAextend oaextendx;
		oaextendx.setAlgorithm ( ( algorithm_t ) MODE_ORIGINAL, &adata );
		OAextend oaextendx2;
		oaextendx2.setAlgorithm ( ( algorithm_t ) MODE_LMC_2LEVEL, &adata2 );

		printf ( "OA unittest: test 2-level algorithm on %s\n", adata.showstr().c_str() );
		std::vector<arraylist_t> aa ( adata.ncols+1 );
		create_root ( &adata, aa[adata.strength] );
		std::vector<arraylist_t> aa2 ( adata.ncols+1 );
		create_root ( &adata, aa2[adata.strength] );

		setloglevel ( SYSTEM );

		for ( int kk=adata.strength; kk<adata.ncols; kk++ ) {
			aa[kk+1] = extend_arraylist ( aa[kk], adata, oaextendx );
			aa2[kk+1] = extend_arraylist ( aa2[kk], adata2, oaextendx2 );
			printf ( "  extend: column %d->%d: %ld->%ld arrays, 2-level method %ld->%ld arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size(), aa2[kk].size(), aa2[kk+1].size() );

			if ( aa[kk+1]!=aa2[kk+1] ) {
				printf ( "oaunittest: error: 2-level algorithm unequal to original algorithm\n" );
				exit ( 1 );
			}
		}
		setloglevel ( QUIET );

		lst = aa[8];
	}

	{
		cprintf ( verbose, "%s: test dtable creation\n", bstr );
		double t0x=get_time_ms();

		for(int i=0; i<4; i++) {
		array_link al = exampleArray(5);
				array_link dtable = createJdtable ( al );
		}
	}

	{
		cprintf ( verbose, "%s: test Pareto calculation\n", bstr );
		double t0x=get_time_ms();

		int nn=lst.size();
		for ( int k=0; k<5; k++ ) {
			for ( int i=0; i<nn; i++ ) {
				lst.push_back ( lst[i] );
			}
		}
		Pareto<mvalue_t<long>,long> r = parsePareto ( lst, 1 );
		cprintf ( verbose,"%s: test Pareto %d/%d: %.3f [s]\n", bstr, r.number(), r.numberindices(), ( get_time_ms() - t0x ) );

	}

	{
		cprintf ( verbose,"%s: check reduction transformation\n", bstr );
		array_link al = exampleArray ( 6 ).reduceLMC();

		arraydata_t adata=arraylink2arraydata ( al );
		LMCreduction_t reduction ( &adata );
		reduction.mode=OA_REDUCE;

		reduction.init_state=COPY;
		OAextend oaextend;
		oaextend.setAlgorithm ( MODE_ORIGINAL, &adata );
		array_link alr = al.randomperm();

		array_link al2=reduction.transformation->apply ( al );

		//printf("input: \n"); al2.showarray();
		lmc_t tmp = LMCcheck ( alr,adata, oaextend, reduction );

		array_link alx = reduction.transformation->apply ( alr );

		bool c = alx==al;
		if ( !c ) {
			printf ( "oaunittest: error: reduction of randomized array failed!\n" );
			printf ( "-- al \n" );
			al.showarraycompact();
			printf ( "-- alr \n" );
			alr.showarraycompact();
			printf ( "-- alx \n" );
			alx.showarraycompact();
			allgood=UERROR;
		}
	}

	{
		cprintf ( verbose,"%s: reduce randomized array\n", bstr );
		array_link al = exampleArray ( 3 );

		arraydata_t adata=arraylink2arraydata ( al );
		LMCreduction_t reduction ( &adata );

		for ( int ii=0; ii<50; ii++ ) {
			reduction.transformation->randomize();
			//reduction.transformation->randomizecolperm();
			array_link al2=reduction.transformation->apply ( al );

			//printf("input: \n"); al2.showarray();

			array_link alr=al2.reduceLMC();
			if ( 0 ) {
				printf ( "\n reduction complete:\n" );
				al2.showarray();
				printf ( "	--->\n" );
				alr.showarray();
			}
			bool c= ( al==alr );
			if ( !c ) {
				printf ( "oaunittest: error: reduction of randomized array failed!\n" );
				allgood=UERROR;
			}

		}
	}



	/* Calculate symmetry group */
	{
		cprintf ( verbose,"%s: calculate symmetry group\n", bstr );

		const arraylist_t &arraylist = aa[5];

		array_link al=arraylist.at ( 0 );
		symmetry_group sg = al.row_symmetry_group();

		aa.resize ( 0 );
	}

	/* Test efficiencies */
	{
		cprintf ( verbose,"%s: efficiencies\n", bstr );

		std::vector<double> d;
		int vb=1;

		array_link al;
		if ( 1 ) {
			al = exampleArray ( 9, vb );
			al.showproperties();
			d = al.Defficiencies ( 0, 1 );
			//printf("verbose: %d\n", verbose);
			if ( verbose>=2 )
				printf ( "  efficiencies: D %f Ds %f D1 %f Ds0 %f\n", d[0], d[1], d[2], d[3] );
			if ( fabs ( d[0]-al.Defficiency() ) >1e-10 ) {
				printf ( "oaunittest: error: Defficiency not good!\n" );
				allgood=UERROR;

			}
		}
		al = exampleArray ( 8, vb );
		//al.showarray();
		al.showproperties();
		d = al.Defficiencies();
		if ( verbose>=2 )
			printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );
		if ( fabs ( d[0]-al.Defficiency() ) >1e-10 ) {
			printf ( "oaunittest: error: Defficiency of examlple array 8 not good!\n" );
		}

		al = exampleArray ( 13, vb );
		//al.showarray();
		//al.showproperties();

		d = al.Defficiencies ( 0,1 );
		if ( verbose>=2 )
			printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );

		if ( ( fabs ( d[0]-0.939014 ) > 1e-4 ) || ( fabs ( d[3]-0.896812 ) > 1e-4 )  || ( fabs ( d[2]-1 ) > 1e-4 ) ) {
			printf ( "ERROR: D-efficiencies of example array 13 incorrect! \n" );
			d = al.Defficiencies ( 2,1 );
			printf ( "  efficiencies: D %f Ds %f D1 %f Ds0 %f\n", d[0], d[1], d[2], d[3] );

			allgood=UERROR;
			exit ( 1 );
		}

		for ( int ii=11; ii<11; ii++ ) {
			printf ( "ii %d: ", ii );
			al = exampleArray ( ii, vb );
			al.showarray();
			al.showproperties();

			d = al.Defficiencies();
			//if ( verbose>=2 )
			printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );
		}

	}
	{
		cprintf ( verbose,"%s: test robustness\n", bstr );

		array_link A ( 0,8,0 );
		printf ( "should return an error\n  " );
		A.Defficiencies();

		A = array_link ( 1,8,0 );
		printf ( "should return an error\n  " );
		A.at ( 0,0 ) =-2;
		A.Defficiencies();
	}

	{
		cprintf ( verbose,"%s: test nauty\n", bstr );

		array_link alr = exampleArray ( 7, 0 );
		if ( unittest_nautynormalform ( alr, 1 ) ==0 ) {
			printf ( "oaunittest: error: unittest_nautynormalform returns an error!\n" );
		}

	}
#ifdef HAVE_BOOST
=======
        array_link al = exampleArray ( 2 );
        Eigen::MatrixXd m1 = array2xfeigen ( al );
        Eigen::MatrixXd m2 = arraylink2eigen ( array2xf ( al ) );

        Eigen::MatrixXd dm = m1-m2;
        int sum = dm.sum();

        myassert ( sum==0, "unittest error: construction of interaction matrices\n" );

>>>>>>> 0019c2ab85111f901159982b3686d30e2c5f93ad

    }

#ifdef OADEV
    /* conference matrices */
    {
        cprintf ( verbose,"%s: conference matrices\n", bstr );

        int N=4;
        conference_t ctype ( N, N );

        arraylist_t kk;
        array_link al = ctype.create_root();
        kk.push_back ( al );

        for ( int extcol=2; extcol<N; extcol++ ) {
            kk = extend_conference ( kk, ctype,  0 );
        }
        myassert ( kk.size() ==1, "unittest error: conference matrices for N=4\n" );
    }

    {
        cprintf ( verbose,"%s: conference matrix Fvalues\n", bstr );
        array_link  al = exampleArray ( 22, 0 );
        if ( verbose>=2 )
            al.show();
        std::vector<int> f3 = al.FvaluesConference ( 3 );
        if ( verbose>=2 ) {
            printf ( "F3: " );
            display_vector ( f3 );
            printf ( "\n" );
        }
        const int N = al.n_rows;
        jstructconference_t js ( N, 4 );
        std::vector<int> f4 = al.FvaluesConference ( 4 );
        std::vector<int> j4 = js.Jvalues();
        
        myassert ( j4[0]==28, "unittest error: conference matricex F values: j4[0]\n" );
        myassert ( f4[0]==0, "unittest error: conference matricex F values: f4[0] \n" );
        myassert ( f4[1]==0, "unittest error: conference matricex F values: j4[1]\n" );
        
        if ( verbose>=2 ) {
            printf ( "j4: " );
            display_vector ( j4 );
            printf ( "\n" );
            printf ( "F4: " );
            display_vector ( f4 );
            printf ( "\n" );
        }
    }

    {
        cprintf ( verbose,"%s: random transformation for conference matrices\n", bstr );

        array_link al = exampleArray ( 19,1 );
        conference_transformation_t T ( al );
        //T.randomizerowflips();
        T.randomize();

        conference_transformation_t Ti = T.inverse();
        array_link alx = Ti.apply ( T.apply ( al ) );

        if ( 0 ) {
            printf ( "input array:\n" );
            al.showarray();
            T.show();
            printf ( "transformed array:\n" );
            T.apply ( al ).showarray();
            Ti.show();
            alx.showarray();
        }

        myassert ( alx==al, "transformation of conference matrix\n" )	;
    }
#endif

    /* constructors */
    {
        cprintf ( verbose,"%s: constructors\n", bstr );

        array_transformation_t t;
        conference_transformation_t ct;

    }

    /* J-characteristics */
    {
        cprintf ( verbose,"%s: J-characteristics\n", bstr );

        array_link al=exampleArray ( 8,1 );

        const int mm[]= {-1,-1,0, 0, 8,16,0,-1};

        for ( int jj=2; jj<7; jj++ ) {
            std::vector<int> jx = al.Jcharacteristics ( jj );
            int j5max = vectormax ( jx, 0 );
            if ( verbose>=2 ) {
                printf ( "oaunittest: jj %d: j5max %d\n", jj, j5max );
            }

            if ( j5max!=mm[jj] ) {
                printfd ( "j5max %d (should be %d)\n", j5max, mm[jj] );
                allgood=UERROR;
                return allgood;
            }
        }
    }
    {
        cprintf ( verbose, "%s: array transformations\n", bstr );

        const int N=9;
        const int t = 3;
        arraydata_t adataX ( 3, N, t, 4 );

        array_link al ( adataX.N,adataX.ncols,-1 );
        al.create_root ( adataX );

        if ( checkTransformationInverse ( al ) )
            allgood=UERROR;

        if ( checkTransformationComposition ( al ) )
            allgood=UERROR;

        al = exampleArray ( 5, 1 );
        if ( checkTransformationInverse ( al ) )
            allgood=UERROR;

        if ( checkTransformationComposition ( al ) )
            allgood=UERROR;


        for ( int i=0; i<15; i++ ) {
            al=exampleArray ( 18,0 );
            if ( checkConferenceComposition ( al ) )
                allgood=UERROR;
            if ( checkConferenceInverse ( al ) )
                allgood=UERROR;
            al=exampleArray ( 19,0 );
            if ( checkConferenceComposition ( al ) )
                allgood=UERROR;
            if ( checkConferenceInverse ( al ) )
                allgood=UERROR;

        }

    }

    {
        cprintf ( verbose, "%s: rank \n", bstr );

        const int idx[10]= {0, 1, 2, 3, 4, 6, 7, 8, 9};
        const int rr[10]= {4,11,13,18,16, 4, 4,29,29};
        for ( int ii=0; ii<9; ii++ ) {
            array_link al = exampleArray ( idx[ii], 0 );
            myassert ( al.is2level(), "unittest error: input array is not 2-level\n" );

            int r = arrayrankColPiv ( array2xf ( al ) );

            int r3 = ( array2xf ( al ) ).rank();
            myassert ( r==r3, "unittest error: rank of array" );

            if ( verbose>=2 ) {
                al.showarray();
                printf ( "unittest: rank of array %d: %d\n", idx[ii], r );
            }

            //array2xf ( al ) .showarray();
            myassert ( rr[ii]==r, "unittest error: rank of example matrix\n" );
        }

    }

    {
        cprintf ( verbose, "%s: Doptimize \n", bstr );
        const int N=40;
        const int t = 0;
        arraydata_t arrayclass ( 2, N, t, 6 );
        std::vector<double> alpha ( 3 );
        alpha[0]=1;
        alpha[1]=1;
        alpha[2]=0;
        int niter=5000;
        double t00 =get_time_ms();
        DoptimReturn rr =  Doptimize ( arrayclass, 10, alpha, 0, DOPTIM_AUTOMATIC, niter );

        array_t ss[7] = {3,3,2,2,2,2,2};
        arraydata_t arrayclassmixed ( ss, 36, t, 5 );
        rr =  Doptimize ( arrayclassmixed, 10, alpha, 0, DOPTIM_AUTOMATIC, niter );

        cprintf ( verbose, "%s: Doptimize time %.3f [s] \n", bstr, get_time_ms() - t00 );
    }


    {
        cprintf ( verbose, "%s: J-characteristics for conference matrix\n", bstr );

        array_link al = exampleArray ( 19, 0 );
        std::vector<int> j2 = Jcharacteristics_conference ( al, 2 );
        std::vector<int> j3 = Jcharacteristics_conference ( al, 3 );

        myassert ( j2[0]==0, "j2 value incorrect" );
        myassert ( j2[1]==0, "j2 value incorrect" );
        myassert ( std::abs ( j3[0] ) ==1, "j3 value incorrect" );

        if ( verbose>=2 ) {
            al.showarray();
            printf ( "j2: " );
            display_vector ( j2 );
            printf ( "\n" );
            printf ( "j3: " );
            display_vector ( j3 );
            printf ( "\n" );
        }
    }

    {
// test PEC sequence
        cprintf ( verbose, "%s: PEC sequence\n", bstr );
        for ( int ii=0; ii<5; ii++ ) {
            array_link al  = exampleArray ( ii, 0 );
            std::vector<double> pec = PECsequence ( al );
            printf ( "oaunittest: PEC for array %d: ", ii );
            display_vector ( pec );
            printf ( " \n" );
        }
    }


    {
        cprintf ( verbose, "%s: D-efficiency test\n", bstr );
        //  D-efficiency near-zero test
        {
            array_link al = exampleArray ( 14 );
            double D= al.Defficiency();
            std::vector<double> dd = al.Defficiencies();
            printf ( "D %f, D (method 2) %f\n", D, dd[0] );
            assert ( fabs ( D-dd[0] ) <1e-4 );
        }
        {
            array_link al = exampleArray ( 15 );
            double D= al.Defficiency();
            std::vector<double> dd = al.Defficiencies();
            printf ( "D %f, D (method 2) %f\n", D, dd[0] );
            assert ( fabs ( D-dd[0] ) <1e-4 );
            assert ( fabs ( D-0.335063 ) < 1e-3 );
        }

    }

    arraydata_t adata ( 2, 20, 2, 6 );
    OAextend oaextendx;
    oaextendx.setAlgorithm ( ( algorithm_t ) MODE_ORIGINAL, &adata );

    std::vector<arraylist_t> aa ( adata.ncols+1 );
    printf ( "OA unittest: create root array\n" );
    create_root ( &adata, aa[adata.strength] );

    /** Test extend of arrays **/
    {
        cprintf ( verbose, "%s: extend arrays\n", bstr );

        setloglevel ( SYSTEM );

        for ( int kk=adata.strength; kk<adata.ncols; kk++ ) {
            aa[kk+1] = extend_arraylist ( aa[kk], adata, oaextendx );
            printf ( "  extend: column %d->%d: %ld->%ld arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size() );

        }

        if ( aa[adata.ncols].size() != 75 ) {
            printf ( "extended ?? to %d arrays\n", ( int ) aa[adata.ncols].size() );
        }
        myassert ( aa[adata.ncols].size() ==75, "number of arrays is incorrect" );

        aa[adata.ncols].size();
        setloglevel ( QUIET );
    }



    {
        /** Test dof **/
        cprintf ( verbose, "%s: test delete-one-factor reduction\n", bstr );

        array_link al = exampleArray ( 4 );
        cprintf ( verbose>=2, "LMC: \n" );
        al.reduceLMC();
        cprintf ( verbose>=2, "DOP: \n" );
        al.reduceDOP();
    }

    arraylist_t lst;

    {
        /** Test different methods **/
        cprintf ( verbose, "%s: test 2 different methods\n", bstr );

        const int s = 2;
        arraydata_t adata ( s, 32, 3, 10 );
        arraydata_t adata2 ( s, 32, 3, 10 );
        OAextend oaextendx;
        oaextendx.setAlgorithm ( ( algorithm_t ) MODE_ORIGINAL, &adata );
        OAextend oaextendx2;
        oaextendx2.setAlgorithm ( ( algorithm_t ) MODE_LMC_2LEVEL, &adata2 );

        printf ( "OA unittest: test 2-level algorithm on %s\n", adata.showstr().c_str() );
        std::vector<arraylist_t> aa ( adata.ncols+1 );
        create_root ( &adata, aa[adata.strength] );
        std::vector<arraylist_t> aa2 ( adata.ncols+1 );
        create_root ( &adata, aa2[adata.strength] );

        setloglevel ( SYSTEM );

        for ( int kk=adata.strength; kk<adata.ncols; kk++ ) {
            aa[kk+1] = extend_arraylist ( aa[kk], adata, oaextendx );
            aa2[kk+1] = extend_arraylist ( aa2[kk], adata2, oaextendx2 );
            printf ( "  extend: column %d->%d: %ld->%ld arrays, 2-level method %ld->%ld arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size(), aa2[kk].size(), aa2[kk+1].size() );

            if ( aa[kk+1]!=aa2[kk+1] ) {
                printf ( "oaunittest: error: 2-level algorithm unequal to original algorithm\n" );
                exit ( 1 );
            }
        }
        setloglevel ( QUIET );

        lst = aa[8];
    }

    {
            cprintf ( verbose, "%s: rank calculation using rankStructure\n", bstr );

            for ( int i=0; i<27; i++ ) {
            array_link al = exampleArray ( i,0 );
            if ( al.n_columns<5 )
                continue;
            al = exampleArray ( i,1 );

            rankStructure rs;
            rs.verbose=0;
            int r = array2xf ( al ).rank();
            int rc = rs.rankxf ( al );
            if ( verbose>=2 ) {
                printf ( "rank of example array %d: %d %d\n", i, r, rc );
            if ( verbose>=3 ) {
                al.showproperties();
            }
            }
            myassert ( r==rc, "rank calculations" );
        }
}
    {
        cprintf ( verbose, "%s: test dtable creation\n", bstr );
        double t0x=get_time_ms();

        for ( int i=0; i<4; i++ ) {
            array_link al = exampleArray ( 5 );
            array_link dtable = createJdtable ( al );
        }
    }

    {
        cprintf ( verbose, "%s: test Pareto calculation\n", bstr );
        double t0x=get_time_ms();

        int nn=lst.size();
        for ( int k=0; k<5; k++ ) {
            for ( int i=0; i<nn; i++ ) {
                lst.push_back ( lst[i] );
            }
        }
        Pareto<mvalue_t<long>,long> r = parsePareto ( lst, 1 );
        cprintf ( verbose,"%s: test Pareto %d/%d: %.3f [s]\n", bstr, r.number(), r.numberindices(), ( get_time_ms() - t0x ) );

    }

    {
        cprintf ( verbose,"%s: check reduction transformation\n", bstr );
        array_link al = exampleArray ( 6 ).reduceLMC();

        arraydata_t adata=arraylink2arraydata ( al );
        LMCreduction_t reduction ( &adata );
        reduction.mode=OA_REDUCE;

        reduction.init_state=COPY;
        OAextend oaextend;
        oaextend.setAlgorithm ( MODE_ORIGINAL, &adata );
        array_link alr = al.randomperm();

        array_link al2=reduction.transformation->apply ( al );

        //printf("input: \n"); al2.showarray();
        lmc_t tmp = LMCcheck ( alr,adata, oaextend, reduction );

        array_link alx = reduction.transformation->apply ( alr );

        bool c = alx==al;
        if ( !c ) {
            printf ( "oaunittest: error: reduction of randomized array failed!\n" );
            printf ( "-- al \n" );
            al.showarraycompact();
            printf ( "-- alr \n" );
            alr.showarraycompact();
            printf ( "-- alx \n" );
            alx.showarraycompact();
            allgood=UERROR;
        }
    }

    {
        cprintf ( verbose,"%s: reduce randomized array\n", bstr );
        array_link al = exampleArray ( 3 );

        arraydata_t adata=arraylink2arraydata ( al );
        LMCreduction_t reduction ( &adata );

        for ( int ii=0; ii<50; ii++ ) {
            reduction.transformation->randomize();
            //reduction.transformation->randomizecolperm();
            array_link al2=reduction.transformation->apply ( al );

            //printf("input: \n"); al2.showarray();

            array_link alr=al2.reduceLMC();
            if ( 0 ) {
                printf ( "\n reduction complete:\n" );
                al2.showarray();
                printf ( "	--->\n" );
                alr.showarray();
            }
            bool c= ( al==alr );
            if ( !c ) {
                printf ( "oaunittest: error: reduction of randomized array failed!\n" );
                allgood=UERROR;
            }

        }
    }



    /* Calculate symmetry group */
    {
        cprintf ( verbose,"%s: calculate symmetry group\n", bstr );

        const arraylist_t &arraylist = aa[5];

        array_link al=arraylist.at ( 0 );
        symmetry_group sg = al.row_symmetry_group();

        aa.resize ( 0 );
    }

    /* Test efficiencies */
    {
        cprintf ( verbose,"%s: efficiencies\n", bstr );

        std::vector<double> d;
        int vb=1;

        array_link al;
        if ( 1 ) {
            al = exampleArray ( 9, vb );
            al.showproperties();
            d = al.Defficiencies ( 0, 1 );
            //printf("verbose: %d\n", verbose);
            if ( verbose>=2 )
                printf ( "  efficiencies: D %f Ds %f D1 %f Ds0 %f\n", d[0], d[1], d[2], d[3] );
            if ( fabs ( d[0]-al.Defficiency() ) >1e-10 ) {
                printf ( "oaunittest: error: Defficiency not good!\n" );
                allgood=UERROR;

            }
        }
        al = exampleArray ( 8, vb );
        //al.showarray();
        al.showproperties();
        d = al.Defficiencies();
        if ( verbose>=2 )
            printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );
        if ( fabs ( d[0]-al.Defficiency() ) >1e-10 ) {
            printf ( "oaunittest: error: Defficiency of examlple array 8 not good!\n" );
        }

        al = exampleArray ( 13, vb );
        //al.showarray();
        //al.showproperties();

        d = al.Defficiencies ( 0,1 );
        if ( verbose>=2 )
            printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );

        if ( ( fabs ( d[0]-0.939014 ) > 1e-4 ) || ( fabs ( d[3]-0.896812 ) > 1e-4 )  || ( fabs ( d[2]-1 ) > 1e-4 ) ) {
            printf ( "ERROR: D-efficiencies of example array 13 incorrect! \n" );
            d = al.Defficiencies ( 2,1 );
            printf ( "  efficiencies: D %f Ds %f D1 %f Ds0 %f\n", d[0], d[1], d[2], d[3] );

            allgood=UERROR;
            exit ( 1 );
        }

        for ( int ii=11; ii<11; ii++ ) {
            printf ( "ii %d: ", ii );
            al = exampleArray ( ii, vb );
            al.showarray();
            al.showproperties();

            d = al.Defficiencies();
            //if ( verbose>=2 )
            printf ( "  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2] );
        }

    }
    {
        cprintf ( verbose,"%s: test robustness\n", bstr );

        array_link A ( 0,8,0 );
        printf ( "should return an error\n  " );
        A.Defficiencies();

        A = array_link ( 1,8,0 );
        printf ( "should return an error\n  " );
        A.at ( 0,0 ) =-2;
        A.Defficiencies();
    }

    {
        cprintf ( verbose,"%s: test nauty\n", bstr );

        array_link alr = exampleArray ( 7, 0 );
        if ( unittest_nautynormalform ( alr, 1 ) ==0 ) {
            printf ( "oaunittest: error: unittest_nautynormalform returns an error!\n" );
        }

    }
    
#ifdef HAVE_BOOST
    if ( writetests ) {
        cprintf ( verbose,"OA unittest: reading and writing of files\n" );

        boost::filesystem::path tmpdir = boost::filesystem::temp_directory_path();
        boost::filesystem::path temp = boost::filesystem::unique_path ( "test-%%%%%%%.oa" );

        const std::string tempstr    = ( tmpdir / temp ).native(); // optional

        if ( verbose>=2 )
            printf ( "generate text OA file: %s\n", tempstr.c_str() );

        int nrows=16;
        int ncols=8;
        int narrays=10;
        arrayfile_t afile ( tempstr.c_str(), nrows, ncols, narrays, ATEXT );
        for ( int i=0; i<narrays; i++ ) {
            array_link al ( nrows, ncols, array_link::INDEX_DEFAULT );
            afile.append_array ( al );
        }
        afile.closefile();

        arrayfile_t af ( tempstr.c_str(), 0 );
        std::cout << "  "<<  af.showstr() << std::endl;
        af.closefile();

        // check read/write of binary file
        
        arraylist_t ll0; ll0.push_back(exampleArray(22)); ll0.push_back(exampleArray(22).randomperm() );
        writearrayfile(tempstr.c_str(), ll0, ABINARY );
        arraylist_t ll = readarrayfile(tempstr.c_str() );
        myassert(ll0.size()==ll.size(), "read and write of arrays: size of list");
        for(size_t i=0; i<ll0.size(); i++ )
        {
            myassert(ll0[i]==ll[i], "read and write of arrays: array unequal");
        }

        ll0.resize(0); ll0.push_back(exampleArray(24));
        writearrayfile(tempstr.c_str(), ll0, ABINARY_DIFFZERO );
         ll = readarrayfile(tempstr.c_str() );
        myassert(ll0.size()==ll.size(), "read and write of arrays: size of list");
        for(size_t i=0; i<ll0.size(); i++ )
        {
            myassert(ll0[i]==ll[i], "read and write of arrays: array unequal");
        }
    }

#endif

    {
        cprintf ( verbose,"OA unittest: test nauty\n" );
        array_link al = exampleArray ( 5, 2 );
        arraydata_t arrayclass = arraylink2arraydata ( al );

        for ( int i=0; i<20; i++ ) {
            array_link alx = al;
            alx.randomperm();
            array_transformation_t t1 = reduceOAnauty ( al );
            array_link alr1 = t1.apply ( al );

            array_transformation_t t2 = reduceOAnauty ( alx );
            array_link alr2 = t2.apply ( alx );


            if ( alr1 != alr2 )
                printf ( "oaunittest: error: Nauty reductions unequal!\n" );
            allgood=UERROR;


        }
    }

    cprintf ( verbose,"OA unittest: complete %.3f [s]!\n", ( get_time_ms() - t0 ) );
    cprintf ( verbose,"OA unittest: also run ptest.py to perform checks!\n" );

    if (	allgood ) {
        printf ( "OA unittest: all tests ok\n" );
        return UGOOD;
    } else {
        printf ( "OA unittest: ERROR!\n" );
        return UERROR;
    }
}

/*
Test sorting algorithm for conference designs
*/

/* calc_rowsort: Helper function to sort rows of a conference array
al: link to the array, pointer
colperm: current column permutation of the array, pointer
n_rows: number of rows of the array, int
n_cols: number of columns of the array, int
*/
indexsort calc_rowsort(const array_link &al, rowsort_t *rowperm, std::vector<int> &colperm, std::vector<int> &rowsignperm, std::vector<int> &colsignperm, const rowindex_t n_rows, const colindex_t n_cols)
{
    // Find.. Test stand alone
    std::vector<mvalue_t<int> > rr;
    for ( int i=0; i < n_rows; i++ ) {
        mvalue_t<int> m;
        for ( int k=0; k < n_cols; k++ )
            // We transform the elements (0,1,-1) to (0,1,2)
            // To perform the sort. We use tha transformations of the array
            //(colsignperm[colperm[k]]*rowsignperm[rowperm[i].val])
            m.v.push_back ( ( ( al.at( rowperm[i].r, colperm[k] ) )+3) % 3 );
        rr.push_back ( m );
    }
    indexsort is ( rr );
    return rr;
}

/*
LMC0_sortrows: Compute the new row sort for the array
\brief Calculate the new order and assign it to the existing
al: link to the array, pointer
colperm: current column permutation of the array, pointer
n_rows: number of rows of the array, int
n_cols: number of columns of the array, int
*/
void LMC0_sortrows ( const array_link &al, rowsort_t *rowperm, std::vector<int> &colperm, std::vector<int> &rowsignperm, std::vector<int> &colsignperm, const rowindex_t n_rows, const colindex_t n_cols )
{
    indexsort aa = calc_rowsort(al, rowperm, colperm, rowsignperm, colsignperm, n_rows, n_cols);
    // Assign the new sorting of the rows
    for (rowindex_t j = 0; j < n_rows; j++){
        rowperm[j].val = aa.indices[j];//rowperm[ aa.indices[j] ].val = rowperm[j].r;
    }

}

template <class numtype>
/**
 * Initialiaze a permutation
 * @param perm
 * @param len
 */
void init_signperm ( std::vector<numtype> &signperm)
{
    for ( size_t i=0; i<signperm.size(); i++ )
        signperm[i]=1;
}
template <class numtype>
void init_index ( std::vector<numtype> &index)
{
    int ni = index.size();
    for ( size_t i=0; i < ni; i++ )
        index[i] = pow( (double) 2, (ni - i)); // weights for each of the column
}

/*
* @brief Returns the value of (part of) a row
* @param array
* @param start_idx
* @param end_idx
* @param n_rows
* @param index Value index for each of the columns of the array
* @return
*/
static inline int lmc0_row_rank ( const carray_t *array, rowindex_t n_rows, const std::vector<int> &index, const std::vector<int> &colsignperm,
                                  colindex_t start_idx, colindex_t end_idx, const std::vector<int> &colperm, rowindex_t row, const int sign_row )
{
int	sum = 0;
const array_t *ar = array+row;
for ( colindex_t i = start_idx; i < end_idx; i++ ) {
//sum += index[i] * ar[n_rows*colperm[i]]; // for Orthogonal arrays
    int sel_element = (colsignperm[colperm[i]]*sign_row) * ar[n_rows*colperm[i]]; // Transform from (0, 1, -1) to (1, 2, 0)
    sum += index[i] * ( (sel_element+3) % 3) ; // for conference designs, Transform from (1, 2, 0) to (0, 1, 2)
}
return sum;
}

/**
* @brief Sort an array LMC0
* @param array My array
* @param ad Root array NO ROOT ARRAY
* @param dyndata Contains information about current transformation of the array
* @param rowsort
*/
inline void LMC0_sort ( const carray_t *array, const std::vector<int> &valueindex, rowsort_t *rowperm, const std::vector<int> &colperm,
                         const std::vector<int> &rowsignperm, const std::vector<int> &colsignperm, const rowindex_t n_rows,
                        const colindex_t n_cols )
{
    //vindex_t *valueindex = new_valueindex<vindex_t> ( ad->s, ad->strength );	// OPTIMIZE: make static allocation?

    //rowsort_t *rowsort = dyndata->rowsort;
    /* perform initial sort, after the initial sort we can sort using pre-calculated structures */
    /* Assign rank to each row*/

    for ( rowindex_t j = 0; j < n_rows; j++ ) {
        int sign_row = rowsignperm[j];
        rowperm[j].val = lmc0_row_rank ( array,	 n_rows, valueindex, colsignperm, ( colindex_t ) 0, n_cols, colperm, j, sign_row );
    }
    //delete [] valueindex;

    // OPTIMIZE: select best sort (stable_sort, sort, oacolSort)
    std::stable_sort ( rowperm, rowperm+n_rows );
}

/* Start the real functions*/
/*
rowlevel_permutation: find the row-level permutation of a column
al: array link, pointer
rowperm: current row permutation, pointer, rowsort_t
colperm: current column permutation, pointer
rowsignperm: current rowsign permutation, pointer
column: the column to find the permutation, int
n_rows: number of rows, n_rows, index
*/
//template<class NumType, class NumTypeIn>
void rowlevel_permutation ( const array_link &al, rowsort_t *rowperm, const std::vector<int> &colperm, std::vector<int> &rowsignperm, const rowindex_t n_rows, int column ) {

    //std::vector<NumType> rowsignone ( nrows );
    //init_signperm ( rowsignone );
    //see what level-permutation we need to get a column with all ones and one zero
    // We don't copy the array but use the current modifications given by the rowsignperm, colperm, and rowperm
    for ( rowindex_t r=0; r < n_rows; r++ ) {
        if ( ( (rowsignperm[rowperm[r].val]) * al.atfast( rowperm[r].val, colperm[column] )) < 0 )
            rowsignperm[ rowperm[r].val ] = -1; // assign new values to the rowsignperm vector
    }

}

/* Sort test */
int cdes_sort_test ( const array_link &al ){

    // Get size of the array
    const int ncols = al.n_columns; // number of columns, rowindex_t
    const int nrows = al.n_rows; // number of rows, colindex_t
    int matrix[ nrows ][ ncols ];

    // Initialize the column permutation
    std::vector<int> colperm ( ncols ); // create pointer for the column permutations
    init_perm ( colperm );
    //initialize column level permutation
    std::vector<int> colsignperm ( ncols );
    init_signperm ( colsignperm );
    //initialize row-level permutation
    std::vector<int> rowsignperm ( nrows );
    init_signperm ( rowsignperm );
    // Initialize row order
    dyndata_t rowperm_data = dyndata_t ( nrows );//rowperm_data.show();
    rowsort_t *rowsortone = rowperm_data.rowsort;
    rowsort_t *rowsorttwo = rowperm_data.rowsort;
    int myints[] = {0, 9, 6, 1, 4, 7, 3, 8, 5, 2};
    std::vector<int> myvec(myints, myints + sizeof(myints));
    for (rowindex_t i = 0; i < nrows; i++){
        rowsortone[i].val = myvec[i];
        rowsorttwo[i].val = myvec[i];
    }
    print_rowsort(rowsortone, nrows);
    // Define index for sort
    std::vector<int> weights_index( ncols, 0 );
    for (colindex_t i = 0; i < ncols; i++){
        weights_index[ i ] = pow( (double) 2, (ncols - i));
    }
        // Swap first two column
    int first_col = colperm[ 0 ]; // copy current first column index
    colperm[ 0 ] = colperm[ 1 ]; // assign to be the first one
    colperm[ 1 ] = first_col; // finish the swap
    rowlevel_permutation ( al, rowsortone, colperm, rowsignperm, nrows, 0 );//

    std::cout <<  "Transformed Array " << std::endl;
    for (int r = 0; r < nrows; r++){
        for (int col = 0; col < ncols; col++){
            matrix[r][col] = (colsignperm[colperm[col]]*rowsignperm[rowsortone[r].val]) * al.atfast ( rowsortone[r].val, colperm[col] );
            std::cout << matrix[r][col] << "    ";
        }
        std::cout << std::endl;
    }


    std::cout <<  "My Sort " << std::endl;
    LMC0_sortrows( al, rowsortone, colperm, rowsignperm, colsignperm, nrows, ncols );
    printf ( "Row order-sort  " );
    print_rowsort(rowsortone, nrows);
    //al.array is a pointer where the array is located

    /*
    std::cout <<  "Sorted array C++ " << std::endl;
    for (int r = 0; r < nrows; r++){
        for (int col = 0; col < ncols; col++){
            matrix[r][col] = (colsignperm[colperm[col]]*rowsignperm[rowsort[r].val]) * al.atfast ( rowsort[r].val, colperm[col] );
            std::cout << matrix[r][col] << " ";
        }
        std::cout << std::endl;
    }
    */
    //Now we get all the data from the matrix into a vector.
    //smat = sort_matrix(matrix, nrows, ncols);
    //print_matrix(smat, nrows, ncols);
    return 0;

}


/**
* @brief Read in files with arrays and join them into a single file
* @param argc
* @param argv[]
* @return
*/
int main ( int argc, char* argv[] ) {

    AnyOption opt;
    opt.setFlag ( "help", 'h' );   /* a flag (takes no argument), supporting long and short form */
    opt.setOption ( "verbose", 'v' );
    opt.setOption ( "random", 'r' );

    opt.addUsage ( "OA: unittest: Perform some checks on the code" );
    opt.addUsage ( "Usage: unittest [OPTIONS]" );
    opt.addUsage ( "" );
    opt.addUsage ( " -v  --verbose  			Print documentation" );
    opt.addUsage ( " -r  --random  			Seed for random number generator" );

    opt.processCommandArgs ( argc, argv );
    int verbose = opt.getIntValue ( 'v', 1 );
    int random = opt.getIntValue ( 'r', 0 );

    if ( opt.getFlag ( "help" ) || opt.getFlag ( 'h' ) ) {
        opt.printUsage();
        exit ( 0 );
    }


    if ( verbose ) {
        print_copyright();
    }
    if ( verbose>=2 ) {
        print_options ( std::cout );
    }

<<<<<<< HEAD
	//oaunittest ( verbose, 1, random );
    char *input = opt.getValue ( 'I' );
    if ( input==0 )
        input="cdesign-10-10.oa";

	printf("### read from file\n");
    arraylist_t ll= readarrayfile ( input );


    for ( size_t i=0; i<ll.size(); i++ ) {
        array_link al = ll[i];
        al.showarray();
        cdes_sort_test ( al );
        for (int trans = 0; trans < 1; trans++){
                /* Transform the array */
            conference_transformation_t T1(al);
	        T1.randomize();
	        //T1.show();
	        array_link al1 = T1.apply ( al );
	        al1.showarray(); // Show transformed array
	        cdes_sort_test ( al1 );

        }


    }

=======
    oaunittest ( verbose, 1, random );
>>>>>>> 0019c2ab85111f901159982b3686d30e2c5f93ad


    return 0;
}
<<<<<<< HEAD
// kate: indent-mode cstyle; indent-width 4; replace-tabs off; tab-width 4;
=======
// kate: indent-mode cstyle; indent-width 4; replace-tabs on; 
>>>>>>> 0019c2ab85111f901159982b3686d30e2c5f93ad
