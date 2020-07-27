// https://en.wikipedia.org/wiki/List_of_prime_numbers

//limit ← 1000000000        // arbitrary search limit
#define BLOCKSIZE 1600000000 // 1600000000 ~1.6GB == The first 12.8 billion numbers
#define BLOCKROOT 126491 // √1600000000

// set of wheel "hit" positions for a 2/3/5 wheel rolled twice as per the Atkin algorithm
//s ← {1,7,11,13,17,19,23,29,31,37,41,43,47,49,53,59}

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

    // Initialize the sieve with enough wheels to include limit:
    //for n ← 60 × w + x where w ∈ {0,1,...,limit ÷ 60}, x ∈ s:
    //    is_prime(n) ← false
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
    // Put in candidate primes:
    //   integers which have an odd number of
    //   representations by certain quadratic forms.
    // Algorithm step 3.1:
    for n ≤ limit, n ← 4x²+y² where x ∈ {1,2,...} and y ∈ {1,3,...} // all x's odd y's
        if n mod 60 ∈ {1,13,17,29,37,41,49,53}:
            is_prime(n) ← ¬is_prime(n)   // toggle state
    // Algorithm step 3.2:
    for n ≤ limit, n ← 3x²+y² where x ∈ {1,3,...} and y ∈ {2,4,...} // only odd x's
        if n mod 60 ∈ {7,19,31,43}:                                 // and even y's
            is_prime(n) ← ¬is_prime(n)   // toggle state
    // Algorithm step 3.3:
    for n ≤ limit, n ← 3x²-y² where x ∈ {2,3,...} and y ∈ {x-1,x-3,...,1} //all even/odd
        if n mod 60 ∈ {11,23,47,59}:                                   // odd/even combos
            is_prime(n) ← ¬is_prime(n)   // toggle state

    // Eliminate composites by sieving, only for those occurrences on the wheel:
    for n² ≤ limit, n ← 60 × w + x where w ∈ {0,1,...}, x ∈ s, n ≥ 7:
        if is_prime(n):
            // n is prime, omit multiples of its square; this is sufficient 
            // because square-free composites can't get on this list
            for c ≤ limit, c ← n² × (60 × w + x) where w ∈ {0,1,...}, x ∈ s:
                is_prime(c) ← false

    // one sweep to produce a sequential list of primes up to limit:
    output 2, 3, 5
    for 7 ≤ n ≤ limit, n ← 60 × w + x where w ∈ {0,1,...}, x ∈ s:
        if is_prime(n): output n

}
