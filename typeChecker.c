// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "typeChecker.h"

int tc_ptr = -1;
int sqb_flag = -1;
int semantic_error = 0;

int check_op(char str[50])
{
	if(strcmp(str,"NEGATE")==0)
		return NEGATION;
	else if(strcmp(str,"PLUS")==0 || strcmp(str,"MINUS")==0 || strcmp(str,"MUL")==0 || strcmp(str,"DIV")==0)
		return ARITHMETICOP;
	else if(strcmp(str,"AND")==0 || strcmp(str,"OR")==0)
		return LOGICALOP;
	else if(strcmp(str,"LT")==0 || strcmp(str,"LE")==0 || strcmp(str,"GT")==0 || strcmp(str,"GE")==0 || strcmp(str,"EQ")==0 || strcmp(str,"NE")==0)
		return RELATIONALOP;
	else if(strcmp(str,"ASSIGNOP")==0)
		return ASSIGNOP;
	else
		return 0;
}

int check_var(char str[50])
{
	if(strcmp(str,"ID")==0)
		return 1;
	if(strcmp(str,"NUM")==0)
		return 1;
	if(strcmp(str,"RNUM")==0)
		return 1;
	if(strcmp(str,"TRUE")==0)
		return 1;
	if(strcmp(str,"FALSE")==0)
		return 1;
	return 0;
}

int assign_type(char str[50])
{
	if(strcmp(str,"NUM")==0)
		return INTEGER;
	if(strcmp(str,"RNUM")==0)
		return REAL;
	if(strcmp(str,"TRUE")==0)
		return BOOLEAN;
	if(strcmp(str,"FALSE")==0)
		return BOOLEAN;
	return ERROR;
}

int is_imp(char str[50])
{
	if(strcmp(str,"RANGEOP")==0)
		return 0;
	if(strcmp(str,"BO")==0)
		return 0;
	if(strcmp(str,"BC")==0)
		return 0;
	if(strcmp(str,"START")==0)
		return 0;
	if(strcmp(str,"END")==0)
		return 0;
	if(strcmp(str,"IN")==0)
		return 0;
	if(strcmp(str,"COLON")==0)
		return 0;
	if(strcmp(str,"SEMICOL")==0)
		return 0;
	if(strcmp(str,"BREAK")==0)
		return 0;
	if(strcmp(str,"COMMA")==0)
		return 0;
	//if(strcmp(str,"SQBO")==0)
	//	return 0;
	//if(strcmp(str,"SQBC")==0)
	//	return 0;
	//if(strcmp(str,"ASSIGNOP")==0)
	//	return 0;
	if(strcmp(str,"USE")==0)
		return 0;
	if(strcmp(str,"MODULE")==0)
		return 0;
	if(strcmp(str,"WITH")==0)
		return 0;
	if(strcmp(str,"PARAMETERS")==0)
		return 0;
	if(strcmp(str,"OF")==0)
		return 0;
	if(strcmp(str,"TAKES")==0)
		return 0;
	if(strcmp(str,"INPUT")==0)
		return 0;
	if(strcmp(str,"DEF")==0)
		return 0;
	if(strcmp(str,"ENDDEF")==0)
		return 0;
	if(strcmp(str,"DRIVERDEF")==0)
		return 0;
	if(strcmp(str,"DRIVER")==0)
		return 0;
	if(strcmp(str,"PROGRAM")==0)
		return 0;
	if(strcmp(str,"DRIVERENDDEF")==0)
		return 0;
	return 1;
}

void delete_node(node* head)
{
	if(head==NULL)
		return;
	if(head==head->par->child)
	{
		head->par->child = head->next;
		free(head);
		return;
	}
	node* temp = head->par->child;
	while(temp->next!=head)
		temp = temp->next;
	temp->next = head->next;
	free(head);
	return;
}

node* make_ast(node* head)
{
	if(head==NULL)
		return head;

	node* f;
	if(!head->is_non_term  && head->hash_val!=epsilon && !is_imp(head->ptr->tokenName))
	{
		f = head->next;
		//printf("Deleting %s\n",(head->ptr)->tokenName);
		delete_node(head);
		return f;
	}
	else if(!head->is_non_term && check_op(head->ptr->tokenName))
	{
		//printf("collapsing %s with %s\n",head->ptr->tokenName,hash_nonterm[head->par->hash_val]);
		f = head->next;
		node* temp = head->par;
		
		//for handling <expression> - > MINUS BO <arithmeticExpr> BC
		temp->ptr = head->ptr;
		if(strcmp(head->ptr->tokenName,"MINUS")==0 && head->par->hash_val==EXPR)
			strcpy(temp->ptr->tokenName,"NEGATE");
		
		temp->is_non_term = 0;
		temp->sym_ptr = head->sym_ptr;
		temp->hash_val = head->hash_val;
		delete_node(head);
		return f;
	}
	else if(head->child!=NULL && (head->child)->next==NULL) //only one child
	{
		//printf("Deleting %s\n",hash_nonterm[head->hash_val]+1	);
		node* f = head->child;
		if(head==head->par->child)
		{
			head->par->child = head->child;
			head->child->next = head->next;
			head->child->par = head->par;
			free(head);
		}
		else
		{
			node* temp = head->par->child;
			while(temp->next!=head)
				temp = temp->next;
			temp->next = head->child;
			head->child->par = head->par;
			head->child->next = head->next;
			free(head);
		}
		return make_ast(f);
	}

	node* temp =  head->child;
	while(temp!=NULL)
		temp = make_ast(temp);
	return head->next;
}


