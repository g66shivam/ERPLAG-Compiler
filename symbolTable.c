// BATCH NUMBER 30
// SHIVAM GUPTA 2014A7PS066P
// PULKIT GUPTA 2014A7PS157P

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "symbolTable.h"

int max(int a,int b)
{
	if(a > b)
		return a;
	else
		return b;
}
int get_width(int x)
{
	if(x == INTEGER)
		return 2;
	else
	if(x == BOOLEAN)
		return 1;
	else
	if(x == REAL)
		return 4;
	return -1;// error 
}
int get_type(char* str)
{
	if(strcmp(str,"integer") == 0)
		return INTEGER;
	else
	if(strcmp(str,"real") == 0)
		return REAL;
	else
	if(strcmp(str,"boolean") == 0)
	return BOOLEAN;
	else
		return ARRAY;
	
}
char* get_type_name(int x)
{
	if(x==INTEGER)
		strcpy(ret,"integer");
	if(x==REAL)
		strcpy(ret,"real");
	if(x==BOOLEAN)
		strcpy(ret,"boolean");
	if(x==ARRAY)
		strcpy(ret,"array");
	return ret;
}
int get_hash_value_sym(char* in)
{
	int ret = 0;
	int l = strlen(in);
	int i;
	for(i = 0;i < l ;i++)
	{
		ret = (ret*97+in[i])%MOD;
	}
	return ret;
}
pi* get_for_FOR(int x)
{
	pi* newnode = (pi*)malloc(sizeof(pi));
	newnode->sc = x;
	return newnode;
}
int put_hash_sym(int init, char* in)
{
	int ret = -10;
	int add = 1;// quadratic hashing
	int iter = 0;
	while(1)
	{
		iter++;
		if(iter == 10000)
			return -1;// error hai;
		if(hash[init][0] == '\0')
		{
			strcpy(hash[init], in);
			ret = init;
			break;
		}
		else
		if(strcmp(hash[init],in) == 0)
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
	return ret;
}
int hash_id(node* head,int typ)// returns -1 if error (redeclartion) else 0;
{
	int ret = -1;
	int iter = 0;
	node* ch;
	int y;
	while(head!=NULL)
	{
		iter++;
		if(iter == 1)
		ch = head->child;
		else
		ch = head->child->next;
		if(ch == NULL)
			break;
		// ch pakka term hoga
		//printf("TYPE%d\n",typ);
		//puts("hello1_hash_id");
		//puts(ch->ptr->tokenVal->tokenVal);
		//printf("scope hai %d\n",c_scope);
		int x = get_hash_value_sym(ch->ptr->tokenVal->tokenVal);
		x = put_hash_sym(x,ch->ptr->tokenVal->tokenVal);
		if(scope[c_scope][x].sc!=-1)// redecleration galat hai
		{
			ch->sym_ptr =  (pi*)malloc(sizeof (pi));
			ch->sym_ptr->dec = -1;
			ret = x;// that id's hash val for printing so after this other iterms arent considered in decleration
		}
		else
		{
			scope[c_scope][x].sc = c_scope;
			scope[c_scope][x].type = typ;
			scope[c_scope][x].dec = 0;
			//puts("chking");
			//int done = cur_offset[current_module];
			scope[c_scope][x].width = get_width(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
			strcpy(scope[c_scope][x].module_name,cur_scope_name);
			ch->sym_ptr = &scope[c_scope][x];// CHECK
			//printf("width hai %d  offset hai %d\n",ch->sym_ptr->width,ch->sym_ptr->offset);
		}
		head = ch->next;
	}
	//printf("iter%d\n",iter);
	return ret;
} 
int hash_array(node* head,int typ,int lo, int hi)// returns -1 if error (redeclartion) else 0;
{
	int ret = -1;
	//puts("in has_array");
	int iter = 0;
	int y;
	node* ch;
	while(head!=NULL)
	{
		iter++;
		if(iter == 1)
		ch = head->child;
	else
		ch = head->child->next;
		if(ch == NULL)
			break;
		// ch pakka term hoga
		//puts(ch->ptr->tokenVal->tokenVal);
		int x = get_hash_value_sym(ch->ptr->tokenVal->tokenVal);
		x = put_hash_sym(x,ch->ptr->tokenVal->tokenVal);
		if(scope[c_scope][x].sc!=-1)// redecleration galat hai
		{
			// dont scope it 
			ch->sym_ptr =  (pi*)malloc(sizeof (pi));
			ch->sym_ptr->dec = -1;
			ret = x;
			//return x;// that id's hash val for printing
		}
		else
		{
			//printf("name :%s lb:%d ub:%d type %d",ch->ptr->tokenVal->tokenVal,lo,hi,typ);
			scope[c_scope][x].dec = 0;
			scope[c_scope][x].sc = c_scope;
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = typ;
			scope[c_scope][x].lb = lo;
			scope[c_scope][x].ub = hi;
			if(lo > hi)
			{
				sprintf(error_log[err_cnt++],"ERROR:18 Arrays can have lower index greater than higher index at line_no:%d",ch->ptr->rowNum);
				semantic_error = 1;
			}
			scope[c_scope][x].width =  get_width(scope[c_scope][x].subtype)*(hi-lo+1);
			scope[c_scope][x].offset = cur_offset[current_module];
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
			strcpy(scope[c_scope][x].module_name,cur_scope_name);
			ch->sym_ptr = &scope[c_scope][x];
			//puts("chking");
			//printf("width hai %d  offset hai %d\n",ch->sym_ptr->width,ch->sym_ptr->offset);
		}
		head = ch->next;
	}
	//printf("iter%d\n",iter);
	return ret;
} 
int hash_input_list(node* head,int hash_val)
{
	int iter = 0;
	int ok = 0;
	while(head!=NULL && head->child!=NULL)
	{
		iter++;
		int y = 0;
		node* var;
		if(iter == 1) // the corresponding parlist will be  NULL
		var = head->child;
			else
		var = head->child->next;
		if(var == NULL)
			break;
		node*  d_type = var->next->next;
		//printf("variable aara hai :");
		//puts(var->ptr->tokenVal->tokenVal);
		int x = get_hash_value_sym(var->ptr->tokenVal->tokenVal);
		x = put_hash_sym(x,var->ptr->tokenVal->tokenVal);
		int datatype = get_type(d_type->child->ptr->tokenVal->tokenVal);
		scope[c_scope][x].sc = c_scope;
		scope[c_scope][x].dec = 0;
		strcpy(scope[c_scope][x].module_name,cur_scope_name);
		if(datatype!=ARRAY)
		{
			scope[c_scope][x].type = datatype;
			scope[c_scope][x].width = get_width(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		else
		{
			node* d_type_child = d_type->child;
			d_type_child = d_type_child->next->next;
			scope[c_scope][x].lb= atoi(d_type_child->child->ptr->tokenVal->tokenVal);
			scope[c_scope][x].ub= atoi(d_type_child->child->next->next->ptr->tokenVal->tokenVal);
			d_type_child = d_type_child->next->next->next;
			//puts("chk");
			//printf("%s\n",hash_nonterm[d_type_child->hash_val]);
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = get_type(d_type_child->child->ptr->tokenVal->tokenVal);// yahan dikkat hai
			scope[c_scope][x].width = get_width(scope[c_scope][x].subtype)*(scope[c_scope][x].ub-scope[c_scope][x].lb+1);
			scope[c_scope][x].offset = cur_offset[current_module];
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		//printf("Width hai %d , offset hai %d\n",scope[c_scope][x].width,scope[c_scope][x].offset);
		var->sym_ptr = &scope[c_scope][x];
		if(iter == 1)
		{
			in_list[hash_val] = (idlist*)malloc(sizeof(idlist));
			in_list[hash_val]->sc = scope[c_scope][x].sc;
			in_list[hash_val]->type = scope[c_scope][x].type;
			in_list[hash_val]->subtype = scope[c_scope][x].subtype;
			in_list[hash_val]->ub = scope[c_scope][x].ub;
			in_list[hash_val]->next = NULL;
			//printf("type hai %d\n",in_list[hash_val]->type);
		}
		else
		{	
			idlist* local = in_list[hash_val];
			while(local->next!=NULL)
				local = local->next;
			local->next = (idlist*)malloc(sizeof(idlist));
			local =  local->next;
			local->sc = scope[c_scope][x].sc;
			local->type = scope[c_scope][x].type;
			local->subtype = scope[c_scope][x].subtype;
			local->ub = scope[c_scope][x].ub;
			local->next = NULL;
			//printf("type hai %d\n",local->sc);
		}
		head = d_type->next;
	}
	return ok;
}
int hash_output_list(node* head,int hash_val)// cant return arrays rest is same as above
{
	//puts("in output list");
	int iter = 0;
	int ok = 0;
	while(head!=NULL && head->child!=NULL)
	{
		iter++;
		int y = 0;
		node* var;
		if(iter == 1) // the corresponding parlist will be  NULL
		var = head->child;
			else
		var = head->child->next;
		if(var == NULL)
			break;
		node*  d_type = var->next->next;
		//printf("variable aara hai :");
		//puts(var->ptr->tokenVal->tokenVal);
		int x = get_hash_value_sym(var->ptr->tokenVal->tokenVal);
		x = put_hash_sym(x,var->ptr->tokenVal->tokenVal);
		int datatype = get_type(d_type->child->ptr->tokenVal->tokenVal);
		scope[c_scope][x].sc = c_scope;
		scope[c_scope][x].dec = 0;
		strcpy(scope[c_scope][x].module_name,cur_scope_name);
		if(datatype!=ARRAY)
		{
			scope[c_scope][x].type = datatype;
			scope[c_scope][x].width = get_width(scope[c_scope][x].type);
			scope[c_scope][x].offset = cur_offset[current_module];
			cur_offset[current_module]+=scope[c_scope][x].width;
			ma_offset = max(ma_offset,cur_offset[current_module]);
		}
		else
		{
			sprintf(error_log[err_cnt++],"ERROR_10 Can't have arrays as return type of modules for module %s:",hash[hash_val]);
			semantic_error = 1;
			node* d_type_child = d_type->child;
			d_type_child = d_type_child->next->next;
			scope[c_scope][x].lb= atoi(d_type_child->child->ptr->tokenVal->tokenVal);
			scope[c_scope][x].ub= atoi(d_type_child->child->next->next->ptr->tokenVal->tokenVal);
			d_type_child = d_type_child->next->next;
			scope[c_scope][x].type = ARRAY;
			scope[c_scope][x].subtype = get_type(d_type_child->ptr->tokenVal->tokenVal);
		}
		//printf("Width hai %d , offset hai %d\n",scope[c_scope][x].width,scope[c_scope][x].offset);
		var->sym_ptr = &scope[c_scope][x];
		if(iter == 1)
		{
			out_list[hash_val] = (idlist*)malloc(sizeof(idlist));
			out_list[hash_val]->sc = scope[c_scope][x].sc;
			out_list[hash_val]->type = scope[c_scope][x].type;
			out_list[hash_val]->subtype = scope[c_scope][x].subtype;
			out_list[hash_val]->ub = scope[c_scope][x].ub;
			out_list[hash_val]->next = NULL;
			out_list[hash_val]->h_val = x;
			//printf("scope hai %d\n",out_list[hash_val]->sc);
		}
		else
		{	
			idlist* local = out_list[hash_val];
			while(local->next!=NULL)
				local = local->next;
			local->next = (idlist*)malloc(sizeof(idlist));
			local =  local->next;
			local->sc = scope[c_scope][x].sc;
			local->type = scope[c_scope][x].type;
			local->subtype = scope[c_scope][x].subtype;
			local->ub = scope[c_scope][x].ub;
			local->next = NULL;
			local->h_val = x;
			//printf("scope hai %d\n",local->sc);
		}
		head = d_type->next;
	}
	return ok;
}
int match_param(int x, node* head,int c_scope)
{
	//puts("chk");
	//	if(head != NULL)
		//printf("%s\n",hash_nonterm[head->hash_val]);
	int iter = 0;
	idlist* trav = in_list[x];
	while(trav!=NULL && head!=NULL)
	{
		//printf("%d\n",trav->type);
		iter++;
		node* ch;
		if(iter == 1)
		ch = head->child;
		else
		ch = head->child->next;
		int x = put_hash_sym(get_hash_value_sym(ch->ptr->tokenVal->tokenVal),ch->ptr->tokenVal->tokenVal);
		if(dec[x]==0 && def[x]==0)
		{//printf("%d  %s\n",scope[1][x].sc,head->ptr->tokenVal->tokenVal);
			int scope_trav = c_scope;
			while(scope_trav!=0) // no global decs assumption
			{
				if(scope[scope_trav][x].sc!=-1)
					break;
				else
					scope_trav = parent[scope_trav];
			}
			if(scope_trav == 0)// nahin mila error
			{
				sprintf(error_log[err_cnt++],"ERROR_1:%s Not declared at line no: %d\n",ch->ptr->tokenVal->tokenVal,ch->ptr->rowNum);
				semantic_error = 1;
				ch->sym_ptr = &blanknode;
			}
			else // update in parse tree' pointer
			{
				ch->sym_ptr = &scope[scope_trav][x];
			}
		}
		//puts("chk_match_param");
		//printf("%d %d\n",trav->type,ch->sym_ptr->type);
		if(ch->sym_ptr->type == trav->type)
		{
			if(trav->type == ARRAY)
			{
				//printf("trav %d ch %d\n",trav->subtype,ch->sym_ptr->subtype);
				if(trav->subtype!=ch->sym_ptr->subtype)
					return 0;
				else
				if(trav->ub!=ch->sym_ptr->ub)
					return 0;
			}
		}
		else
			return 0;
		head = ch->next;
		trav = trav->next;
	}
	//printf("%s\n",hash_term[head->child->hash_val]);
	if(head == NULL || head->child == NULL)
		return 0;
	if(trav == NULL && strcmp(hash_term[head->child->hash_val],"e") == 0)
		return 1;
	else
		return 0;
}
int match_retlist(int x, node* head,int c_scope) // note that relist cannot have arrays in decleration
{
	//puts("chk");
	//if(head != NULL)
		//printf("%s\n",hash_nonterm[head->hash_val]);
	int iter = 0;
	idlist* trav = out_list[x];
	while(trav!=NULL && head!=NULL)
	{
		//printf("%d\n",trav->type);
		iter++;
		node* ch;
		if(iter == 1)
		ch = head->child;
		else
		ch = head->child->next;
		int x = put_hash_sym(get_hash_value_sym(ch->ptr->tokenVal->tokenVal),ch->ptr->tokenVal->tokenVal);
		if(dec[x]==0 && def[x]==0)
		{//printf("%d  %s\n",scope[1][x].sc,head->ptr->tokenVal->tokenVal);
			int scope_trav = c_scope;
			while(scope_trav!=0) // no global decs assumption
			{
				if(scope[scope_trav][x].sc!=-1)
					break;
				else
					scope_trav = parent[scope_trav];
			}
			if(scope_trav == 0)// nahin mila error
			{
				sprintf(error_log[err_cnt++],"ERROR_1: %s Not declared at line no: %d\n",ch->ptr->tokenVal->tokenVal,ch->ptr->rowNum);
				semantic_error = 1;
				ch->sym_ptr = &blanknode;
			}
			else // update in parse tree' pointer
			{
				ch->sym_ptr = &scope[scope_trav][x];
			}
		}
		//puts("chk_match_param");
		//printf("%d %d\n",trav->type,ch->sym_ptr->type);
		if(ch->sym_ptr->type == trav->type)
		{
			if(trav->type == ARRAY)
			{
				//printf("trav %d ch %d\n",trav->subtype,ch->sym_ptr->subtype);
				if(trav->subtype!=ch->sym_ptr->subtype)
					return 0;
				else
				if(trav->ub!=ch->sym_ptr->ub)
					return 0;
			}
		}
		else
			return 0;
		head = ch->next;
		trav = trav->next;
	}
	//printf("%s\n",hash_term[head->child->hash_val]);
	if(head == NULL || head->child == NULL)
		return 0;
	if(trav == NULL && strcmp(hash_term[head->child->hash_val],"e") == 0)
		return 1;
	else
		return 0;
}
void all_assigned(int idx, node* head)
{
	if(head == NULL)
		return;
	if(head->is_non_term == 1 && strcmp(hash_nonterm[head->hash_val],"<assignmentStmt") == 0)
	{
		idlist* trav = out_list[idx];
		while(trav!=NULL && trav->h_val!=(put_hash_sym(get_hash_value_sym(head->child->ptr->tokenVal->tokenVal),head->child->ptr->tokenVal->tokenVal)))
		{
			trav = trav->next;
		}
		if(trav!=NULL)
			trav->mark = 1;
	}
	node* temp = head->child;
	while(temp!=NULL)
	{
		all_assigned(idx,temp);
		temp = temp->next;
	}

}
void traverse(node* head)
{
	if(head == NULL)
		return;
	int has = head->hash_val;
	if(head->is_non_term == 1)// chance of getting declareStmt
	{
		//printf("%d\n",c_scope);
		//printf("nxt hash val --> ");
		//puts(hash_nonterm[has]);
		if(strcmp(hash_nonterm[has],"<declareStmt") == 0)// bingo // CHECK
		{
			
			node* t = head->child;
			t = t->next;// idlist
			node* typee = t->next->next;// datatype
			int put_type = get_type(typee->child->ptr->tokenVal->tokenVal);
			if(put_type!=ARRAY)
			{
				int chk = hash_id(t,put_type);
				if(chk!=-1) // -1 here means all went well since hash val can't be -1
				{
					sprintf(error_log[err_cnt++],"ERROR_2: %s was declared more than once at line no: %d\n",hash[chk],typee->child->ptr->rowNum);
					semantic_error = 1;
				}
			}
			else // array hai
			{
				//puts("dd");
				node* rng_ptr = typee->child->next->next;
				node* sub_type_ptr = rng_ptr->next->next->next;
				int sub_type = get_type(sub_type_ptr->child->ptr->tokenVal->tokenVal);
				int lo = atoi(rng_ptr->child->ptr->tokenVal->tokenVal);
				int hi = atoi(rng_ptr->child->next->next->ptr->tokenVal->tokenVal);
				int chk = hash_array(t,sub_type,lo,hi);
				if(chk!=-1) // -1 here means all went well since hash val can't be -1
				{
					sprintf(error_log[err_cnt++],"ERROR_2: %s was declared more than once at line no: %d\n",hash[chk],typee->child->ptr->rowNum);
					semantic_error = 1;
				}
			}
		}
		else // DECLARATIONS
		if(strcmp(hash_nonterm[has],"<moduleDeclaration") == 0)
		{
			node* mod_name = head->child->next->next;
			//puts(mod_name->ptr->tokenVal->tokenVal);
			int hash_val = put_hash_sym(get_hash_value_sym(mod_name->ptr->tokenVal->tokenVal),mod_name->ptr->tokenVal->tokenVal);
			if(dec[hash_val])
			{
				sprintf(error_log[err_cnt++],"ERROR_3: %s is being redeclared at line no: %d\n",mod_name->ptr->tokenVal->tokenVal,mod_name->ptr->rowNum);
				semantic_error = 1;
			}
			dec[hash_val] = 1;
		}
		else
		if(strcmp(hash_nonterm[has],"<module") == 0)
		{
			int hash_val;
			already_scoped = 1;
			int new_scope = ++global_scope_ctr;
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			current_module = c_scope;
			node* inp_list = head->child;
			int ctr = 0;
			while(ctr!=7)
			{
				inp_list = inp_list->next;
				ctr++;
				if(ctr == 2) // naam aa gaya
				{
					//puts(inp_list->ptr->tokenVal->tokenVal);
					hash_val = put_hash_sym(get_hash_value_sym(inp_list->ptr->tokenVal->tokenVal),inp_list->ptr->tokenVal->tokenVal);
					if(def[hash_val] == 1)
					{
						sprintf(error_log[err_cnt++],"ERROR_4: %s is being redefind at line no: %d\n",inp_list->ptr->tokenVal->tokenVal,inp_list->ptr->rowNum);
						semantic_error = 1;
					}
					def[hash_val] = 1;
					strcpy(cur_scope_name,inp_list->ptr->tokenVal->tokenVal);
					//printf("\n\n\n\n\n\n\n\n%s",cur_scope_name);
				}
			}
			//puts("chk");
			//puts(hash_nonterm[inp_list->hash_val]);
			//puts(hash[hash_val]);
			int ret_val = hash_input_list(inp_list,hash_val);// hash vars into new scope + fill def list variables for this module
			if(ret_val)
			{
				sprintf(error_log[err_cnt++],"ERROR_5: Some anomaly in parameter list of module %s",hash[hash_val]);
				semantic_error = 1;
			}
			ret_val = 0;
			node* outp_list = inp_list;
			outp_list = outp_list->next->next->next;
			if(strcmp(hash_term[outp_list->child->hash_val],"e") != 0)
			{
				//printf("%s\n",hash_term[outp_list->child->hash_val]);
				outp_list = outp_list->child->next->next;
				ret_val = hash_output_list(outp_list,hash_val);
				all_assigned(hash_val,head);
				idlist* s_trav = out_list[hash_val];
				while(s_trav!=NULL)
				{
					if(s_trav->mark!=1)
					{
						sprintf(error_log[err_cnt++],"ERROR_18: variable %s in module %s return list not assigned a value in module",hash[s_trav->h_val],hash[hash_val]);
						semantic_error = 1;
					}
					s_trav = s_trav->next;
				}
			}
		}
		else
		if(strcmp(hash_nonterm[has],"<driverModule") == 0)
		{
			already_scoped = 1;
			int new_scope = ++global_scope_ctr;
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			current_module = c_scope;
			strcpy(cur_scope_name,"driver");
		}
		else
		if(strcmp(hash_nonterm[has],"<moduleReuseStmt") == 0) // calling modules handling functions defined above
		{
			//debugg;
			node* to_call = head->child->next->next->next;
			int hash_call = put_hash_sym(get_hash_value_sym(to_call->ptr->tokenVal->tokenVal),to_call->ptr->tokenVal->tokenVal);
			if(def[hash_call])	
			{
				if(strcmp(cur_scope_name,to_call->ptr->tokenVal->tokenVal) == 0)
				{	
					sprintf(error_log[err_cnt++],"ERROR_6 Recursion not allowed at line no: %d\n",to_call->ptr->rowNum);
					semantic_error = 1;
				}
				node* list  =  to_call->next->next->next;
				int input_match = match_param(hash_call,list,c_scope);
				if(input_match == 0)
				{	
					sprintf(error_log[err_cnt++],"ERROR_7 %s parameter list doesn't match the call at line no: %d\n",to_call->ptr->tokenVal->tokenVal,to_call->ptr->rowNum);
					semantic_error = 1;
				}
				int output_match = 1;
				if(strcmp(hash_term[head->child->child->hash_val],"SQBO") == 0) // there's a return list
					output_match = match_retlist(hash_call,head->child->child->next,c_scope);
				if(output_match == 0)
				{
					sprintf(error_log[err_cnt++],"ERROR_8 %s return list doesn't match the call at line no: %d\n",to_call->ptr->tokenVal->tokenVal,to_call->ptr->rowNum);
					semantic_error = 1;
				}
			}
			else
			if(dec[hash_call])
			{
				node* list  =  to_call->next->next->next;
				todo[lzyptr].in = list;
				todo[lzyptr].scope = c_scope;
				if(strcmp(hash_term[head->child->child->hash_val],"SQBO") == 0)
				todo[lzyptr].out = head->child->child->next;
				else
				todo[lzyptr].out = NULL;
				todo[lzyptr].x = hash_call;
				lzyptr++;
				// handle case of declaration w/o definition
			}
			else
			{
				to_call->sym_ptr = &blanknode;
				sprintf(error_log[err_cnt++],"ERROR_9: The function %s has neither been defined nor been declared earlier (line no %d)\n",hash[hash_call],to_call->ptr->rowNum);
				semantic_error = 1;
			}
		}
		//printf("%d\n",c_scop);
	}
	// declrations checked !
	if(head->is_non_term == 0) // starts and ends
	{
		//printf("%d\n",c_scope);
		//printf("nxt hash val --> ");
		//puts(hash_term[has]);
		if(strcmp(hash_term[head->hash_val],"START") == 0)
		{
			if(!already_scoped)
			{
				int new_scope = ++global_scope_ctr;
				parent[new_scope] = c_scope;
				c_scope = new_scope;
			}
			else
			{	
				already_scoped = 0;
			}
			//head->sym_ptr = get_for_start(c_scope);
			start[c_scope] = head->ptr->rowNum;;
		}
		else
		if(strcmp(hash_term[head->hash_val],"END") == 0)
		{
			end[c_scope] = head->ptr->rowNum;
			c_scope = parent[c_scope];
		}
		else
		if(strcmp(hash_term[head->hash_val],"FOR") == 0)
		{
			already_scoped = 1;
			int new_scope = ++global_scope_ctr;
			parent[new_scope] = c_scope;
			c_scope = new_scope;
			head->sym_ptr = get_for_FOR(c_scope);
		}
		else
		if(strcmp(hash_term[head->hash_val],"ID") == 0)
		{
			int x = put_hash_sym(get_hash_value_sym(head->ptr->tokenVal->tokenVal),head->ptr->tokenVal->tokenVal);
			if(dec[x]==0 && def[x]==0)
			{//printf("%d  %s\n",scope[1][x].sc,head->ptr->tokenVal->tokenVal);
				int scope_trav = c_scope;
				while(scope_trav!=0) // no global decs assumption
				{
					if(scope[scope_trav][x].sc!=-1)
						break;
					else
						scope_trav = parent[scope_trav];
				}
				if(scope_trav == 0 && head->sym_ptr == NULL)// nahin mila error
				{
					sprintf(error_log[err_cnt++],"ERROR_1: %s Not declared at line no: %d\n",head->ptr->tokenVal->tokenVal,head->ptr->rowNum);
					semantic_error = 1;
					head->sym_ptr = &blanknode;
				}
				else // update in parse tree' pointer
				{
					head->sym_ptr = &scope[scope_trav][x];
				}
			}
		}
		//printf("%d\n",c_scope);
	}
	node* temp = head->child;
	while(temp!=NULL)
	{
		traverse(temp);
		temp = temp->next;
	}	
}
void build_symbol_table(node* pt_head)
{
	blanknode.sc = -1;blanknode.type = -1;
	blanknode.subtype = -1;blanknode.lb = -1;
	blanknode.ub = -1;blanknode.offset = -1;
	blanknode.dec = -1;
	memset(parent,-1,sizeof parent);
	memset(scope,-1,sizeof scope);
	int i;
	struct Symbol_Table* trav = symbolTable_head;
	while(trav!=NULL)
	{
		if(strcmp(trav->tokenName,"ID")==0)
		{
			int x = get_hash_value_sym(trav->tokenVal);
			x = put_hash_sym(x,trav->tokenVal);
			//printf("%d  %s\n",x,trav->tokenVal);
			//puts(trav->tokenVal);
		}
		trav = trav->Next;
	}
	// all variables hashed
	traverse(pt_head);// start scopes from 1 so 0 is unassigned
	for (i = 0; i <=1000 ; ++i)
	{
		int iter = 1;
		int cur = i;  
		while(parent[cur]!=0)
		{
			++iter;
			cur = parent[cur];
		}
		scope_map[i] = iter;
	}

}
void print_symbol_table(node* head)
{ 
	int m;
	if(head == NULL)
		return;
	if(strcmp(hash_term[head->hash_val],"ID") == 0)
	{	
		int x = put_hash_sym(get_hash_value_sym(head->ptr->tokenVal->tokenVal),head->ptr->tokenVal->tokenVal);
		if(dec[x]==0 && def[x]==0 && head->sym_ptr->dec == 0)
		{
			head->sym_ptr->dec = 1;
			++sno;
			printf("%-4d ",sno);
			//m = strlen(head->ptr->tokenVal->tokenVal);
			//m = 12-m;
			printf("%-12s",head->ptr->tokenVal->tokenVal);
			int ctr = printf("%s",get_type_name(head->sym_ptr->type));
			if(head->sym_ptr->type == ARRAY)
			{	
				ctr+=printf("(%d, %s)",head->sym_ptr->ub-head->sym_ptr->lb+1,get_type_name(head->sym_ptr->subtype));
			}
			int it = 0;
			for(it = 0;it<=22-ctr;it++)
				printf(" ");
			int y = head->sym_ptr->sc;
			printf("%-13s",head->sym_ptr->module_name);
			it = 0;
			it+=printf("%d ",start[y]);
			it+=printf("to ");
			it+=printf("%d ",end[y]);
			while(it!=14)
			{
				it++;
				printf(" ");
			}
			printf("%-9d",scope_map[head->sym_ptr->sc]);
			printf("%-7d%d\n",head->sym_ptr->width,head->sym_ptr->offset);
			var_chk[x] = 1;
		}
	}
	node* temp = head->child;
	while(temp!=NULL)
	{
		print_symbol_table(temp);
		temp = temp->next;
	}	
}
void deficit_errors()
{
	int i;
//	puts("\n\n\n\n");
	for (i = 0; i < lzyptr; ++i)
	{
		//printf("%s\n",hash[todo[i].x]);
		int input_match = match_param(todo[i].x,todo[i].in,todo[i].scope);
		if(input_match == 0)
		{
			sprintf(error_log[err_cnt++],"ERROR_7: %s parameter list doesn't match the call\n",hash[todo[i].x]);
			semantic_error = 1;
		}
		int output_match = 1;
		if(todo[i].out!=NULL) // there's a return list
			output_match = match_retlist(todo[i].x,todo[i].out,todo[i].scope);
		if(output_match == 0)
		{
			sprintf(error_log[err_cnt++],"ERROR_8: %s return list doesn't match the call\n",hash[todo[i].x]);
			semantic_error = 1;
		}
	}
}

void print_sym_table_errors()
{
	//printf("%d",err_cnt);
	int i;
	for (i = 0; i < err_cnt; ++i)
	{
		printf("%s",error_log[i]);
	}
}