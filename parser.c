// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#define MOD 1009
#define START 263
#define DOLLAR 36
#define epsilon 101

node* parseTree_head = NULL;

node* getMem_PT(int hash_val,int is_non_term,node* par)
{
	node* temp = (node*)malloc(sizeof(node));
	temp->hash_val = hash_val;
	temp->is_non_term = is_non_term;
	temp->par = par;
	temp->child  = NULL;
	temp->next = NULL;
	temp->ptr = NULL;
	temp->sym_ptr = NULL;
	return temp;
}

void push_stk(node* parent,int rule_idx)
{
	int first=0;
	node* prev = NULL;
	node* arr[15];
	atom* trav = list[rule_idx]->next;
	int arrptr = 0;
	
	if(trav->hash_val==epsilon)
	{
		parent->child = getMem_PT(trav->hash_val,trav->is_non_term,parent);
		parent->child->ptr = (struct Token*)malloc(sizeof(struct Token));
		strcpy(parent->child->ptr->tokenName,"epsilon");
		(parent->child)->par = parent;	
	}

	while(trav!=NULL && trav->hash_val!=epsilon)
	{
		arr[arrptr] = getMem_PT(trav->hash_val,trav->is_non_term,parent);
		if(first==0)
		{
			parent->child = arr[arrptr];
			first = 1;
		}
		if(prev!=NULL)
			prev->next = arr[arrptr];	
		prev = arr[arrptr];
		arrptr++;
		trav = trav->next;
	}
	arrptr--;
	while(arrptr > -1)
	{
		mystack[stktop] = arr[arrptr];
		arrptr--;
		stktop++;
	}
}

void pop_stk()
{
	stktop--;
}

int append_fir(int i,int j)// tell if added something new and add it
{
	if(j == 101)
		hasepsilon[i] = 1;
	int temp;
	for(temp=0;First[i][temp]!=-1;temp++)
	{
		//glocnt++;
		if(First[i][temp]==j)
			return 0;
	}
	First[i][temp] = j;
	return 1;
}
void find_first()
{
	int change = 0;	int i;int k;
	memset(First,-1,sizeof First);
	do
	{
		change = 0;
		for(i = 0;i<=rule_no;i++)
		{	
			atom *cur = list[i];
			atom *trav = cur->next;
			atom *prev = list[i];
			do
			{
				if(trav->is_non_term == 0)
				{
					change|=append_fir(cur->hash_val,trav->hash_val);
					break;
				}
				int tmp = trav->hash_val;
				for(k = 0;First[tmp][k]!=-1;k++)
				{
					if(First[tmp][k] == 101)
						continue;
					int get = append_fir(cur->hash_val,First[tmp][k]);
					change |= get;
				}
				if(hasepsilon[trav->hash_val])
				{
					prev = trav;
					trav = trav->next;
				}
				else
					break;
			}while(trav!=NULL);
			if(trav == NULL && hasepsilon[prev->hash_val] == 1)
				append_fir(cur->hash_val,101);
		}
	}while(change!=0);
}

int append_fol(int i,int j)// tell if added something new and add it
{
	int temp;
	for(temp=0;Follow[i][temp]!=-1;temp++)
	{
		//glocnt++;
		if(Follow[i][temp]==j)
			return 0;
	}
	Follow[i][temp] = j;
	return 1;
}

