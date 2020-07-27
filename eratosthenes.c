/*https://en.wikipedia.org/wiki/Sieve_of_Eratosthenes

algorithm Sieve of Eratosthenes is
    input: an integer n > 1.
    output: all prime numbers from 2 through n.
*/

// let A be an array of Boolean values, indexed by integers 2 to n,
// Max size of binary array in 16 byte aligned blocks
#define BLOCKSIZE 1600000000 // 1600000000 ~1.6GB == The first 12.8 billion numbers
#define BLOCKROOT 126491 // √1600000000

// initially all set to true.
// for speed I'm going reverse true and false 

void primeIter(u_int32_t *primearray, u_int32_t *primepos, u_int64_t *array, u_int8_t *(bitpos));

#ifdef MAIN
int main(int argc, char *argv[])
{
    u_int32_t *primearray;
    u_int32_t primepos;
    u_int64_t *array;
    u_int8_t bitpos;
    #ifdef MEMORY
       u_int64_t *mask;
    #else
        u_int64_t mask;
    #endif

    primearray=calloc(126491,4);
    array=calloc(1600000000,8);

    #ifdef SPEED
        mask=calloc(mask,64);
        primeIter(primearray, &primepos, array, &bitpos, mask);
    #else
        primeIter(primearray, &primepos, array, &bitpos, &mask);
    #endif

    free(primarray);
    free(array);

    return(0);
}
#endif

void primeIter(u_int32_t *primearray, u_int32_t *primepos, u_int64_t *array, u_int8_t *bitpos, u_int64_t *mask)
{
    //for i = 2, 3, 4, ..., not exceeding √n do
    if ( ( *(primepos) < BLOCKROOT ) && ( *(primearray+*(primepos)) < BLOCKROOT ) )
    {
        if (*(position) < BLOCKSIZE )
        {
            // if A[i] is true
            if ( ((*(array+*(position)) >> bitpos)%2) == 0 ) 
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
                
                // Test this with a mask array first
                #ifdef SPEED
                    *(array+*(position))=  ( *(array+*(position)) & ~*(mask+*(bitpos))) | ( -1 & *(mask+*(bitpos)) );
                // Then without
                #else
                    *(mask)=1;
                    *(mask) >>= *(bitpos); 
                    *(array+*(position))=*(array+*(position)) & ~*(mask) ) | ( -1 & *(mask));
                #endif

                *(position)=*(position)+1;s
                primeIter(primearray, primepos, array, bitpos);
            }        
        }else{
            *(postion)=0;
        
            *(primepos)=*(primepos)+1;
            primeIter(primearray, primepos, array, bitpos);
        }
    }
}
//return all i such that A[i] is true.
