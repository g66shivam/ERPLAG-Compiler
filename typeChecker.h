// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef typeChecker
#define typeChecker
#include <stdio.h>
#include <stdlib.h>
#include "typeCheckerDef.h"

extern int check_op(char str[50]);
extern int check_var(char str[50]);
extern int assign_type(char str[50]);
extern int is_imp(char str[50]);
extern void delete_node(node* head);
extern node* make_ast(node* head);
extern void type_checker(node* head);
extern void semantic_analyzer(node *head);

#endif