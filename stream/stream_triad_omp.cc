/*-----------------------------------------------------------------------*/
/* Program: Stream                                                       */
/* Revision: $Id: stream_omp.c,v 5.4 2009/02/19 13:57:12 mccalpin Exp mccalpin $ */
/* Original code developed by John D. McCalpin                           */
/* Programmers: John D. McCalpin                                         */
/*              Joe R. Zagar                                             */
/*                                                                       */
/* This program measures memory transfer rates in MB/s for simple        */
/* computational kernels coded in C.                                     */
/*-----------------------------------------------------------------------*/
/* Copyright 1991-2003: John D. McCalpin                                 */
/*-----------------------------------------------------------------------*/
/* License:                                                              */
/*  1. You are free to use this program and/or to redistribute           */
/*     this program.                                                     */
/*  2. You are free to modify this program for your own use,             */
/*     including commercial use, subject to the publication              */
/*     restrictions in item 3.                                           */
/*  3. You are free to publish results obtained from running this        */
/*     program, or from works that you derive from this program,         */
/*     with the following limitations:                                   */
/*     3a. In order to be referred to as "STREAM benchmark results",     */
/*         published results must be in conformance to the STREAM        */
/*         Run Rules, (briefly reviewed below) published at              */
/*         http://www.cs.virginia.edu/stream/ref.html                    */
/*         and incorporated herein by reference.                         */
/*         As the copyright holder, John McCalpin retains the            */
/*         right to determine conformity with the Run Rules.             */
/*     3b. Results based on modified source code or on runs not in       */
/*         accordance with the STREAM Run Rules must be clearly          */
/*         labelled whenever they are published.  Examples of            */
/*         proper labelling include:                                     */
/*         "tuned STREAM benchmark results"                              */
/*         "based on a variant of the STREAM benchmark code"             */
/*         Other comparable, clear and reasonable labelling is           */
/*         acceptable.                                                   */
/*     3c. Submission of results to the STREAM benchmark web site        */
/*         is encouraged, but not required.                              */
/*  4. Use of this program or creation of derived works based on this    */
/*     program constitutes acceptance of these licensing restrictions.   */
/*  5. Absolutely no warranty is expressed or implied.                   */
/*-----------------------------------------------------------------------*/
# include <stdio.h>
# include <math.h>
# include <float.h>
# include <limits.h>
# include <sys/time.h>
# include <memory>
#include <omp.h>

/* INSTRUCTIONS:
 *
 *	1) Stream requires a good bit of memory to run.  Adjust the
 *          value of 'N' (below) to give a 'timing calibration' of 
 *          at least 20 clock-ticks.  This will provide rate estimates
 *          that should be good to about 5% precision.
 */

/*
 *	3) Compile the code with full optimization.  Many compilers
 *	   generate unreasonably bad code before the optimizer tightens
 *	   things up.  If the results are unreasonably good, on the
 *	   other hand, the optimizer might be too smart for me!
 *
 *         Try compiling with:
 *               cc -O stream_omp.c -o stream_omp
 *
 *         This is known to work on Cray, SGI, IBM, and Sun machines.
 *
 *
 *	4) Mail the results to mccalpin@cs.virginia.edu
 *	   Be sure to include:
 *		a) computer hardware model number and software revision
 *		b) the compiler flags
 *		c) all of the output from the test case.
 * Thanks!
 *
 */

# define HLINE "-------------------------------------------------------------\n"

# ifndef MIN
# define MIN(x,y) ((x)<(y)?(x):(y))
# endif
# ifndef MAX
# define MAX(x,y) ((x)>(y)?(x):(y))
# endif