void type_checker(node* head)
{
	if(head==NULL)
		return;

	if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<moduleReuseStmt")==0) //type checking on function calls already done
		return;
	
	node *temp = head->child;
	while(temp!=NULL)
	{
		type_checker(temp);
		temp = temp->next;
	}
	int op; 
	//CURRENT NODE
	if(head->is_non_term==0)
	{
		if(strcmp(head->ptr->tokenName,"SQBO")==0)
			bracket_st[++sqb_flag] = '[';
		else if(strcmp(head->ptr->tokenName,"SQBC")==0)
			sqb_flag--;
		else if(sqb_flag!=-1)	// should not push id[index] in the stack
		{
			if(head->is_non_term==0 && strcmp(head->ptr->tokenName,"ID")==0)
			{
				if(head->sym_ptr->type!=INTEGER)
				{
					semantic_error = 1;
					printf("ERROR_19: DATATYPE of the indexing variable must be INTEGER: line no %d\n",head->ptr->rowNum);
				}
			}
			return;
		}
		else if(check_var(head->ptr->tokenName))
		{
			//printf("inserting %s %s %d of type ",head->ptr->tokenName,head->ptr->tokenVal->tokenVal,head->ptr->rowNum);
			//if(head->sym_ptr!=NULL)
			//	printf("%d",head->sym_ptr->type);
			type_checker_st[++tc_ptr] = head;
			//printf("\n");
			//printf("counter is %d\n",tc_ptr);
		}
		else if(check_op(head->ptr->tokenName)==NEGATION)
		{//shivam gupt
			int t1;
			//printf("found MINUS\n");
			//printf("did it\n");

			if(type_checker_st[tc_ptr--]->sym_ptr==NULL)
			{
				//printf("found NULL %s \n",type_checker_st[tc_ptr+1]->ptr->tokenName);
				t1 = assign_type(type_checker_st[tc_ptr+1]->ptr->tokenName);
			}
			else
			{
				//printf("Not NULL %s \n",type_checker_st[tc_ptr+1]->ptr->tokenName);
				t1 = type_checker_st[tc_ptr+1]->sym_ptr->type;
				if(t1==ARRAY)
					t1 = type_checker_st[tc_ptr+1]->sym_ptr->subtype;
			}
			
			node* temp = getMem_PT(0,0,NULL);
			temp->ptr = (struct Token*)malloc(sizeof(struct Token));
			temp->ptr->rowNum = type_checker_st[tc_ptr+1]->ptr->rowNum;
			temp->sym_ptr = (pi*)malloc(sizeof(pi));

			if(t1==INTEGER || t1==REAL)
			{
				temp->sym_ptr->type = t1;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type %d\n",t1);
			}
			else
			{
				semantic_error = 1;
				//printf("error in 1st\n");
				temp->sym_ptr->type = ERROR;
				type_checker_st[++tc_ptr] = temp;
			}
		}
		else if(op=check_op(head->ptr->tokenName))
		{
			int t1,t2;
			//printf("%d\n",op);
			//printf("again did it\n");
			
			if(type_checker_st[tc_ptr--]->sym_ptr==NULL)
			{
				t1 = assign_type(type_checker_st[tc_ptr+1]->ptr->tokenName);
				//printf("found NULL %s and type is %d\n",type_checker_st[tc_ptr+1]->ptr->tokenName,t1);
			}
			else
			{
				//printf("Not NULL %s \n",type_checker_st[tc_ptr+1]->ptr->tokenName);
				t1 = type_checker_st[tc_ptr+1]->sym_ptr->type;
				if(t1==ARRAY)
					t1 = type_checker_st[tc_ptr+1]->sym_ptr->subtype;
			}

			if(type_checker_st[tc_ptr--]->sym_ptr==NULL)
			{
				t2 = assign_type(type_checker_st[tc_ptr+1]->ptr->tokenName);
				//printf("found NULL %s and type is %d\n",type_checker_st[tc_ptr+1]->ptr->tokenName,t2);
			}
			else
			{
				//printf("Not NULL %s \n",type_checker_st[tc_ptr+1]->ptr->tokenName);
				t2 = type_checker_st[tc_ptr+1]->sym_ptr->type;
				if(t2==ARRAY)
					t2 = type_checker_st[tc_ptr+1]->sym_ptr->subtype;
			}

			node* temp = getMem_PT(0,0,NULL);
			temp->ptr = (struct Token*)malloc(sizeof(struct Token));
			temp->ptr->rowNum = type_checker_st[tc_ptr+2]->ptr->rowNum;
			temp->sym_ptr = (pi*)malloc(sizeof(pi));
			
			if(op==ASSIGNOP && t1==t2)
			{}
			else if(op==ASSIGNOP && t1!=t2)
				printf("ERROR_20 in assignment line number %d\n",head->ptr->rowNum);
			else if(t1==INTEGER && t2==INTEGER && op==ARITHMETICOP)
			{
				temp->sym_ptr->type = INTEGER;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type integer\n");
			}
			else if(t1==REAL && t2==REAL && op==ARITHMETICOP)
			{
				temp->sym_ptr->type = REAL;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type REAL\n");
			}
			else if(t1==INTEGER && t2==INTEGER && op==RELATIONALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else if(t1==REAL && t2==REAL && op==RELATIONALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else if(t1==BOOLEAN && t2==BOOLEAN && op==LOGICALOP)
			{
				temp->sym_ptr->type = BOOLEAN;
				type_checker_st[++tc_ptr] = temp;
				//printf("inserting new made node of type BOOLEAN\n");
			}
			else
			{
				semantic_error = 1;
				printf("ERROR_19: invalid operation : Line number %d\n",head->ptr->rowNum);
				temp->sym_ptr->type = ERROR;
				type_checker_st[++tc_ptr] = temp;
			}
		}
	}
	else if(head->par!=NULL && head->par->is_non_term==1 && strcmp(hash_nonterm[head->par->hash_val],"<iterativeStmt")==0 && strcmp(head->par->child->ptr->tokenName,"WHILE")==0)
	{
		if(head->par->child->next==head)
		{
			int t1;
			if(type_checker_st[tc_ptr]->sym_ptr==NULL)
				t1 = assign_type(type_checker_st[tc_ptr]->ptr->tokenName);
			else
			{
				t1 = type_checker_st[tc_ptr]->sym_ptr->type;
				if(t1==ARRAY)
					t1 = type_checker_st[tc_ptr]->sym_ptr->subtype;
				
			}
			if(t1!=BOOLEAN)
			{
				semantic_error = 1;
				printf("ERROR_21: WHILE expression must evaluate to BOOLEAN : Line No - %d\n",head->par->child->ptr->rowNum);
			}
		}
	}
}

//INORDER TRAVERSAL
void semantic_analyzer(node *head)
{
	if(head==NULL)
		return;
	if(head->is_non_term && strcmp(hash_nonterm[head->hash_val],"<condionalStmt")==0)
	{
		//printf("found switch.................\n");
		int t = (head->child->next)->sym_ptr->type;
		//int t = BOOLEAN;
		int line_no = (head->child->next)->ptr->rowNum;

		if(t==INTEGER)
		{
			if(head->child->next->next->next->is_non_term==0)
			{
				semantic_error = 1;
				printf("ERROR_11: Switch with INTEGER datatype should have default statement : line_no %d\n",line_no);
			}
			node* temp = head->child->next->next;
			while(!(temp->is_non_term==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
			{
				if(strcmp(temp->child->next->ptr->tokenName,"NUM")!=0)
				{
					semantic_error = 1;
					printf("ERROR_12: Switch with INTEGER datatype can only have NUM in CASE statement : line_no %d\n",temp->child->next->ptr->rowNum);
				}
				temp = temp->child->next->next->next;	
			}
		}
		else if(t==BOOLEAN)
		{
			if(head->child->next->next->next->is_non_term!=0)
			{
				semantic_error = 1;
				printf("ERROR_13: Switch with BOOLEAN datatype can not have default statement : line_no %d\n",line_no);
			}
			node* temp = head->child->next->next;
			while(!(temp->is_non_term==0 && strcmp(temp->ptr->tokenName,"epsilon")==0))
			{
				if(!(strcmp(temp->child->next->ptr->tokenName,"TRUE")==0 || strcmp(temp->child->next->ptr->tokenName,"FALSE")==0))
				{
					semantic_error = 1;
					printf("ERROR_14: Switch with BOOLEAN datatype can only have TRUE or FALSE in CASE statement : line_no %d\n",temp->child->next->ptr->rowNum);
				}
				temp = temp->child->next->next->next;
			}
		}
		else if(t==REAL)
		{
			semantic_error = 1;
			printf("ERROR_15: Switch ID can not have REAL datatype : line no- %d\n",line_no);
		}
		else
		{
			semantic_error = 1;
			printf("ERROR_16: Switch ID invalid datatype : line no- %d\n",line_no);
		}
		return;
	}
	//VISTING ITS CHILDREN
	node* temp = head->child;
	while(temp!=NULL)
	{
		semantic_analyzer(temp);
		temp = temp->next;
	}
}