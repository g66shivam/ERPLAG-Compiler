// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"
#include "parser.h"

char buffer[100];
char cur_string[10000];
int state,g_start,row_number;
struct Symbol_Table *symbolTable_head = NULL;
struct Symbol_Table *symbolTable_tail = NULL;
struct Token *tokenList_head = NULL;
struct Token *tokenList_tail = NULL;
int error = 0;

struct Symbol_Table* getMem_ST(char* tokenName,char* tokenVal)
{
    struct Symbol_Table* temp = (struct Symbol_Table*)malloc(sizeof(struct Symbol_Table));
    strcpy(temp->tokenName,tokenName);
    if(tokenVal!=NULL)
        strcpy(temp->tokenVal,tokenVal);
    else
        temp->tokenVal[0] = '\0';
    temp->Next = NULL;
    return temp;
}

void insert_ST(char* tokenName,char* tokenVal)
{
    struct Symbol_Table* temp = getMem_ST(tokenName,tokenVal);
    if(symbolTable_head==NULL)
    {
        symbolTable_head = temp;
        symbolTable_tail = temp;
    }
    else
    {
        symbolTable_tail->Next = temp;
        symbolTable_tail = temp;
    }
}

void search_ST(char *str,char *str1)
{
	struct Symbol_Table *head = symbolTable_head;
	while(head!=NULL)
	{
		if(strcmp(head->tokenVal,str)==0)
		{
			struct Token* temp = (struct Token*)malloc(sizeof(struct Token));
			strcpy(temp->tokenName,head->tokenName);
			temp->tokenVal = head;
			temp->rowNum = row_number;
			temp->Next = NULL;

			//inserting in the token list
			if(tokenList_head==NULL)
		    {
		        tokenList_head = temp;
		        tokenList_tail = temp;
		    }
		    else
		    {
		        tokenList_tail->Next = temp;
		        tokenList_tail = temp;
		    }
			return;
		}
		head = head->Next;
	}

	if(strcmp(str1,"ID")==0 && strlen(str)>8)
	{
		error = 1;
		printf("ERROR_1: Identifier at line %d is longer than the prescribed length\n",row_number);
		return;
	}

	struct Token* temp = (struct Token*)malloc(sizeof(struct Token));
	insert_ST(str1,str);
	strcpy(temp->tokenName,str1);
	temp->tokenVal = symbolTable_tail;
	temp->rowNum = row_number;
	temp->Next = NULL;

	//inserting in the token list
	if(tokenList_head==NULL)
	{
	    tokenList_head = temp;
	    tokenList_tail = temp;
	}
	else
	{
		tokenList_tail->Next = temp;
		tokenList_tail = temp;
	}
	return;
}

void read_symbolTable()
{
	char arr[100];
	char arr2[100];
	FILE *fp = fopen("keywords.txt","r");
	if(fp!=NULL){
		while(fscanf(fp,"%s %s",arr,arr2)==2)
			insert_ST(arr2,arr);
	}
	fclose(fp);
}

void print()
{
	struct Symbol_Table *head = symbolTable_head;
    while(head!=NULL)
    {
        printf("%s %s\n",head->tokenName,head->tokenVal);
        head = head->Next;
    }
}

void insert_Token(char* tokenName,struct Symbol_Table* tokenVal)
{
	struct Token* temp = (struct Token*)malloc(sizeof(struct Token));
	strcpy(temp->tokenName,tokenName);
	temp->tokenVal = tokenVal;
	temp->rowNum = row_number;
	temp->Next = NULL;

    if(tokenList_head==NULL)
    {
        tokenList_head = temp;
        tokenList_tail = temp;
    }
    else
    {
        tokenList_tail->Next = temp;
        tokenList_tail = temp;
    }
}

