#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "common.h"

typedef struct bindata {
    unsigned char*   data;   //buffer
    short  s;       //size of data
}bindata;

typedef struct dictionary {
    bindata     code[MAXLZW];
    int         dicCount;               //number of code strings generated so far
}dictionary;

void InitDictionary(dictionary *d);
int isInDic(bindata s, dictionary* d);
int getCode(bindata s, dictionary* d);
void addToDic(bindata s, dictionary* d);
void adddata(bindata *P, unsigned char C);
void addbinarydata(bindata *P, bindata C);
void copybinarydata(bindata *P, bindata C);
void copydata(bindata *P, unsigned char C);
void freeDic(dictionary *d);
void freebindata(bindata *P);
bindata getFromDic(dictionary* d, short code);