extern double mysecond();
extern void checkSTREAMresults();
#ifdef TUNED
extern void tuned_STREAM_Copy();
extern void tuned_STREAM_Scale(double scalar);
extern void tuned_STREAM_Add();
extern void tuned_STREAM_Triad(double scalar);
#endif
int
main()
    {
    int			quantum, checktick();
    double		scalar, t;

    register int        max_threads, nthreads;
    
    unsigned long long N = 20000000;
    unsigned long long NTIMES = 10;
    unsigned long long OFFSET = 0;

    /* --- SETUP --- determine precision and check timing --- */
    printf("require %7.3f GB of memory divided in 3 buffers\n",
	   (N+OFFSET)*3*sizeof(double) * 1e-9);
    std::unique_ptr<double[]> pa(new double[N+OFFSET]);
    std::unique_ptr<double[]> pb(new double[N+OFFSET]);
    std::unique_ptr<double[]> pc(new double[N+OFFSET]);
    double times[NTIMES];

    double *a = pa.get();
    double *b = pb.get();
    double *c = pc.get();

    // we are doing stream tri-add !!! 
    double	bytes = 3 * sizeof(double) * N;

    max_threads = omp_get_max_threads();

    /* Get initial value for system clock. */
#pragma omp parallel for num_threads(max_threads) if (max_threads > 1)
    for (unsigned long long j=0; j<N; j++) {
	a[j] = 1.0;
	b[j] = 2.0;
	c[j] = 0.0;
	}

    printf(HLINE);

    if  ( (quantum = checktick()) >= 1) 
	printf("Your clock granularity/precision appears to be "
	    "%d microseconds.\n", quantum);
    else
	printf("Your clock granularity appears to be "
	    "less than one microsecond.\n");

    t = mysecond();
#pragma omp parallel for
    for (unsigned long long j = 0; j < N; j++)
	a[j] = 2.0E0 * a[j];
    t = 1.0E6 * (mysecond() - t);


    scalar = 3.0; 
    printf("Function #threads Avg Rate (GB/s) MinRate (GB/s) MaxRate(GB/s)  Avg time     Min time     Max time\n");
    for(int nthreads = 1; nthreads <= max_threads; nthreads++){
      double avgtime = 0.0;
      double mintime = FLT_MAX;
      double maxtime =-FLT_MAX;
      for (unsigned long long k=0; k<NTIMES; k++)
	{
	  
	  times[k] = mysecond();
#ifdef TUNED
	  tuned_STREAM_Triad(a, b, c, N, scalar, scalar);
#else
#pragma omp parallel for num_threads(nthreads) if(nthreads > 1)
	  for (unsigned long long j=0; j<N; j++)
	    a[j] = b[j]+scalar*c[j];
#endif
	times[k] = mysecond() - times[k];
	}

    /*	--- SUMMARY --- */

      for (unsigned long long k=1; k<NTIMES; k++) /* note -- skip first iteration */
	{
	    avgtime = avgtime + times[k];
	    mintime = MIN(mintime, times[k]);
	    maxtime = MAX(maxtime, times[k]);
	}
    
	avgtime = avgtime/(double)(NTIMES-1);

	printf("Triad : %2d threads : %11.4f %11.4f %11.4f  %11.4f  %11.4f  %11.4f\n",
	       nthreads,
	       1.0E-09 * bytes/avgtime,
	       1.0E-09 * bytes/maxtime,
	       1.0E-09 * bytes/mintime,
	       avgtime,
	       mintime,
	       maxtime);
    }
    return 0;
}

# define	M	20

int
checktick()
    {
    int		i, minDelta, Delta;
    double	t1, t2, timesfound[M];

/*  Collect a sequence of M unique time values from the system. */

    for (i = 0; i < M; i++) {
	t1 = mysecond();
	while( ((t2=mysecond()) - t1) < 1.0E-6 )
	    ;
	timesfound[i] = t1 = t2;
	}

/*
 * Determine the minimum difference between these M values.
 * This result will be our estimate (in microseconds) for the
 * clock granularity.
 */

    minDelta = 1000000;
    for (i = 1; i < M; i++) {
	Delta = (int)( 1.0E6 * (timesfound[i]-timesfound[i-1]));
	minDelta = MIN(minDelta, MAX(Delta,0));
	}

   return(minDelta);
    }



/* A gettimeofday routine to give access to the wall
   clock timer on most UNIX-like systems.  */

#include <sys/time.h>

double mysecond()
{
        struct timeval tp;
        struct timezone tzp;
        int i;

        i = gettimeofday(&tp,&tzp);
        return ( (double) tp.tv_sec + (double) tp.tv_usec * 1.e-6 );
}
 
void tuned_STREAM_Triad(double *a, double *b, double *c, unsigned long long N, double scalar, int nthreads)
{

#pragma omp parallel for num_threads(nthreads) if (nthreads > 1)
  for (unsigned long long j=0; j<N; j++)
    a[j] = b[j]+scalar*c[j];
}

