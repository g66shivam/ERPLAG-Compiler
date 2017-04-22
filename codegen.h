// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#ifndef codegen
#define codegen
#include <stdio.h>
#include <stdlib.h>
#include "typeChecker.h"

extern void code_generator_assignment(node* head);
extern void code_generator_conditional(node* head);
extern void code_generator_iterative(node* head);
extern void code_generator_print(node *head);
extern void code_generator_input(node* head);
extern void code_generator(node* head);

#endif