void getNextToken()
{
	int start = 0;
	int exec = 1;
	while(exec)
	{
		if(buffer[start]=='\0')
			{exec=0;continue;}
		switch(state)
		{
			case 0: g_start = 0;
					if((buffer[start]>='a' && buffer[start]<='z') || (buffer[start]>='A' && buffer[start]<='Z'))
					{
						cur_string[g_start++] = buffer[start++];
						state = 1;
					}
					else if(buffer[start]>='0' && buffer[start]<='9')
					{
						cur_string[g_start++] = buffer[start++];
						state = 3;
					}
					else if(buffer[start]=='+')
					{
						insert_Token("PLUS",NULL);
						start++;
					}
					else if(buffer[start]=='-')
					{
						insert_Token("MINUS",NULL);
						start++;
					}
					else if(buffer[start]=='/')
					{
						insert_Token("DIV",NULL);
						start++;
					}
					else if(buffer[start]=='<')
					{
						start++;
						state = 15;
					}
					else if(buffer[start]=='>')
					{
						start++;
						state = 19;
					}
					else if(buffer[start]=='=')
					{
						start++;
						state = 26;
					}
					else if(buffer[start]=='!')
					{
						start++;
						state = 28;
					}
					else if(buffer[start]==':')
					{
						start++;
						state = 30;
					}
					else if(buffer[start]=='.')
					{
						start++;
						state = 33;
					}
					else if(buffer[start]==';')
					{
						start++;
						insert_Token("SEMICOL",NULL);
					}
					else if(buffer[start]==',')
					{
						start++;
						insert_Token("COMMA",NULL);
					}
					else if(buffer[start]=='[')
					{
						start++;
						insert_Token("SQBO",NULL);
					}
					else if(buffer[start]==']')
					{
						start++;
						insert_Token("SQBC",NULL);
					}
					else if(buffer[start]=='(')
					{
						start++;
						insert_Token("BO",NULL);
					}
					else if(buffer[start]==')')
					{
						start++;
						insert_Token("BC",NULL);
					}
					else if(buffer[start]=='\n')
					{
						start++;
						row_number++;
					}
					else if(buffer[start]=='*')
					{
						start++;
						state = 23;
					}
					else if(buffer[start]==' ' || buffer[start]=='\t' || buffer[start]=='\r')
					{
						start++;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_2: Unknown Symbol %c at line %d\n",buffer[start],row_number);
						start++;
					}
					break;

			case 1: 
					if((buffer[start]>='a' && buffer[start]<='z') || (buffer[start]>='A' && buffer[start]<='Z') || (buffer[start]>='0' && buffer[start]<='9') || buffer[start]=='_')
						cur_string[g_start++] = buffer[start++];
					else
					{
						//if(buffer[start]=='\0')
						//	exec=0;
						cur_string[g_start] = '\0';
						search_ST(cur_string,"ID");
						state = 0;
					}
					break;

			case 3: 
					if(buffer[start]>='0' && buffer[start]<='9')
						cur_string[g_start++] = buffer[start++];
					else if(buffer[start]=='E' || buffer[start]=='e')
					{
						cur_string[g_start++] = buffer[start++];
						state = 7;
					}
					else if(buffer[start]=='.')
					{
						cur_string[g_start++] = buffer[start++];
						state = 4;	
					}
					else
					{
						cur_string[g_start] = '\0';
						search_ST(cur_string,"NUM");
						state = 0;
					}
					break;

			case 4: 
					if(buffer[start]>='0' && buffer[start]<='9')
					{
						cur_string[g_start++] = buffer[start++];
						state = 6;
					}
					else if(buffer[start]=='.')
					{
						cur_string[g_start-1] = '\0';
						start--;
						search_ST(cur_string,"NUM");
						state = 0;
					}
					else
					{
						//ERROR
						error = 1;
						cur_string[g_start] = '\0';
						printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
						state = 0;
					}
					break;

			case 6: 
					if(buffer[start]>='0' && buffer[start]<='9')
						cur_string[g_start++] = buffer[start++];
					else if(buffer[start]=='E' || buffer[start]=='e')
					{
						cur_string[g_start++] = buffer[start++];
						state = 7;
					}
					else
					{
						cur_string[g_start] = '\0';
						search_ST(cur_string,"RNUM");
						state = 0;
					}
					break;

			case 7: 
			        if(buffer[start]=='+' || buffer[start]=='-')
			        {
						cur_string[g_start++] = buffer[start++];
						state = 8;
					}
					else if(buffer[start]>='0' && buffer[start]<='9')
					{
						cur_string[g_start++] = buffer[start++];
						state = 9;
					}
					else
					{
						//ERROR
						error = 1;
						cur_string[g_start] = '\0';
						printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
						state = 0;
					}
					break;

			case 8: if(buffer[start]>='0' && buffer[start]<='9')
					{
						cur_string[g_start++] = buffer[start++];
						state = 9;
					}
					else
					{
						//ERROR
						error = 1;
						cur_string[g_start] = '\0';
						printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
						state = 0;
					}
					break;

			case 9: if(buffer[start]>='0' && buffer[start]<='9')
						cur_string[g_start++] = buffer[start++];
					else
					{
						cur_string[g_start] = '\0';
						search_ST(cur_string,"RNUM");
						state = 0;
					}
					break;

			case 15:if(buffer[start]=='=')
					{
						start++;
						insert_Token("LE",NULL);
						state = 0;
					} 
					else if(buffer[start]=='<')
					{
						start++;
						//insert_Token("DEF",NULL);
						state = 17;
					}
					else
					{
						insert_Token("LT",NULL);
						state = 0;
					}
					break;

			case 17: if(buffer[start]=='<')
					 {
					 	start++;
						insert_Token("DRIVERDEF",NULL);
						state = 0;
					 }
					 else
					 {
					 	insert_Token("DEF",NULL);
						state = 0;
					 }
					 break;

			case 19:if(buffer[start]=='=')
					{
						start++;
						insert_Token("GE",NULL);
						state = 0;
					} 
					else if(buffer[start]=='>')
					{
						start++;
						//insert_Token("ENDDEF",NULL);
						state = 21;
					}
					else
					{
						insert_Token("GT",NULL);
						state = 0;
					}
					break;

			case 21: if(buffer[start]=='>')
					 {
					 	start++;
						insert_Token("DRIVERENDDEF",NULL);
						state = 0;
					 }
					 else
					 {
					 	insert_Token("ENDDEF",NULL);
						state = 0;
					 }
					 break;

			case 23:
					if(buffer[start]=='*')
					{
						//implement comment ignoring part here
					}
					else
					{
						insert_Token("MUL",NULL);
						state = 0;
					}
					break;

			case 26:if(buffer[start]=='=')
					{
						start++;
						insert_Token("EQ",NULL);
						state = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: Unknown pattern = at line %d\n",row_number);
						state = 0;
					}
					break;

			case 28:if(buffer[start]=='=')
					{
						start++;
						insert_Token("NE",NULL);
						state = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: Unknown pattern ! at line %d\n",row_number);
						state = 0;
					}
					break;

			case 30:if(buffer[start]=='=')
					{
						start++;
						insert_Token("ASSIGNOP",NULL);
						state = 0;
					}
					else
					{
						insert_Token("COLON",NULL);
						state = 0;
					}
					break;

			case 33:if(buffer[start]=='.')
					{
						start++;
						insert_Token("RANGEOP",NULL);
						state = 0;
					}
					else
					{
						//ERROR
						error = 1;
						printf("ERROR_3: Unknown pattern . at line %d\n",row_number);
						state = 0;
					}
					break;
		}
	}
}

