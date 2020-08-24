#pragma once
#include <stdio.h>
#include<fcntl.h>
#include<unistd.h>
#include<string.h>
#include<sys/time.h>
#define MAX 256
#define MAXLZW 4096

//Error code definitions
typedef enum errorcode {
    SUCCESS=0,
    INPUTERROR,
    FILENOTFOUNDERROR,
    FILEWRITEFAILED,
    FILECORRUPTED,
    DISKFULLERROR,
}errorcode;

void printUsage();
int onError(errorcode error, char* filename);

errorcode  HuffmanC(char* infile, char* outfile);
errorcode  LZWC(char* infile, char* outfile);
errorcode  HuffmanD(char* infile, char* outfile);
errorcode  LZWD(char* infile, char* outfile);
float getratio(char* infile, char* outfile);
void printsize(char* file);