void find_follow()
{
	int i,change,k;
	memset(Follow,-1,sizeof Follow);
	append_fol(START,DOLLAR);//
	do
	{
		change = 0;
		for (i = 0; i <= rule_no; ++i)
		{
			atom *cur = list[i];
			atom *trav = cur->next;
			while(trav!=NULL)
			{
				if(trav->is_non_term == 0)
				{
					trav = trav->next;
				}
				else
				{
					atom* tnext = trav->next;
					while(1)
					{
						if(tnext == NULL)
						{
							for(k = 0;Follow[cur->hash_val][k]!=-1;k++)
							{
								change |= append_fol(trav->hash_val,Follow[cur->hash_val][k]);
							}
							break;
						}
						else if(tnext->is_non_term==0)
						{
							change |= append_fol(trav->hash_val,tnext->hash_val);
							break;
						}
						else
						{
							int got_epsilon = 0;							
							for(k = 0;First[tnext->hash_val][k]!=-1;k++)
							{
								if(First[tnext->hash_val][k] != epsilon)
								change |= append_fol(trav->hash_val,First[tnext->hash_val][k]);
								else
								got_epsilon = 1;
							}
							if(!got_epsilon)
								break;
						}
						tnext = tnext->next;
					 }
					 trav = trav->next;
				}
			}
		}
	}while(change!=0);
}
// combined hash for term and non term
int get_hash_value(int nonterm, char* in)
{
	int ret = 0;
	int l = strlen(in);
	int i;
	if (nonterm)
		i = 1;
	else
		i = 0;
	for(;i < l ;i++)
	{
		ret = (ret*97+in[i])%MOD;
	}
	return ret;
}
int put_hash(int nonterm, int init, char* in)
{
	int ret = -10;
	int add = 1;
	int iter = 0;
	while(1)
	{
		if(nonterm)
		{
			iter++;
			if(iter == 10000)
				return -1;
			if(hash_nonterm[init][0] == '\0')
			{
				strcpy(hash_nonterm[init], in);
				ret = init;
				break;
			}
			else
			if(strcmp(hash_nonterm[init],in) == 0)
			{
				ret = init;
				break;
			}
			else
			{
				init=(init+add*add)%MOD;
				add++;
			}
		}
		else
		{
			iter++;
			if(iter == 10000)
				return -1;// error hai;
			if(hash_term[init][0] == '\0')
			{
				strcpy(hash_term[init], in);
				ret = init;
				break;
			}
			else
			if(strcmp(hash_term[init],in) == 0)
			{
				ret = init;
				break;
			}
			else
			{
				init=(init+add*add)%MOD;
				add++;
			}
		}
    }
	return ret;
}
void insert(int pos,int nonterm, int hashval)
{
	atom* newatom = (atom* )malloc(sizeof(atom));
	newatom->is_non_term = nonterm;
	newatom->hash_val = hashval;
	newatom->next = NULL;
	if(list[pos] == NULL)
	{
		list[pos] = tail[pos] = newatom;
	}
	else
	{
		tail[pos]->next = newatom;
		tail[pos] = tail[pos]->next;
	}
}
void ComputeFirstAndFollowSets()
{
	find_first();
	find_follow();
	/*int a = put_hash(1,get_hash_value(1,"<term"),"<term");
	int i;
	for(i=0;First[a][i]!=-1;i++)
		printf("%s ",hash_term[First[a][i]]);
	printf("\n");
	for(i=0;Follow[a][i]!=-1;i++)
		printf("%s ",hash_term[Follow[a][i]]);*/
}
void createParseTable()
{
	memset(parse_table,-1,sizeof parse_table);
	ComputeFirstAndFollowSets();
	int i,j;
	for(i = 0; i<= rule_no; i++)// iterate over rules in linked list
	{
		int lhs = list[i]->hash_val;
	    atom* trav = list[i]->next;
	    int is_eps = 0;
	    while(trav!=NULL)
	    {
	    	if(trav->hash_val == epsilon)
	    	{
	    		is_eps = 1;
	    		break;
	    	}
	    	else if(trav->is_non_term == 0)
	    	{
	    		parse_table[lhs][trav->hash_val] = i;
	    		break;
	    	}
	    	else 
	    	{
	    		int is_eps2 = 0;
	    		for(j = 0;First[trav->hash_val][j]!=-1;j++)
	    		{
	    			if(First[trav->hash_val][j] == epsilon)
	    			{
	    				is_eps2 = 1;
	    			}
	    			else
	    			{
	    				parse_table[lhs][First[trav->hash_val][j]] = i;
	    			}
	    		}
	    		if(is_eps2 == 0)
	    			break;
	    	}
	    	trav = trav->next;
	    }
	    if(trav == NULL)
	    	is_eps = 1;
	    if(is_eps)
	    {
	    	for(j = 0;Follow[lhs][j]!=-1;j++)
	    	{
	    		parse_table[lhs][Follow[lhs][j]] = i;
	    	}
	    }
	}
/*
	int fg[1010];
	for(i=0;i<1010;i++)
		fg[i] = 0;

	for(i = 0; i<= rule_no; i++)
	{
		int lhs = list[i]->hash_val;
		if(fg[lhs]==1)
			continue;
		fg[lhs] = 1;
		for(j=0;j<1010;j++)
			if(parse_table[lhs][j]!=-1)
				{printf("%s %s\n",hash_nonterm[lhs],hash_term[j]);}
	}*/
}

