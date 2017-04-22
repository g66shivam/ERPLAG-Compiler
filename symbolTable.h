// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef symbolTable
#define symbolTable
#include <stdio.h>
#include <stdlib.h>
#include "symbolTableDef.h"

extern int max(int a,int b);
extern int get_width(int x);
extern int get_type(char* str);
extern char* get_type_name(int x);
extern int get_hash_value_sym(char* in);
extern pi* get_for_FOR(int x);
extern int put_hash_sym(int init, char* in);
extern int hash_id(node* head,int typ);// returns -1 if error (redeclartion) else 0;
extern int hash_array(node* head,int typ,int lo, int hi);// returns -1 if error (redeclartion) else 0;
extern int hash_input_list(node* head,int hash_val);
extern int hash_output_list(node* head,int hash_val);// cant return arrays rest is same as above
extern int match_param(int x, node* head,int c_scope);
extern int match_retlist(int x, node* head,int c_scope); // note that relist cannot have arrays in decleration
extern void all_assigned(int idx, node* head);
extern void traverse(node* head);
extern void build_symbol_table(node* pt_head);
extern void print_symbol_table(node* head);
extern void deficit_errors();
extern void print_sym_table_errors();

#endif