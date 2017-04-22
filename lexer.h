// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef lexer
#define lexer
#include <stdio.h>
#include <stdlib.h>
#include "lexerDef.h"

extern struct Symbol_Table* getMem_ST(char* tokenName,char* tokenVal);
extern void insert_ST(char* tokenName,char* tokenVal);
extern void search_ST(char *str,char *str1);
extern void read_symbolTable();
extern void print();
extern void insert_Token(char* tokenName,struct Symbol_Table* tokenVal);
extern void getNextToken();
extern void getStream(FILE *fp);
extern void printT();
extern void DFAcomments(FILE *f);
extern void removeComments(FILE *fp);

#endif