void print_lexeme(char* str)
{
	if(strcmp(str,"MUL")==0)
		printf("*\t");
	else if(strcmp(str,"GE")==0)
		printf(">=\t");
	else if(strcmp(str,"ENDDEF")==0)
		printf(">>\t");
	else if(strcmp(str,"GT")==0)
		printf(">\t");
	else if(strcmp(str,"DRIVERENDDEF")==0)
		printf(">>>\t");
	else if(strcmp(str,"LE")==0)
		printf("<=\t");
	else if(strcmp(str,"DEF")==0)
		printf("<<\t");
	else if(strcmp(str,"LT")==0)
		printf("<\t");
	else if(strcmp(str,"DRIVERDEF")==0)
		printf("<<<\t");
	else if(strcmp(str,"DIV")==0)
		printf("/\t");
	else if(strcmp(str,"MINUS")==0)
		printf("-\t");
	else if(strcmp(str,"PLUS")==0)
		printf("+\t");
	else if(strcmp(str,"EQ")==0)
		printf("==\t");
	else if(strcmp(str,"NE")==0)
		printf("!=\t");
	else if(strcmp(str,"ASSIGNOP")==0)
		printf(":=\t");
	else if(strcmp(str,"COLON")==0)
		printf(":\t");
	else if(strcmp(str,"RANGEOP")==0)
		printf("..\t");
	else if(strcmp(str,"SEMICOL")==0)
		printf(";\t");
	else if(strcmp(str,"COMMA")==0)
		printf(",\t");
	else if(strcmp(str,"SQBO")==0)
		printf("[\t");
	else if(strcmp(str,"SQBC")==0)
		printf("]\t");
	else if(strcmp(str,"BO")==0)
		printf("(\t");
	else if(strcmp(str,"BC")==0)
		printf(")\t");
	else
		printf("%s\t",str);
}

void parseInputSourceCode()
{
	int local_mark[1010];// error resolution
	int i;
	memset(local_mark,0,sizeof local_mark);
	struct Token* temp_dollar = (struct Token*)malloc(sizeof(struct Token));
	strcpy(temp_dollar->tokenName,"$");
	temp_dollar->tokenVal = NULL;
	temp_dollar->rowNum = -1;
	temp_dollar->Next = NULL;
	tokenList_tail->Next = temp_dollar;

	stktop = 0;
    parseTree_head = getMem_PT(START,1,NULL);
	mystack[stktop] = parseTree_head;
	stktop++;
	node* store_head;

	struct Token* token_ptr = tokenList_head;
	while(stktop!=0 && token_ptr!=NULL)
	{
		int hval = mystack[stktop-1]->hash_val;
		if (mystack[stktop-1]->is_non_term == 0)
		{
			if(strcmp(hash_term[hval],token_ptr->tokenName) == 0)
			{
				mystack[stktop-1]->ptr = token_ptr;
				pop_stk();
				token_ptr = token_ptr->Next;
			}
			else
			{
				error = 1;
				printf("ERROR_5: The token %s for lexeme ",token_ptr->tokenName);
				if(token_ptr->tokenVal == NULL)
					print_lexeme(token_ptr->tokenName);
				else
					printf("%s ",(token_ptr->tokenVal)->tokenVal);
				printf("does not match at line %d. The expected token here is %s\n",token_ptr->rowNum,hash_term[hval]);
				pop_stk();
			}
		}
		else
		{
			int hash_of_name = get_hash_value(0,token_ptr->tokenName);
			hash_of_name = put_hash(0,hash_of_name,token_ptr->tokenName);
			int rule_idx = parse_table[hval][hash_of_name];
			if (rule_idx == -1)
			{	
				error = 1;
				char expected[40];
				strcpy(expected,hash_term[First[hval][0]]);
				while(token_ptr!=NULL && parse_table[hval][put_hash(0,get_hash_value(0,token_ptr->tokenName),token_ptr->tokenName)] == -1)
				{
					if(strcmp(token_ptr->tokenName,"$")==0)
					{
						token_ptr = token_ptr->Next;
						continue;
					}	
					printf("ERROR_5: The token %s for lexeme ",token_ptr->tokenName);
					if(token_ptr->tokenVal == NULL)
						print_lexeme(token_ptr->tokenName);
					else
						printf("%s ",(token_ptr->tokenVal)->tokenVal);
					printf("does not match at line %d. The expected token here is %s\n",token_ptr->rowNum,expected);
					token_ptr = token_ptr->Next;
				}
				continue;
			}// skip over linked list tilL ele in first or follow on nonterm
			else
			{
				store_head = mystack[stktop-1];
				pop_stk();
				push_stk(store_head,rule_idx);
			}
		}
	}
	while(stktop!=0)
		pop_stk();

	tokenList_tail->Next = NULL;
	free(temp_dollar);
}

