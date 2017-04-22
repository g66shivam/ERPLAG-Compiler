// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef typeCheckerDef
#define typeCheckerDef
#include <stdio.h>
#include <stdlib.h>
#include "parser.h"

#define MOD 1009
#define ARRAY 4
#define epsilon 101
#define ARITHMETICOP 1
#define LOGICALOP 2
#define RELATIONALOP 3
#define ASSIGNOP 4
#define NEGATION 5
#define INTEGER 1
#define REAL 2
#define BOOLEAN 3
#define ERROR 5
#define EXPR 325

node* type_checker_st[1000];
char bracket_st[100];
extern int tc_ptr;
extern int sqb_flag;
extern int semantic_error;

#endif