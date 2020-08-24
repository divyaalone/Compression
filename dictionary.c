#include "dictionary.h"


//Create first 256 entries
void InitDictionary(dictionary *d) {
    int i = 0;
    d->dicCount = 0;
    while (i < MAX) {
        unsigned char* tmp = (unsigned char*)calloc(1, 1);
        *tmp = i;
        d->code[i].data = tmp;
        d->code[i].s = 1;
        d->dicCount++;
        i++;
    }
    i = MAX;
    while (i < MAXLZW) {
        d->code[i].data = NULL;
        d->code[i].s = 0;
        i++;
    }
}

void freeDic(dictionary *d){
	int i=0;
	while(i<MAXLZW){
		if(d->code[i].s)
			free(d->code[i].data);
		else
			break;
		i++;
	}
}

int isInDic(bindata s, dictionary *d) {
    int ndx = MAX;
    while ((d->code[ndx].data != NULL) && (ndx < MAXLZW)) {
        if ((d->code[ndx].s == s.s) && memcmp(s.data, d->code[ndx].data, d->code[ndx].s) == 0 ) {
            //printf("Code found at ndx=%d codesize=%d\n", ndx, s.s);
            return 1;
        }
        ndx++;
    }
    return 0;
}

int getCode(bindata s, dictionary* d) {
    int ndx = 0;
    while ((d->code[ndx].data != NULL) && (ndx < MAXLZW)) {
        if ((d->code[ndx].s == s.s) && memcmp(s.data, d->code[ndx].data, d->code[ndx].s) == 0 )
            return ndx;
        ndx++;
    }
    return 0;
}

void addToDic(bindata s, dictionary* d)
{
    if (d->dicCount >= MAXLZW)
        return;

    unsigned char *tmp = (unsigned char*)calloc(s.s, 1);
    memcpy(tmp, s.data, s.s);

    d->code[d->dicCount].data = tmp;
    d->code[d->dicCount].s = s.s;
    d->dicCount++;
}

bindata getFromDic(dictionary* d, short ndx)
{
    bindata b = { 0 };
    copybinarydata(&b, d->code[ndx]);
    return b;
}

void copybinarydata(bindata *P, bindata C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(C.s, 1);
    else
        P->data = (unsigned char*)realloc(P->data, C.s);
    memcpy(P->data, C.data, C.s);
    P->s = C.s;
}

void copydata(bindata *P, unsigned char C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(1, 1);
    else
        P->data = (unsigned char*)realloc(P->data, 1);
    memcpy(P->data, &C, 1);
    P->s = 1;
}

void adddata(bindata *P, unsigned char C)
{
    if(P->data==NULL)
        P->data = (unsigned char*)calloc(1, 1);
    else
        P->data = (unsigned char*)realloc(P->data, P->s + 1);
    memcpy((P->data + P->s), &C, 1);
    P->s++;
}

void addbinarydata(bindata *P, bindata C)
{
    if (P->data == NULL)
        P->data = (unsigned char*)calloc(C.s, 1);
    else
        P->data = (unsigned char*)realloc(P->data, P->s + C.s);
    memcpy((P->data + P->s), C.data, C.s);
    P->s = P->s + C.s;
}

void freebindata(bindata *P){
	if(P->s)
		free(P->data);
}
	

