// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

int num_relop = 0;
int num_switch = 0;
int pr_switch = 0;
int num_for = 0;
int num_while = 0;
int index_fg = 0;

void code_generator_assignment(node* head)
{
	if(head==NULL)
		return;
	node *temp = head->child;
	while(temp!=NULL)
	{
		code_generator_assignment(temp);
		temp = temp->next;
	}

	int op; 
	//CURRENT NODE
	if(head->is_non_term==0)
	{
		if(strcmp(head->ptr->tokenName,"ID")==0)
		{
			//printf("in ID\n");
			type_checker_st[++tc_ptr] = head;
			printf("MOV AX,word[MEM+%d]\n",head->sym_ptr->offset);
			printf("PUSH AX\n");
		}
		else if(strcmp(head->ptr->tokenName,"NUM")==0)
		{
			//printf("in NUM\n");
			type_checker_st[++tc_ptr] = head;
			printf("MOV AX,%s\n",head->ptr->tokenVal->tokenVal);
			printf("PUSH AX\n");	
		}
		else if(strcmp(head->ptr->tokenName,"TRUE")==0)
		{
			//printf("in NUM\n");
			type_checker_st[++tc_ptr] = head;
			printf("MOV AX,1\n");
			printf("PUSH AX\n");	
		}
		else if(strcmp(head->ptr->tokenName,"FALSE")==0)
		{
			//printf("in NUM\n");
			type_checker_st[++tc_ptr] = head;
			printf("MOV AX,0\n");
			printf("PUSH AX\n");	
		}
		else if(check_op(head->ptr->tokenName)==NEGATION)
		{
			//printf("in NEGATION\n");
			printf("POP AX\n");
			printf("IMUL -1\n");
			printf("PUSH AX\n");
		}
		else if(op=check_op(head->ptr->tokenName))
		{
			node* temp = getMem_PT(0,0,NULL);

			printf("POP BX\n");
			printf("POP AX\n");
			tc_ptr--;
			
			// checking for array
			if(type_checker_st[tc_ptr]->par!=NULL && type_checker_st[tc_ptr]->par->child!=type_checker_st[tc_ptr])
			{
				//printf("got in\n");
				node* tt = type_checker_st[tc_ptr]->par->child;
				while(tt->next!=type_checker_st[tc_ptr])
					tt = tt->next;
				if(tt->is_non_term==0 && strcmp(tt->ptr->tokenName,"SQBO")==0)
				{
					//printf("yes found\n");
					if(type_checker_st[tc_ptr]->sym_ptr==NULL)
					{
						int a = atoi(type_checker_st[tc_ptr]->ptr->tokenVal->tokenVal);
						printf("MOV word[arr],%d\n",2*a-2);
					}
					else
					{
						printf("MOV AX,word[MEM+%d]\n",type_checker_st[tc_ptr]->sym_ptr->offset);
						if(type_checker_st[tc_ptr]->sym_ptr->subtype==INTEGER)
						{
							printf("IMUL 2\n");
							printf("SUB AX,2\n");
						}
						else
							printf("SUB AX,1\n");
						printf("MOV word[arr],AX\n");
					}
					index_fg = 1;
					printf("POP AX\n");
					tc_ptr--;
				}
				else
					tc_ptr--;
			}

			if(op==ASSIGNOP)
			{
				if(type_checker_st[tc_ptr]->sym_ptr->type==BOOLEAN)
				{
					if(index_fg)
					{
						index_fg = 0;
						printf("MOV CX,word[arr]\n");
						printf("AND ECX,0x00001111\n");
						printf("MOV byte[MEM+%d+ECX],BL\n",type_checker_st[tc_ptr]->sym_ptr->offset);
					}
					else
						printf("MOV byte[MEM+%d],BL\n",type_checker_st[tc_ptr]->sym_ptr->offset);
				}
				else
				{
					if(index_fg)
					{
						index_fg = 0;
						printf("MOV CX,word[arr]\n");
						printf("AND ECX,0x00001111\n");
						printf("MOV word[MEM+%d+ECX],BX\n",type_checker_st[tc_ptr]->sym_ptr->offset);
					}
					else	
						printf("MOV word[MEM+%d],BX\n",type_checker_st[tc_ptr]->sym_ptr->offset);
				}
				tc_ptr--;
			}
			else if(strcmp(head->ptr->tokenName,"PLUS")==0)
			{
				printf("ADD AX,BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"MINUS")==0)
			{
				printf("SUB AX,BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"MUL")==0)
			{
				printf("IMUL BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"DIV")==0)
			{
				printf("IDIV BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"AND")==0)
			{
				printf("AND AX,BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;	
			}
			else if(strcmp(head->ptr->tokenName,"OR")==0)
			{
				printf("AND AX,BX\n");
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
			}
			else if(strcmp(head->ptr->tokenName,"LT")==0)
			{
				printf("CMP AX,BX\n");
				printf("JL TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
			else if(strcmp(head->ptr->tokenName,"LE")==0)
			{
				printf("CMP AX,BX\n");
				printf("JLE TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
			else if(strcmp(head->ptr->tokenName,"GT")==0)
			{
				printf("CMP AX,BX\n");
				printf("JG TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
			else if(strcmp(head->ptr->tokenName,"GE")==0)
			{
				printf("CMP AX,BX\n");
				printf("JGE TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
			else if(strcmp(head->ptr->tokenName,"EQ")==0)
			{
				printf("CMP AX,BX\n");
				printf("JE TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
			else if(strcmp(head->ptr->tokenName,"NQ")==0)
			{
				printf("CMP AX,BX\n");
				printf("JNE TRUE%d\n",num_relop);
				printf("MOV AX,0\n");
				printf("JMP FALSE%d\n",num_relop);
				printf("TRUE%d: MOV AX,1\n",num_relop);
				printf("FALSE%d:\n",num_relop);
				printf("PUSH AX\n");
				type_checker_st[tc_ptr] = temp;
				num_relop++;
			}
		}	
	}
}

void code_generator_conditional(node* head)
{
	pr_switch = 0;
	node* curr = head->child->next;
	node* temp;

	if(curr->sym_ptr==NULL)
		printf("null in switch Please check\n");
	else if(curr->sym_ptr->type==BOOLEAN)
	{
		printf("MOV CL,byte[MEM+%d]\n",curr->sym_ptr->offset);
		temp = curr->next;
		while(!(temp->is_non_term==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
		{
			if(strcmp((temp->child->next)->ptr->tokenName,"TRUE")==0)
				printf("CMP CL,1\n");
			else if(strcmp((temp->child->next)->ptr->tokenName,"FALSE")==0)
				printf("CMP CL,0\n");
			else
				printf("CMP CL,%s\n",(temp->child->next->ptr->tokenVal->tokenVal));
			printf("JNZ C%d%d\n",num_switch,pr_switch);
			code_generator(temp->child->next->next);
			printf("JMP SWITCH_EXIT%d\n",num_switch);
			printf("C%d%d:\n",num_switch,pr_switch);
			pr_switch++;
			temp = temp->child->next->next->next;
		}
		printf("SWITCH_EXIT%d:\n",num_switch);
	}
	else if(curr->sym_ptr->type==INTEGER)
	{
		printf("MOV CX,word[MEM+%d]\n",curr->sym_ptr->offset);
		temp = curr->next;
		while(!(temp->is_non_term==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
		{
			printf("CMP CX,%s\n",(temp->child->next->ptr->tokenVal->tokenVal));
			printf("JNZ C%d%d\n",num_switch,pr_switch);
			code_generator(temp->child->next->next);
			printf("JMP SWITCH_EXIT%d\n",num_switch);
			printf("C%d%d:\n",num_switch,pr_switch);
			pr_switch++;
			temp = temp->child->next->next->next;
		}
		//default statement
		code_generator(curr->next->next->child->next);
		printf("SWITCH_EXIT%d:\n",num_switch);
	}
	else
		printf("ERROR_22: invalid input in code_generator_conditional\n");
	num_switch++;
}	

void code_generator_iterative(node* head)
{
	if(strcmp(head->child->ptr->tokenName,"FOR")==0)
	{
		num_for++;
		node* curr = head->child->next;
		node* num1 = curr->next->child;
		node* num2 = num1->next;
		
		printf("MOV CX,%s\n",num1->ptr->tokenVal->tokenVal);
		printf("MOV word[MEM+%d],CX\n",curr->sym_ptr->offset);
		printf("FOR_LOOP%d:\n",num_for);
		code_generator(curr->next->next);
		printf("MOV CX,word[MEM+%d]\n",curr->sym_ptr->offset);
		printf("INC CX\n");
		printf("MOV word[MEM+%d],CX\n",curr->sym_ptr->offset);
		printf("MOV AX,%s\n",num2->ptr->tokenVal->tokenVal);
		printf("CMP CX,AX\n");
		printf("JLE FOR_LOOP%d\n",num_for);
	}
	else if(strcmp(head->child->ptr->tokenName,"WHILE")==0)
	{
		num_while++;
		code_generator(head->child->next);
		printf("POP AX\n");
		printf("CMP AL,1\n");
		printf("JNZ WHILE_END%d\n",num_while);
		printf("WHILE_LOOP%d:\n",num_while);
		code_generator(head->child->next->next);
		code_generator(head->child->next);
		printf("POP AX\n");
		printf("CMP AL,1\n");
		printf("JZ WHILE_LOOP%d\n",num_while);
		printf("WHILE_END%d:\n",num_while);
	}
}

void code_generator_print(node *head)
{
	printf("\n");
	printf("mov rax,0\n");
	printf("MOV rbx,0xFFFFFFFFFFFFFFFF\n");
	printf("cmovl rax,rbx\n");

	if(head->child->next->is_non_term==0)
	{
		if(strcmp(head->child->next->ptr->tokenName,"TRUE")==0)
			printf("mov AX,1\n");		
		else if(strcmp(head->child->next->ptr->tokenName,"FALSE")==0)
			printf("mov AX,0\n");
		else
			printf("mov AX,%s\n",head->child->next->ptr->tokenVal->tokenVal);
	}
	else if(head->child->next->child->next->is_non_term==0 && strcmp(head->child->next->child->next->ptr->tokenName,"epsilon")==0)
		printf("mov AX,word[MEM+%d]\n",head->child->next->child->sym_ptr->offset);
	else
	{
		node* curr = head->child->next->child;
		node* temp = curr->next->child->next;
		if(temp->sym_ptr==NULL)
		{
			int a = atoi(temp->ptr->tokenVal->tokenVal);
			printf("MOV word[arr],%d\n",a=a*2-2);
		}
		else
		{
			printf("MOV AX,word[MEM+%d]\n",temp->sym_ptr->offset);
			if(curr->sym_ptr->subtype==INTEGER)
			{
				printf("IMUL 2\n");
				printf("SUB AX,2\n");
			}
			else
				printf("SUB AX,1\n");	
			printf("MOV word[arr],AX\n");
		}
		printf("MOV CX,word[arr]\n");
		printf("AND ECX,0x00001111\n");
		printf("mov AX,word[MEM+%d+ECX]\n",curr->sym_ptr->offset);
	}

	printf("push    rax\n");
	printf("  push    rcx\n");
	printf("\tmov     rdi, men\n");
	printf("\tmov     rsi, rax\n");
	printf("\txor     rax, rax\n");
	printf("\tcall    printf\n");
	printf("  pop     rcx\n");
	printf("pop     rax\n");
}

void code_generator_input(node* head)
{
	node* curr = head->child->next;
	printf("\n");
	printf("MOV RSI,var+0\n");
	printf("MOV RDI,inputFormat\n");
	printf("XOR RAX,RAX\n");
	printf("call scanf\n");
	printf("MOV AX,word[var+0]\n");
	
	if(curr->sym_ptr->type==INTEGER)
		printf("MOV word[MEM+%d],AX\n",curr->sym_ptr->offset);
	else
		printf("MOV byte[MEM+%d],AL\n",curr->sym_ptr->offset);
}

//DO THIS WHEN NO ERRORS
void code_generator(node* head)
{
	if(head==NULL)
		return;

	if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<assignmentStmt")==0)
	{
		tc_ptr = -1;
		code_generator_assignment(head);
		return;
	}
	else if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<condionalStmt")==0)
	{
		code_generator_conditional(head);
		return;
	}
	else if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<iterativeStmt")==0)
	{
		code_generator_iterative(head);
		return;
	}
	else if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<ioStmt")==0 && strcmp(head->child->ptr->tokenName,"PRINT")==0)
	{
		code_generator_print(head);
		return;
	}
	else if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<ioStmt")==0 && strcmp(head->child->ptr->tokenName,"GET_VALUE")==0)
	{
		code_generator_input(head);
		return;
	}

	node *temp = head->child;
	while(temp!=NULL)
	{
		code_generator(temp);
		temp = temp->next;
	}
}
