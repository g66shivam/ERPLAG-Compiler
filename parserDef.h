// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef parserDef
#define parserDef
#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"
#define MAXSZ 10010

char gram[105][150]; // seperator is $
char hash_term[1020][40];// 40 =  max len of string // 1009 is prime;
char hash_nonterm[1020][40];
int hasepsilon[1020];
int parse_table[1010][1010];
int nodes_par;
int nodes_ast;
typedef struct atom
{
	int is_non_term;
	int hash_val;
	struct atom * next;
}atom;

typedef struct pi 
{ int sc;
  int type;
  int subtype;
  int lb; 
  int ub; 
  int offset; 
  int width;
  int dec;
  char module_name[10];
}pi;

typedef struct node
{
	struct node* par;
	struct node* child;
	struct node* next;
	int is_non_term;
	struct Token* ptr;
	pi* sym_ptr;
	int hash_val;
}node;

node* mystack[MAXSZ];
int stktop;
extern node* parseTree_head;

atom* list[100];
atom* tail[100];
int rule_no;
int First[1010][100];
int Follow[1010][100];


#endif
