#ifdef ALLIN
    #define COMPILER "gcc "
    #define MAIN    1
    #define TIMESTAMP 1
    #define GRAPH 1
    #define AVXPRINT 1
    #define AVXMATH 1
    #define INFO 1
    #define BENCH 1
    #define COMPILE 1
#endif

#if defined MAIN || defined CINC
#include <sys/types.h>
#include <immintrin.h>
#include <inttypes.h>
#include "gnuplot_pipes.h"
#include <stdlib.h>
#endif

#ifdef MATHMACROS
#define ADD(a,b) (a+b)
#define DIV(a,b) (a/b)
#endif

#ifdef TIMESTAMP
void timestamp(u_int64_t *result)
{
    struct timeval tv;
    *(result)=0;

    gettimeofday(&tv,NULL);
    // https://graphics.stanford.edu/~seander/bithacks.html#IntegerLog10
    *(result)=(tv.tv_sec*((tv.tv_usec >= 1000000) ? 6 : (tv.tv_usec >= 100000) ? 5 : (tv.tv_usec >= 10000) ? 4 : 
    (tv.tv_usec >= 1000) ? 3 : (tv.tv_usec >= 100) ? 2 : (tv.tv_usec >= 10) ? 1 : 0))+tv.tv_usec;
}
#endif

#ifdef AVXPRINT
void print256_num(__m256i var) 
{
    int64_t *v64val = (uint64_t*) &var;
    printf("%llu %llu %llu %llu\t",v64val[3], v64val[2],v64val[1], v64val[0]);
}
#endif

#ifdef AVXMATH
// https://stackoverflow.com/questions/42442325/how-to-divide-a-m256i-vector-by-an-integer-variable 
inline __m256i _mm256_div_epi16 (const __m256i va, const int b)
{
    __m256i vb = _mm256_set1_epi16(32768 / b);
    return _mm256_mulhrs_epi16(va, vb);
}
#endif

#ifdef COMPILE
void compile(char *cmd, char *defines)
{
    char compileline[65535];

    strcpy(compileline, COMPILER);
    strcat(compileline, cmd);
    strcat(compileline, ".c ");
    strcat(compileline, defines);
    strcat(compileline, " -o ");
    strcat(compileline, cmd);

    system(compileline);
}
#endif

#ifdef BENCH
void benchmark(char *cmd, int runs, char *defines, u_int64_t *results, u_int64_t *fastest, u_int64_t *slowest)
{
    __m256i total=_mm256_set_epi64x(0,0,0,0);
    __m256i average=_mm256_set_epi64x(0,0,0,0);
    __m256i temp=_mm256_set_epi64x(0,0,0,0);
    
    u_int64_t start=0;
    u_int64_t stop=0;
    u_int64_t result=0;
    char nullpointer='\0';
    int i=0;

#ifdef COMPILE
    if ( *(cmd+(strlen(cmd)-1)) == 'c' )
    {
        compile(cmd,&nullpointer);
    }
#endif 

    for(i=0; i < runs; i=i+2)
    {

        timestamp(&start);
        system(cmd);
        timestamp(&stop);

        result=stop-start;
        if ( *(results+runs-1) == 0 )
        {
            *(results+i*2+1)=result;
        }else{
            *(results+i)=result;
        }

        temp=_mm256_set_epi64x(0,0,0,result);
        total=_mm256_add_epi64(total,temp);

        if ( result < *(fastest))
        {
            *(fastest)=result;
        }

        if ( result > *(slowest))
        {
            *(slowest)=result;
        }

    }

    printf("Slowest time: %llu \n Fastest Time %llu\n",slowest,fastest);

    #ifdef AVXPRINT
        printf("Total time is: \n");
        print256_num(total);
        printf("\n");

        printf("Average time is: \n");
        print256_num(_mm256_div_epi16(total,runs));
        printf("\n");
    #endif

#ifdef COMPILE
    if ( *(defines) != '\0' )
    {
        #ifdef INFO
            printf("Recompiling with different compile defines");
        #endif
        compile(cmd,defines);
        #ifdef INFO
            printf("Benchmarking with new defines");
        #endif
        *(defines)='\0';
        benchmark(cmd, runs, &nullpointer, results, fastest, slowest);  
    }
#endif

}
#endif

#ifdef GRAPH
/*
void plotpoint()
{
    // Plot points
    gnuplot_setstyle(h1, "points") ;
    gnuplot_plot1d_var2(h1, dp, 1,"") ;
}*/

void plotpoints(gnuplot_ctrl *h1, dpoint *dp, int runs)
{
    // Plot points
    gnuplot_setstyle(h1, "points") ;
    gnuplot_plot1d_var2(h1, dp, runs,"") ;
    
}
/*
void plotline()
{
    // Line for average
    gnuplot_setstyle(h1, "lines") ;
    gnuplot_plot_slope(h1, 2.0, 0.0, "") ;
}*/
#endif

#ifdef MAIN
int main(int argc, char *argv[])
{
    u_int64_t slowest=0;
    u_int64_t fastest=18446744073709551615ULL;
    u_int64_t *results;
    u_int64_t runs=0;
    gnuplot_ctrl *h1;

    runs=strtol(argv[2],NULL,10);
    results=calloc(runs*2,8);

#ifdef GRAPH
    h1 = gnuplot_init() ;
#endif

    benchmark(argv[1],runs,argv[3], results, &fastest, &slowest);
    plotpoints(h1, (struct _DPOINT_ *)results,runs);

#ifdef GRAPH
    gnuplot_close(h1) ;
#endif

    free(results);

    return(0);
}
#endif
