#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define _GNU_SOURCE
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)

#define MAXSIZE 1000000000

// https://stackoverflow.com/questions/15906600/what-is-the-most-efficient-way-to-store-prime-numbers-in-an-array
//1, 7, 11, 13, 17, 19, 23 or 29
// prime/30 = byte in array
// prime-(30*bitpos) = bit in array

#ifdef MAIN
int main(int argc, char *argv[])
{
    u_int64_t byte=1;
    u_int8_t bitpos[8] = {1,7,11,13,17,19,23,29};
    u_int8_t *primearray;
    u_int8_t mask=1;
    u_int8_t prev=0;
    u_int64_t filesize=0;
    primearray=calloc(MAXSIZE,1);

    do
    {
       prev=byte;
       scanf("%llu\n",&byte);
       filesize++;

    #ifdef INFO
        printf("%llu\n",byte);
    #endif

        switch ( byte-(byte/30*30) )
        {
            case 1:
                *(primearray+(byte/30))=*(primearray+(byte/30))+mask;
            break;

            case 7:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 1);
            break;

            case 11:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 2);
            break;

            case 13:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 3);
            break;

            case 17:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 4);
            break;

            case 19:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 5);
            break;

            case 23:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 6);
            break;

            case 29:
                *(primearray+(byte/30))=*(primearray+(byte/30))+(mask << 7);
            break;

            default:
                #ifdef DEBUG
                    printf("Error %lld\n",byte);
                #endif
            break;
        }
    }while ( ( filesize < MAXSIZE ) && ( byte != prev ) && ( byte != 0 ) );

#ifdef WRITE
    FILE *output;
    output=fopen("result.block","wb+");
    fwrite(primearray,1,filesize,output);
    fclose(output);
#endif

    return(0);
}
#endif