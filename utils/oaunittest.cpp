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

#ifdef HAVE_BOOST
#include <string>
#include <boost/filesystem.hpp>
#endif

template <class Type>
void permute(Type *source, Type *target, const std::vector<int> p)
{
    for(size_t i=0; i<p.size(); i++)
        target[i]=source[p[i]];
}

/**
* @brief Read in files with arrays and join them into a single file
* @param argc
* @param argv[]
* @return
*/
int main ( int argc, char* argv[] )
{

   AnyOption opt;
    opt.setFlag ( "help", 'h' );   /* a flag (takes no argument), supporting long and short form */
    opt.setOption ( "verbose", 'v' ); 

    opt.addUsage ( "OA: unittest: Perform some checks on the code" );
    opt.addUsage ( "Usage: unittest [OPTIONS]" );
    opt.addUsage ( "" );
    opt.addUsage ( " -v  --verbose  			Print documentation" );
    
        opt.processCommandArgs ( argc, argv );
    int verbose = opt.getIntValue('v', 1);

        if ( opt.getFlag ( "help" ) || opt.getFlag ( 'h' )  ) {
        opt.printUsage();
	exit(0);
	}
	
    double t0=get_time_ms();

      if(verbose) {
    	print_copyright();
      }
      if (verbose>=2) {
	print_options(std::cout);
      }
      
      const char *bstr = "OA unittest";
    cprintf(verbose, "%s: start\n", bstr);

    arraydata_t adata(2, 20, 2, 6);
    OAextend oaextendx;
    oaextendx.setAlgorithm((algorithm_t)MODE_ORIGINAL, &adata);

    std::vector<arraylist_t> aa(adata.ncols+1);
    printf("OA unittest: create root array\n");
    create_root (&adata, aa[adata.strength] );

    {
        cprintf(verbose, "%s: array transformations\n", bstr);
        // TODO: array transformation class: check non-symmetric case

        const int N=9;
        const int t = 3;
        arraydata_t adataX(3, N, t, 4);

        array_link al(adataX.N,adataX.ncols,-1);
        al.create_root(adataX);

        array_transformation_t T1(&adataX);
        T1.randomize();
        array_transformation_t T2(&adataX);
        T2.randomize();
        array_transformation_t T3;
        T3= T1 * T2;


        array_transformation_t T1i = T1.inverse();
        array_transformation_t II = T1i * T1;

        myassert( II.isIdentity(), "unittest error: inverse of array transformation\n" );

        array_link al2 = T2.apply(al);
        array_link al12 = T1.apply(al2);
        array_link al3 = T3.apply(al);

        // T1.show(); T2.show();
        // al.showarray(); al2.showarray(); al12.showarray(); al3.showarray();

        myassert(al3==al12, "unittest error: composition of array transformations\n");
    }

    /** Test extend of arrays **/
    {
              cprintf(verbose, "%s: extend arrays\n", bstr);

        setloglevel ( SYSTEM );

        for(int kk=adata.strength; kk<adata.ncols; kk++) {
            aa[kk+1] = extend_arraylist(aa[kk], adata, oaextendx);
            printf("  extend: column %d->%d: %zu->%zu arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size());

        }
        myassert(aa[adata.ncols].size()==75, "number of arrays");

        aa[adata.ncols].size();
        setloglevel ( QUIET );
    }

        { /** Test different methods **/
                    cprintf(verbose, "%s: test delete-one-factor reduction\n", bstr);

		    array_link al = exampleArray(4);
cprintf(verbose>=2, "LMC: \n");
		    al.reduceLMC();	
cprintf(verbose>=2, "DOP: \n");
al.reduceDOP();
	}
	  
    { /** Test different methods **/
                    cprintf(verbose, "%s: test 2 different methods\n", bstr);

      const int s = 2;
          arraydata_t adata(s, 32, 3, 10);
          arraydata_t adata2(s, 32, 3, 10);
    OAextend oaextendx;
    oaextendx.setAlgorithm((algorithm_t)MODE_ORIGINAL, &adata);
    OAextend oaextendx2;
    oaextendx2.setAlgorithm((algorithm_t)MODE_LMC_2LEVEL, &adata2);

    printf("OA unittest: test 2-level algorithm on %s\n", adata.showstr().c_str() );
    std::vector<arraylist_t> aa(adata.ncols+1);
    create_root (&adata, aa[adata.strength] );
    std::vector<arraylist_t> aa2(adata.ncols+1);
    create_root (&adata, aa2[adata.strength] );

    setloglevel ( SYSTEM );

            for(int kk=adata.strength; kk<adata.ncols; kk++) {
            aa[kk+1] = extend_arraylist(aa[kk], adata, oaextendx);
            aa2[kk+1] = extend_arraylist(aa2[kk], adata2, oaextendx2);
            printf("  extend: column %d->%d: %zu->%zu arrays, 2-level method %zu->%zu arrays\n", kk, kk+1, aa[kk].size(), aa[kk+1].size(), aa2[kk].size(), aa2[kk+1].size());

	    if (aa[kk+1]!=aa2[kk+1]) {
	      printf("oaunittest: error: 2-level algorithm unequal to original algorithm\n");
	     exit(1); 
	    }
        }
setloglevel ( QUIET );

      
    }
    
    {
        cprintf(verbose,"%s: reduce randomized array\n", bstr);
        array_link al = exampleArray(3);

        arraydata_t adata=arraylink2arraydata(al);
        LMCreduction_t reduction(&adata);

        for(int ii=0; ii<50; ii++) {
            reduction.transformation->randomize();
            //reduction.transformation->randomizecolperm();
            array_link al2=reduction.transformation->apply(al);

	    //printf("input: \n"); al2.showarray();
	    
            array_link alr=al2.reduceLMC();
	    if (0) {
	    	    printf("\n reduction complete:\n");
		    al2.showarray();
	    	    printf("	--->\n");
		    alr.showarray();
	    }
            bool c= (al==alr);
            if (!c) {
                printf("oaunittest: error: reduction of randomized array failed!\n" );
            }

        }
    }
    
    {
        cprintf(verbose,"%s: calculate symmetry group\n", bstr);

        int k = 5;
        const arraylist_t &arraylist = aa[5];

        array_link al=arraylist.at(0);
        symmetry_group sg = al.row_symmetry_group();

        aa.resize(0);
    }

    /* Test efficiencies */
    {
              cprintf(verbose,"%s: efficiencies\n", bstr);

       std::vector<double> d;
      int vb=1;
      
      array_link al;
      if (1) {
     al = exampleArray(9, vb);
     al.showproperties();     
    d = al.Defficiencies();
    //printf("verbose: %d\n", verbose);
    if (verbose>=2)
     printf("  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2]);
     if ( fabs( d[0]-al.Defficiency())>1e-10 ) {
      printf("oaunittest: error: Defficiency not good!\n"); 
     }
      }
      al = exampleArray(8, vb);
      //al.showarray();
     al.showproperties();     
     d = al.Defficiencies();
    if (verbose>=2)
     printf("  efficiencies: D %f Ds %f D1 %f\n", d[0], d[1], d[2]);
     if ( fabs( d[0]-al.Defficiency())>1e-10 ) {
      printf("oaunittest: error: Defficiency not good!\n"); 
     }
      
    }
#ifdef HAVE_BOOST

    {
        cprintf(verbose,"OA unittest: reading and writing of files\n");


    boost::filesystem::path tmpdir = boost::filesystem::temp_directory_path();
    boost::filesystem::path temp = boost::filesystem::unique_path("test-%%%%%%%.oa");
    
    const std::string tempstr    = (tmpdir / temp).native();  // optional

if (verbose>=2)
    printf("generate text OA file: %s\n", tempstr.c_str() );

    int nrows=16;
    int ncols=8;
    int narrays=10;
      arrayfile_t afile(tempstr.c_str(), nrows, ncols, narrays, ATEXT );
      for(int i=0;i<narrays;i++) {
	array_link al(nrows, ncols, array_link::INDEX_DEFAULT);
	afile.append_array(al);
      }
      afile.closefile();
      
    arrayfile_t af(tempstr.c_str(), 0);
    std::cout << "  "<<  af.showstr() << std::endl;
    af.closefile();
    
    // TODO: implement writing of binary files...
    
//      oainfo(tempstr.c_str() );
	
    }
    
#endif    
    
    cprintf(verbose,"OA unittest: complete %.3f [s]!\n", (get_time_ms() - t0) );
    cprintf(verbose,"OA unittest: also run ptest.py to perform checks!\n");

    return 0;
}
