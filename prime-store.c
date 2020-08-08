#if defined CINC || defined MAIN
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

    int encode(char *outfile, u_int8_t *bitpos, u_int8_t *primearray);
    int decode(char *outfile, u_int8_t *bitpos, u_int8_t *primearray);
#endif

#ifdef MAIN
    int main(int argc, char *argv[])
    {
       
        u_int8_t bitpos[8] = {1,7,11,13,17,19,23,29};
        u_int8_t *primearray;
        
        #ifdef ENCODE
            encode(argv[1],&bitpos[0],primearray);
        #elif DECODE
            decode(argv[1],&bitpos[0],primearray);
        #endif

        return 0;
    }
#endif

#ifdef ENCODE
    int encode(char *outfile, u_int8_t *bitpos, u_int8_t *primearray)
    {
        u_int64_t byte=0;
        u_int8_t mask=1;
        u_int8_t prev=0;
        u_int64_t filesize=0;
        primearray=calloc(MAXSIZE,1);
        int i=0;

        do
        {
        prev=byte;
        scanf("%llu\n",byte);

        #ifdef INFO
            printf("%llu\n",*(byte));
        #endif
    
            byte=byte-((byte/(u_int64_t)30)*(u_int64_t)30);

            for(i=0; i < 8; i++)
            {   
                if ( *(bitpos+i) == byte)
                {
                    *(primearray+filesize)=*(primearray+filesize)+(mask<<*(bitpos+i));
                    mask=1;
                    i=8;
                }
            }
            filesize++;
        }while ( ( filesize < MAXSIZE ) && ( byte != prev ) && ( byte != 0 ) );

    #ifdef WRITE
        FILE *output;
        output=fopen(outfile,"wb+");
        fwrite(primearray,1,filesize,output);
        fclose(output);
    #endif

        return(0);
    }
#endif

#ifdef DECODE
    int decode(char *outfile, u_int8_t *bitpos, u_int8_t *primearray)
    {
        FILE *input;
        u_int64_t byte=1;
        u_int64_t filesize=0;
        u_int64_t position=0;
        char infile[65535];
        int i=0;

        if ( ( input=fopen(infile,"rb+")) == NULL ) exit(-1);
        fseek(input,0L,SEEK_END);
        filesize=ftell(input);
        fseek(input,0L,SEEK_SET);

    #ifdef WRITE
        FILE *output;
        output=fopen(outfile,"wb+");
    #endif

        if ( filesize < MAXSIZE )
        {
            primearray=calloc(filesize,1);
            fread(primearray,1,filesize,input);
        }else{
            primearray=calloc(MAXSIZE,1);
        }

        do{
            for(i=0; i < 8; i++)
            {
                byte=(*(bitpos+i)+position*30);
                #ifdef DISPLAY
                    printf("%llu\n",byte);
                #elif WRITE
                    fwrite(byte,1,1,output);
                #endif
            }
            position++;
        }while ( ( position < filesize ) && ( position < MAXSIZE ) );
        
    #ifdef WRITE
        fclose(output);
    #endif

        return(0);
    }
#endif