void printParseTree(node* head,int x,int flag)
{
	if(head==NULL)
		return;
	if(x == 1)
	++nodes_par;// parse
	else
	if(x == 2)
	++nodes_ast;
	printParseTree(head->child,x,flag);
	//PRINTING THE NODE
	//lexemeCurrentNode     lineno   token   valueIfNumber   parentNodeSymbol   isLeafNode(yes/no)    NodeSymbol
	if(flag == 1)
	{
		if(head->is_non_term==0)
		{
			if(head->hash_val==epsilon)
			{
				//LEXEME
				printf("----\t");

				//LINENO
				printf("----\t");

				//TOKEN
				printf("----\t");

				//valueIfNumber
				printf("----\t");

				//parentNodeSymbol
				if(head->par==NULL)
					printf("ROOT\t");
				else
					printf("%s\t",hash_nonterm[(head->par)->hash_val]+1);

				//isLeafNode
				printf("yes\t");

				//NodeSymbol
				printf("%s\t",hash_term[head->hash_val]);
			}
			else
			{
				//LEXEME
				if((head->ptr)->tokenVal==NULL)
					print_lexeme((head->ptr)->tokenName);
				else
					printf("%s\t",((head->ptr)->tokenVal)->tokenVal);

				//LINENO
				printf("%d\t",(head->ptr)->rowNum);

				//TOKEN
				printf("%s\t",(head->ptr)->tokenName);

				//valueIfNumber
				if(strcmp((head->ptr)->tokenName,"NUM")==0 || strcmp((head->ptr)->tokenName,"RNUM")==0)
					printf("%s\t",((head->ptr)->tokenVal)->tokenVal);
				else
					printf("----\t");

				//parentNodeSymbol
				if(head->par==NULL)
					printf("ROOT\t");
				else
					printf("%s\t",hash_nonterm[(head->par)->hash_val]+1);

				//isLeafNode
				printf("yes\t");

				//NodeSymbol
				printf("----\t");
			}
		}
		else
		{
			//LEXEME
			printf("----\t");

			//LINENO
			printf("----\t");

			//TOKEN
			printf("----\t");

			//valueIfNumber
			printf("----\t");

			//parentNodeSymbol
			if(head->par==NULL)
				printf("ROOT\t");
			else
				printf("%s\t",hash_nonterm[(head->par)->hash_val]+1);

			//isLeafNode
			printf("no\t");

			//NodeSymbol
			printf("%s\t",hash_nonterm[head->hash_val]+1);
		}
		printf("\n");
	}
	//PRINTING SIBLINGS
	if(head->child!=NULL)
	{
		node* siblings = (head->child)->next;
		while(siblings!=NULL)
		{
			printParseTree(siblings,x,flag);
			siblings = siblings->next;
		}
	}  
}

void Read_Grammar()
{
	FILE* fp = fopen("Grammar.txt","r");
	char *line = NULL;
	size_t len = 0;
	rule_no = -1;
	int i,j;
	while(getline(&line,&len,fp)!=-1)
	{
		int l = strlen(line);
		char nt[50];
		memset(nt,'\0',sizeof nt);
		int ptr = 0;
		int tptr = 0;
		int gptr = 0;
		while(ptr < l)
		{
			tptr = 0;
			while(ptr < l && line[ptr]!='-' && line[ptr]!=' ' && line[ptr]!='|' && line[ptr]!='\t' && line[ptr]!='\n')
			{
				nt[tptr] = line[ptr];
				ptr++;tptr++;
			}
			if(tptr!=0 || line[ptr] == '|')
			{
				if(line[ptr] == '-')
				{
					rule_no++;
					gptr = 0;
				}
				for(i = 0; i < tptr ;i++)
						gram[rule_no][gptr++] = nt[i];
					if(tptr)
						gram[rule_no][gptr++] = '$';
				if(line[ptr] == '|')
				{
					int nxtrul = rule_no+1;
					int newptr = 0;
					while(gram[rule_no][newptr]!='$')
					{
						gram[nxtrul][newptr] = gram[rule_no][newptr];
						newptr++;
					}
					gram[nxtrul][newptr++] = '$';
					gptr = newptr;
					rule_no = nxtrul;
				}
			}
			ptr++;
		}
	}
		FILE* fp1 = fopen("grammmar.txt","write");
		for(i = 0;i<=rule_no;i++)
		{
			fprintf(fp1,"%s\n", gram[i]);
		}
		fclose(fp1);
	
	char dol[5] = "$"; 
	for(i = 0;i <= rule_no ;i++)
	{
		char unit[50];
		int l = strlen(gram[i]);
		int ptr = 0;
		int first = 1;
		while(ptr < l)
		{
			int uptr = 0;
			int nonterm = 1;
			while(gram[i][ptr]!='$')
			{
				unit[uptr] = gram[i][ptr];
				uptr++;ptr++;
			}
			if(unit[uptr-1] != '>')
				nonterm = 0;
			else
				unit[uptr-1] = '\0';
			unit[uptr] = '\0';
			ptr++;
			int initval = get_hash_value(nonterm,unit);
			int hashval = put_hash(nonterm , initval, unit);
			if(hashval == -1)
			{
				puts("BAD HASH!");
				return;
			}
			insert(i,nonterm,hashval);
		}
	}
	put_hash(0,get_hash_value(0,dol),dol);
	fclose(fp);
}