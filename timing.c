#if defined MAIN || defined CINC
#include <sys/types.h>
#include <immintrin.h>
#include <inttypes.h>
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
    *(result)=(tv.tv_sec*((tv.tv_usec >= 1000000) ? 6 : (tv.tv_usec >= 100000) ? 5 : (v >= 10000) ? 4 : 
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

#ifdef BENCH
void benchmark(char *cmd, char *runs)
{
    __m256i total=_mm256_set_epi64x(0,0,0,0);
    __m256i average=_mm256_set_epi64x(0,0,0,0);
    u_int64_t slowest=0;
    u_int64_t fastest=18446744073709551615ULL;
    __m256i temp=_mm256_set_epi64x(0,0,0,0);
    
    u_int64_t start=0;
    u_int64_t stop=0;
    u_int64_t result=0;

    for(i=0; i < runs; i++)
    {

        timestamp(&start);
        exec(cmd);
        timestamp(&stop);

        result=stop-start;

        temp=_mm256_set_epi64x(0,0,0,result);
        total=_mm256_add_epi64(total,temp);

        if ( result < fastest)
        {
            fastest=result;
        }

        if ( result > slowest)
        {
            slowest=result;
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
}

#ifdef MAIN
int main(int argc, char *argv[])
{
    benchmark(argv[1],strtol(argv[2],NULL,10));

    return(0);
}
#endif
