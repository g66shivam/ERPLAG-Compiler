// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef parser
#define parser
#include <stdio.h>
#include <stdlib.h>
#include "parserDef.h"

extern node* getMem_PT(int hash_val,int is_non_term,node* par);
extern void push_stk(node* parent,int rule_idx);
extern void pop_stk();
extern int append_fir(int i,int j);
extern void find_first();
extern int append_fol(int i,int j);
extern void find_follow();
extern int get_hash_value(int nonterm, char* in);
extern int put_hash(int nonterm, int init, char* in);
extern void insert(int pos,int nonterm, int hashval);
extern void ComputeFirstAndFollowSets();
extern void createParseTable();
extern void print_lexeme(char* str);
extern void parseInputSourceCode();
extern void printParseTree(node* head,int x,int flag);
extern void Read_Grammar();

#endif