void getStream(FILE *fp)
{
	state = 0;
	g_start = 0;
	row_number = 1;
	int t;
	while(t=fread(buffer,1,100,fp))
	{
		buffer[t] = '\0';
		getNextToken();
		//printf("printing token -----\n\n");
		//printf("%s %ld\n\n",buffer,strlen(buffer));
	}

	switch(state)
	{
			case 1: 
					cur_string[g_start] = '\0';
					search_ST(cur_string,"ID");
					break;

			case 3: 
					cur_string[g_start] = '\0';
					search_ST(cur_string,"NUM");
					break;

			case 4: 
					//ERROR
					error = 1;
					cur_string[g_start] = '\0';
					printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
					break;

			case 6: 
					cur_string[g_start] = '\0';
					search_ST(cur_string,"RNUM");
					break;

			case 7: 
					//ERROR
					error = 1;
					cur_string[g_start] = '\0';
					printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
					break;

			case 8: 
					//ERROR
					error = 1;
					cur_string[g_start] = '\0';
					printf("ERROR_3: Unknown pattern %s at line %d\n",cur_string,row_number);
					break;

			case 9: 
					cur_string[g_start] = '\0';
					search_ST(cur_string,"RNUM");
					break;

			case 15:
					insert_Token("LT",NULL);
					break;

			case 17:
					insert_Token("DEF",NULL);
					break;

			case 19:
					insert_Token("GT",NULL);
					break;
			case 21:
					insert_Token("ENDDEF",NULL);
					break;
			case 23:
					insert_Token("MUL",NULL);
					break;

			case 26:
					//ERROR
					error = 1;
					printf("ERROR_3: Unknown pattern = at line %d\n",row_number);
					break;

			case 28:
					//ERROR
					error = 1;
					printf("ERROR_3: Unknown pattern ! at line %d\n",row_number);
					break;

			case 30:
					insert_Token("COLON",NULL);
					break;

			case 33:
					//ERROR
					error = 1;
					printf("ERROR_3: Unknown pattern . at line %d\n",row_number);
					break;
	}
}

void printT()
{
	struct Token *head = tokenList_head;
    while(head!=NULL)
    {
        printf("%s\t",head->tokenName);
        head->tokenVal==NULL ? print_lexeme(head->tokenName) : printf("%s\t",(head->tokenVal)->tokenVal);printf("%d\n",head->rowNum);
        head = head->Next;
    }
}

void DFAcomments(FILE *f)
{
	int start = 0;
	int exec = 1;
	while(exec)
	{
		if(g_start==100)
		{
			fwrite(cur_string,1,100,f);
			g_start = 0;
		}
		if(buffer[start]=='\0')
			{exec = 0;continue;}

		switch(state)
		{
			case 1: if(buffer[start]=='*')
					{
						start++;
						state = 2;
					}
					else
						cur_string[g_start++] = buffer[start++];
					break;

			case 2: if(buffer[start]=='*')
					{
						state = 3;
						start++;
					}
					else
					{
						cur_string[g_start++] = '*';
						state = 1;
					}
					break;

			case 3: if(buffer[start]=='*')
					{
						start++;
						state = 4;
					}
					else
						start++;
					break;

			case 4: if(buffer[start]=='*')
					{
						state = 1;
						start++;
					}
					else
						state = 3;
					break;		
		}
	}
}

void removeComments(FILE *fp)
{
	FILE *f = fopen("cleanFile.txt","w");
	fclose(f);
	f = fopen("cleanFile.txt","a");
	state = 1;
	g_start = 0;
	int t;
	while(t=fread(buffer,1,100,fp))
	{
		buffer[t] = '\0';
		DFAcomments(f);
	}
	if(state==2)
		cur_string[g_start++] = '*';
	fwrite(cur_string,1,g_start,f);
	fclose(f);
}
