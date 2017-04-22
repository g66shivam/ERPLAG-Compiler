// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef symbolTableDef
#define symbolTableDef
#include <stdio.h>
#include <stdlib.h>
#include "typeChecker.h"

int dec[1010];// tells if a function with hash value x is declared or not
int def[1010];// tells if a function with hash value x is defined or not
int var_chk[1010];
int already_scoped;
int sno;
int current_module;
char cur_scope_name[20];
typedef struct idlist
{
	int sc;
	int type;
	int subtype;
	int ub;// for arrays
	int h_val;
	int mark;
	struct idlist* next;
}idlist;
idlist *in_list[1010];
idlist *out_list[1010];

typedef struct lazy
{
	int scope;
	node* in;
	node* out;
	int x;// hash value with whom to do paramaeter testing 
}lazy;

char error_log[500][150];
int err_cnt;
lazy todo[120];
int lzyptr;
pi blanknode;
int global_scope_ctr;// init to 0
int c_scope;
char hash[1050][12];// 8 letters long only
int parent[110];// parent scope
pi scope[110][1050];// 110 scopes hashed over 1050 hash space
int scope_map[1010];
int cur_offset[1010];
int start[1010];
int end[1010];
char ret[20];
int ma_offset;
int global_for_test;

#endif