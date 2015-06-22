#include <cblas.h>
#include <omp.h>
#include <sys/time.h>
#include <stdio.h>
#include <memory>
#include <float.h>

double getNow(){
  struct timeval now;
  gettimeofday(&now, NULL);
  return (1.0*now.tv_sec + 1.0e-6*now.tv_usec);
}

int main(int argc, char** argv){
  int max_threads = omp_get_max_threads();

  const int M = 2000;
  const int K = 2000;
  const int N = 600;


  unsigned long long memReq = max_threads * (M*N + M*K + K*N) * sizeof(float);
  printf("Memoy required %8.3f GB\n", memReq*1e-9);
  float** A= new float*[max_threads];
  float** B= new float*[max_threads];
  float** C= new float*[max_threads];
  std::unique_ptr<float[]> pA(new float[max_threads*M*K]);
  std::unique_ptr<float[]> pB(new float[max_threads*K*N]);
  std::unique_ptr<float[]> pC(new float[max_threads*M*N]);
  
  float *a = pA.get();
  float *b = pB.get();
  float *c = pC.get();
  for(int th=0; th< max_threads; th++){
    A[th] = &a[M*K*th];
    B[th] = &b[K*N*th];
    C[th] = &c[M*N*th];
    //printf("th=%d A[th]=%p B[th]=%p C[th]=%p\n", th, A[th], B[th], C[th]);
  }
  const float ALPHA = 1.0f;
  const float BETA = 10.0f;
  // SGEMM flops is :
  //C = alpha * op(A)*op(B) + beta* C
  // where op is identity or transpose
  const int ITERS = 50;
#pragma omp parallel for num_threads(max_threads) if (max_threads > 1)
  for(int nthreads = 1; nthreads <= max_threads; nthreads++){
    int th = omp_get_thread_num();
    for(unsigned long long i=0; i < M*K; i++) A[th][i] = 1.0*i;
    for(unsigned long long i=0; i < K*N; i++) B[th][i] = 1.0*i;
    for(unsigned long long i=0; i < M*N; i++) C[th][i] = 1.0*i;

    // #pragma ordered 
    // {
    //   printf("th=%d A[th]=%p B[th]=%p C[th]=%p\n", th, A[th], B[th], C[th]);
    // }
    // do I want Row or Col majors ? // row fast, colum slow?
    // verify the API.
    //cblas_sgemm( CblasColMajor,  CblasNoTrans, CblasNoTrans, M, N, K, ALPHA, A[th], M, B[th], K, BETA, C[th], M ); // row major
    cblas_sgemm( CblasRowMajor,  CblasNoTrans, CblasNoTrans, M, N, K, ALPHA, A[th], K, B[th], N, BETA, C[th], N ); // row major
  }
  printf("initialized\n");

  double times[max_threads];

  for(int nthreads = 1; nthreads <= max_threads; nthreads++){
#pragma omp parallel num_threads(nthreads)
    {
      int th = omp_get_thread_num();

      double before = getNow();
      for(int it = 0; it < ITERS; it++){
	// do I want Row or Col major
	//cblas_sgemm( CblasColMajor,  CblasNoTrans, CblasNoTrans, M, N, K, ALPHA, A[th], M, B[th], K, BETA, C[th], M ); // row major 	
	cblas_sgemm( CblasRowMajor,  CblasNoTrans, CblasNoTrans, M, N, K, ALPHA, A[th], K, B[th], N, BETA, C[th], N ); // row major
      }
      double elapsed = getNow()  - before;
      times[th] = elapsed / ITERS;

    }
    double minTime = FLT_MAX;
    double maxTime =-FLT_MAX;
    double sumTime = 0.0;

    for(int i=0; i < nthreads; i++){
      double t = times[i] ;
      minTime = std::min(minTime, t);
      maxTime = std::max(maxTime, t);
      sumTime += t;
    }

    double avgTime = sumTime / (1.0*nthreads);
    double gflops = ((1.0*M)*(1.0*N)*(1.0*K)*2.0 + (1.0*M)*(1.0*N))*nthreads / avgTime *1e-9;
    printf("%d threads avgTime=%11.5f minTime=%11.5f maxTime=%11.5f, %11.5f GFlops\n", nthreads, avgTime, minTime, maxTime, gflops);
    fflush(stdout);
  }

  return 0;
}
