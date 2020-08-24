#include "common.h"

//calculate effeciency
float getratio(char* infile, char* outfile){
	float ratio=0;
	int size1=0, size2=0; 
	FILE* fp=fopen(infile, "r");
	fseek(fp, 0, SEEK_END);
    size1 = ftell(fp);
    fclose(fp);
    
    fp = fopen(outfile, "r");
	fseek(fp, 0, SEEK_END);
    size2 = ftell(fp);
    fclose(fp);
    
    if(size1 && size2){
    	ratio = (float)size1/(float)size2;
    }
    printf("Size of input file: %d bytes\nSize of compressed file: %d bytes\n",size1,size2);
    return ratio;
} 

void printsize(char* file){
	FILE * fp = fopen(file, "r");
	fseek(fp, 0, SEEK_END);
    int size = ftell(fp);
    fclose(fp);
   
    printf("Size of file '%s': %d bytes\n",file, size);
    return;
}
