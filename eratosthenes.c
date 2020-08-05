/*algorithm Sieve of Eratosthenes is
    input: an integer n > 1.
    output: all prime numbers from 2 through n.
*/
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <string.h>

#define _GNU_SOURCE
#define MAP_HUGE_1GB (30 << MAP_HUGE_SHIFT)

// let A be an array of Boolean values, indexed by integers 2 to n,
// Max size of binary array in 16 byte aligned blocks
#define BLOCKSIZE 1600000 //1600000000 // 1600000000 ~1.6GB == The first 12.8 billion numbers
#define BLOCKROOT 1265 // 5-491 // √1600000000

// initially all set to true.
// for speed I'm going reverse true and false 
void primeIter(u_int32_t *primearray, u_int32_t *primepos, u_int64_t *array, u_int64_t *position, u_int8_t *bitpos, u_int64_t *multipos, u_int64_t *mask);

#ifdef MAIN
int main(int argc, char *argv[])
{
    FILE *input;
    u_int32_t *primearray;
    u_int32_t primepos;
    u_int64_t *array;
    u_int64_t position;
    u_int8_t bitpos;
    u_int64_t multipos=1;
    #ifdef SPEED
       u_int64_t *mask;
    #else
        u_int64_t mask;
    #endif

    u_int64_t i=0;
    u_int64_t fs=0;

    primearray=calloc(BLOCKROOT,4);
    array=calloc(BLOCKSIZE,8);

    if ( ( input=fopen(argv[1],"r+") ) == NULL) exit(-1);
    fseek(input,0L,SEEK_END);
    fs=ftell(input);
    fseek(input,0L,SEEK_SET);

    for(i=0; i < BLOCKROOT; i++)
    {
        multipos='\0';
        fscanf(input,"%llu\n",&multipos);
        //printf("%llu\n",multipos);
        *(primearray+i)=multipos;
    }
    multipos=1;

    #ifdef SPEED
        mask=calloc(64,8);
        for(i=0; i < 64; i++)
        {
            *(mask+i)=1;
            *(mask+i) <<= i;
        }

        primeIter(primearray, &primepos, array, &position, &bitpos, &multipos, mask);
        free(mask);
    #else
        primeIter(primearray, &primepos, array, &position, &bitpos, &multipos, &mask);
    #endif

#ifdef WRITE
    FILE *output;
    output=fopen("reesult.block","wb+");
    fwrite(array,8,BLOCKSIZE,output);
    fclose(output);
#endif

    free(primearray);
    free(array);

    return(0);
}
#endif

void primeIter(u_int32_t *primearray, u_int32_t *primepos, u_int64_t *array, u_int64_t *position, u_int8_t *bitpos, u_int64_t *multipos, u_int64_t *mask)
{
    //for i = 2, 3, 4, ..., not exceeding √n do
    if ( ( *(primepos) < BLOCKROOT ) && ( *(primearray+*(primepos)) < BLOCKROOT ) )
    {
        if (*(position) < BLOCKSIZE )
        {
            // if A[i] is true
            if ( ((*(array+*(position)) >> *(bitpos))%2) == 0 ) 
            {
                // for j = i2, i2+i, i2+2i, i2+3i, ..., not exceeding n do
                // A[j] := false
                /* https://graphics.stanford.edu/~seander/bithacks.html#ConditionalSetOrClearBitsWithoutBranching

                bool f;         // conditional flag
                unsigned int m; // the bit mask
                unsigned int w; // the word to modify:  if (f) w |= m; else w &= ~m; 

                w ^= (-f ^ w) & m;

                // OR, for superscalar CPUs:
                w = (w & ~m) | (-f & m);
                */
                if ( *(position)*8+*(bitpos) == *(primearray+*(primepos))*2+*(multipos)**(primearray+*(primepos)) )
                {
                    // Test this with a mask array first
                    #ifdef SPEED
                        *(array+*(position))=  ( *(array+*(position)) & ~*(mask+*(bitpos))) | ( -1 & *(mask+*(bitpos)) );
                    // Then without
                    #else
                        *(mask)=1;
                        *(mask) >>= *(bitpos); 
                        *(array+*(position))=*(array+*(position)) & ~*(mask) ) | ( -1 & *(mask));
                    #endif

                    
                    *(multipos)=*(multipos)+1;

                    if ( *(primearray+*(primepos))*2+*(multipos)**(primearray+*(primepos)) )
                    {
                        *(multipos)=1;
                        *(bitpos)=*(bitpos)+1;

                        if ( *(bitpos) == 63 )
                        {
                            *(bitpos)=0;
                            *(position)=*(position)+1;
                        }
                    }
                }
                primeIter(primearray, primepos, array, position,bitpos,multipos,mask);
            }        
        }else{
            *(position)=0;
            *(bitpos)=0;
            *(multipos)=1;
        
            *(primepos)=*(primepos)+1;
            primeIter(primearray, primepos, array, position, bitpos, multipos, mask);
        }
    }
}
//return all i such that A[i] is true.
