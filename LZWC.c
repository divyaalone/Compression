//LZW compression
#include "common.h"
#include "dictionary.h"

unsigned char getlsbbyte(short code){
    short tmp = code & 0x00FF;
    return (unsigned char)tmp;
}

unsigned char getmsbbyte(short code) {
    short tmp = code & 0x0FF0;
    tmp = tmp >> 4;
    return (unsigned char)tmp;
}

unsigned char getmsbnibble(short code) {
    short tmp = code & 0x0F00;
    tmp = tmp >> 8;
    return (unsigned char)tmp;
}

unsigned char getlsbnibble(short code) {
    short tmp = code & 0x000F;
    return (unsigned char)tmp;
}

unsigned char createbytefromnibble(unsigned char highnibble, unsigned char lownibble)
{
    short tmp = highnibble;
    tmp = tmp << 4;
    tmp = tmp | lownibble;
    return (unsigned char)tmp;
}

void writebytedata(FILE* fpout, short code, int finish) {
    static int remainingbits = 0;
    static unsigned char remainingdata = 0;  //if some data is left out from previous op, then it would always be lsb nibble

    //we write MSB first and then LSB
    unsigned char c1 = getmsbbyte(code);
    unsigned char c2 = getlsbnibble(code);

    if (finish == 1) {
        //write remaining data if any
        if (remainingbits) {
            unsigned char tmp = createbytefromnibble(getlsbnibble(remainingdata), getmsbnibble(code));
            fwrite(&tmp, 1, 1, fpout);
        }
        return;
    }
    if (remainingbits == 0) {
        //write lsb as it is
        fwrite(&c1, 1, 1, fpout);
        remainingdata = c2;
        remainingbits = 4;
    }
    else {
        unsigned char tmp=createbytefromnibble(getlsbnibble(remainingdata), getmsbnibble(code));
        fwrite(&tmp, 1, 1, fpout);
        tmp = getlsbbyte(code);
        fwrite(&tmp, 1, 1, fpout);
        remainingdata = 0;
        remainingbits = 0;
    }

}

void encodeLZWData(FILE* fpin, FILE* fpout, dictionary *d){
    unsigned char C = '\0';
    bindata P = { 0 };

    //read data from file and create table dynamically
    if (fread(&C, 1, 1, fpin)) {
        //sprintf(P, "%c", C);
        adddata(&P, C);
        while (!feof(fpin)) {
            if (fread(&C, 1, 1, fpin)) {
                bindata temp = { 0 };
                //sprintf(temp, "%s%c", P, C);
                addbinarydata(&temp, P);
                adddata(&temp, C);
                if (isInDic(temp, d)) {
                    copybinarydata(&P, temp);
                    //strcpy(P, temp);
                }
                else {
                   // output the code for P
                    short code = getCode(P, d);
                    //write code to out file
                    //fwrite(&code, sizeof(short), 1, fpout);
                    //write 12 bits of code
                    writebytedata(fpout, code, 0);
                    addToDic(temp, d);
                    //sprintf(P, "%c", C);
                    copydata(&P, C);
                }
		         //free data
		        freebindata(&temp);
            }
        }
        int code = getCode(P, d);
        //write code to out file
        //fwrite(&code, sizeof(int), 1, fpout);
        writebytedata(fpout, code, 0);
        writebytedata(fpout, 0, 1);  //write remaining data if any
    }
    freebindata(&P);
}
//---------------------------------------------------------------------------------------
//this function takes the input file and compresses it usingthe lzw algorithm and saves the output file
errorcode LZWC(char* infile, char* outfile){
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    dictionary d1;
    FILE *fpin, *fpout;
    fpin = fopen(infile, "rb");
    if (fpin == NULL) return FILENOTFOUNDERROR;

    fpout = fopen(outfile, "wb");
    if (fpout == NULL) {
        fclose(fpin);
        return FILEWRITEFAILED;
    }
    //Init dictionary
    InitDictionary(&d1);

    //Build LZW code table
    encodeLZWData(fpin, fpout, &d1);
	
	freeDic(&d1);
    fclose(fpin);
    fclose(fpout);

	gettimeofday(&end, NULL);
	long seconds = (end.tv_sec - start.tv_sec);
	long micros = ((seconds * 1000000) + end.tv_usec) - (start.tv_usec);
	printf("Time elpased is %ld seconds and %ld micros\n", seconds, micros);
	    
    float ratio = getratio(infile, outfile);
    printf("Compression Ratio using LZW Compression Algorithm is %f\n", ratio);

    return SUCCESS;
}
