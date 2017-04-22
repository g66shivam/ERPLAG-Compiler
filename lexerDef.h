// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef lexerDef
#define lexerDef
#include <stdio.h>
#include <stdlib.h>

struct Symbol_Table{
    char tokenName[50];
    char tokenVal[100];
    struct Symbol_Table *Next;
};

struct Token{
	char tokenName[50];
    struct Symbol_Table *tokenVal;
    int rowNum;
    struct Token *Next;	
};

extern int error;
extern char buffer[100];
extern char cur_string[10000];
extern int state,g_start,row_number;
extern struct Symbol_Table *symbolTable_head;
extern struct Symbol_Table *symbolTable_tail;
extern struct Token *tokenList_head;
extern struct Token *tokenList_tail;

#endif