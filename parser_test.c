#include "instrlist.h"
#include <stdio.h>

#define END_STATEMENT 66 //just to know that enter is over
#define SYNTAX_OK 100
#define SYNTAX_ERROR 99
/*void generateInstruction(int instType, void *addr1, void *addr2, void *addr3)
// vlozi novou instrukci do seznamu instrukci
{
   tInstr I;
   I.instType = instType;
   I.addr1 = addr1;
   I.addr2 = addr2;
   I.addr3 = addr3;
   listInsertLast(list, I);
}*/

//		1) <prog> 	   -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
// int statement[] = {CLASS, MAIN, LEFT_VINCULUM, STATIC, VOID, RUN, LEFT_BRACKET, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_STATEMENT}; 

		// 2) <prog>      -> CLASS ID LEFT_VINCULUM <class-dec>  // declaration of clas
// int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, RIGHT_VINCULUM, END_STATEMENT}; 
int statement[] = {CLASS, ID, LEFT_VINCULUM,STATIC, INT, ID, EQUAL, ID, LEFT_BRACKET, ID,COMMA, STRING_LITERAL, RIGHT_BRACKET,  SEMICOLON, RIGHT_VINCULUM, END_STATEMENT}; 

		// 3) <prog>      -> STATIC [INT/DOUBLE/SRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
// int statement[] = { STATIC, INT, ID , LEFT_BRACKET, INT, ID, COMMA, DOUBLE, ID,COMMA,STRING,ID, RIGHT_BRACKET, LEFT_VINCULUM, RIGHT_VINCULUM, END_STATEMENT}; 

int token;
int return_args();
int statement_list();
int func_var();
int program();
int equal();
int class_dec();
int func_args();
int func_args_list();
int func_params();
int func_params_list();
int bool_expr();
int math_expr();


int get_next_token(){
	static int number=0;
	if(statement[number] != END_STATEMENT)
		return statement[number++];
	else
		return END_STATEMENT;
}

// 1) <prog> -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
// 2) <prog> -> CLASS ID LEFT_VINCULUM <class-dec> // declaration of clas
// 3) <prog> -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>	//declaration and initialization of function
int program(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case CLASS:
			if ( (token = get_next_token()) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 1) <prog> 	   -> CLASS MAIN LEFT_VINCULUM STATIC VOID RUN LEFT_BRACKET RIGHT_BRACKET LEFT_VINCULUM <st-list>  RIGHT_VINCULUM
				case MAIN:
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM)
							 if ( (token = get_next_token()) != LEX_ERROR && token == STATIC)
							 	if ( (token = get_next_token()) != LEX_ERROR && token == VOID)
							 		if ( (token = get_next_token()) != LEX_ERROR && token == RUN)
									 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET)
										 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
											 	if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
											 		if( (result = statement_list()) != SYNTAX_OK)
											 			return result;
											 		else
													 	if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_VINCULUM)
														 	if ( (token = get_next_token()) != LEX_ERROR && token == END_STATEMENT)
														 		return SYNTAX_OK;
												}
						if(token == LEX_ERROR)
							return LEX_ERROR;						
						return SYNTAX_ERROR;
						break;	
// 2) <prog>      -> CLASS ID LEFT_VINCULUM <class-dec> // declaration of class						
				case ID: 
						if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = class_dec()) != SYNTAX_OK)
					 			return result;
							if ( (token = get_next_token()) != LEX_ERROR  && token == END_STATEMENT)
								return SYNTAX_OK;
						}
						if(token == LEX_ERROR)
							return LEX_ERROR;
						return SYNTAX_ERROR;
						break;
			}
			return SYNTAX_ERROR;
// 3) <prog>      -> STATIC [INT/DOUBLE/STRING/VOID] ID LEFT_BRACKET <func_params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list>
		case STATIC:
			if ( (token = get_next_token()) != LEX_ERROR  && (token == VOID || token == INT || token == DOUBLE || token == STRING) )
				if ( (token = get_next_token()) != LEX_ERROR  && token == ID)
					if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_BRACKET){
						if( (result = func_params()) != SYNTAX_OK)
					 			return result;
					 	if ( (token = get_next_token()) != LEX_ERROR  && token == LEFT_VINCULUM){
					 		if( (result = statement_list()) != SYNTAX_OK)
					 			return result;
					 		//else return SYNTAX_OK;
					 		if ( (token = get_next_token()) != LEX_ERROR && token == END_STATEMENT)
								return SYNTAX_OK;
					 	}
					}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;	
	}
	return SYNTAX_ERROR;
}

/*st-list>//function body
		1) <st-list>   -> RETURN <return-args> <st-list> //return in function
		2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> <st-list>//cycle while
		3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list> //if-else statement
		4) <st-list>   -> RIGHT_VINCULUM	// end of sequence
		5) <st-list>   -> STATIC [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list> // ????????? can we declare static variables in func ?
		6) <st-list>   -> [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list> //it can be function call OR inicialization of var*/
int statement_list(){
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	int result;
	switch(token){
// 1) <st-list>   -> RETURN <return-args> <st-list> 		
		case RETURN:
			if((result = return_args()) != SYNTAX_OK)
				return result;
			if((result = statement_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
// 2) <st-list>   -> WHILE LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> <st-list>			
		case WHILE:
			if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr()) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						else return SYNTAX_OK;
					}
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 3) <st-list>   -> IF LEFT_BRACKET <bool-expr> RIGHT_BRACKET LEFT_VINCULUM <st-list> ELSE LEFT_VINCULUM <st-list> <st-list>			
		case IF:
			if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET){
				if ( (result = bool_expr()) != SYNTAX_OK)
					return result;
				if ( (token = get_next_token()) != LEX_ERROR && token == RIGHT_BRACKET)
					if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						if ( (token = get_next_token()) != LEX_ERROR && token == ELSE)
							if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
							}
					}
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 4) <st-list>   -> RIGHT_VINCULUM			
		case RIGHT_VINCULUM:
			return SYNTAX_OK;
			break;
// 5) <st-list>   -> STATIC [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		case STATIC:
			if ( (token = get_next_token()) != LEX_ERROR && (token == INT || token == DOUBLE || token == STRING) )
				if ( (token = get_next_token()) != LEX_ERROR && token == ID)
					if ( (token = get_next_token()) != LEX_ERROR && token == EQUAL){
						if ( (result = equal()) != SYNTAX_OK)
							return result;
						if ( (result = statement_list()) != SYNTAX_OK)
							return result;
						else return SYNTAX_OK;
					}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
// 6) <st-list>   -> [INT/DOUBLE/SRING] ID EQUAL <equal>(MUST return something OR we must give some data to this func) <st-list>
		case INT:
		case DOUBLE:
		case STRING:
			if ( (token = get_next_token()) != LEX_ERROR && token == ID)
				if ( (token = get_next_token()) != LEX_ERROR && token == EQUAL){
					if ( (result = equal()) != SYNTAX_OK)
						return result;
					if ( (result = statement_list()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
				}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;	
 // 7) <st-list>   -> ID <func_var>(we must give pointer to ID) <st-list>
		case ID:
			if ( (result = func_var()) != SYNTAX_OK)
					return result;
			if ( (result = statement_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-params>//function DECLARATION + INIT
// 		1) <func-params> -> RIGHT_BRACKET
// 		2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>
int func_params(){
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	int result;
	switch(token){
// 1) <func-params> -> RIGHT_BRACKET		
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
// 2) <func-params> -> [INT/DOUBLE/SRING] ID <func-params-list>			
		case INT:
		case DOUBLE:
		case STRING:
			if ( (token = get_next_token()) != LEX_ERROR && token == ID){
				if ( (result = func_params_list()) != SYNTAX_OK)
					return result;
				else return SYNTAX_OK;
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-params-list>
// 		1) <func-params-list> -> RIGHT_BRACKET
// 		2) <func-params-list> -> COMMA [INT/DOUBLE/SRING] ID <func-params-list>
int func_params_list(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case COMMA:
			if ( (token = get_next_token()) != LEX_ERROR && (token == INT || token == DOUBLE || token == STRING))
				if ( (token = get_next_token()) != LEX_ERROR && token == ID){
					if ( (result = func_params_list()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
				}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
	}
	return SYNTAX_ERROR;
}

// <func_var>
// 		1) LEFT_BRACKET <func-args> SEMICOLON //its function call
// 		2) EQUAL <equal> //its inicialization of var
int func_var(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) LEFT_BRACKET <func-args> SEMICOLON //its function call	
		case LEFT_BRACKET:
			if ( (result = func_args()) != SYNTAX_OK)
				return result;
			if ( (token = get_next_token()) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
// 2) EQUAL <equal> //its inicialization of var			
		case EQUAL:
			if ( (result = equal()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <return-args>
// 		1) <return-args> -> SEMICOLON (ONLY if we in VOID function)
// 		2) <return-args> -> ID SEMICOLON
// 		3) <return-args> -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON
// 		4) <return-args> -> <math-expr> SEMICOLON
int return_args(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
// 1) <return-args> -> SEMICOLON (ONLY if we in VOID function)		
		case SEMICOLON:
			return SYNTAX_OK;
			break;
// 2) <return-args> -> ID SEMICOLON			
		case ID:
			if ( (token = get_next_token()) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
// 3) <return-args> -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON			
		case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:
			if ( (token = get_next_token()) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
		// case <math-expr>                    //FIXME
	}
	return SYNTAX_ERROR;
}


// <equal>	
// 		1) <equal>	   -> [INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] SEMICOLON
// 		2) <equal>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON 
// 		3) <equal>	   -> ID [SEMICOLON/EQUAL <equal>]
// 		4) <equal>	   -> <math-expr> SEMICOLON
int equal(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:
			if ( (token = get_next_token()) != LEX_ERROR && token == SEMICOLON)
				return SYNTAX_OK;
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
		case ID:
			if ( (token = get_next_token()) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){
// 2) <equal>	   -> ID LEFT_BRACKET <func-args>(we MUST give pointer to funtion) SEMICOLON 				
				case LEFT_BRACKET:
					if ( (result = func_args()) != SYNTAX_OK)
						return result;
					if ( (token = get_next_token()) != LEX_ERROR && token == SEMICOLON)
						return SYNTAX_OK;
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;
// 3) <equal>	   -> ID SEMICOLON					
				case SEMICOLON:
					return SYNTAX_OK;
					break;
// 3) <equal>	   -> ID EQUAL <equal>					
				case EQUAL:	
					if ( (result = equal()) != SYNTAX_OK)
						return result;
					else return SYNTAX_OK;
					break;
			}
			return SYNTAX_ERROR;
		// case <math-expr>                       //FIXME
	}
	return SYNTAX_ERROR;
}

// <func-args>//function CALL
		// 1) <func-args> -> RIGHT_BRACKET
		// 2) <func-args> -> [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case ID:
		case INT_LITERAL:
		case DOUBLE_LITERAL:
		case STRING_LITERAL:
			if ( (result = func_args_list()) != SYNTAX_OK)
				return result;
			else return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

// <func-args-list>
// 		1) <func-args-list> -> RIGHT_BRACKET
// 		2) <func-args-list> -> COMMA [ID/INT_LITERAL/DOUBLE_LITERAL/STRING_LITERAL] <func-args-list>
int func_args_list(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case RIGHT_BRACKET:
			return SYNTAX_OK;
			break;
		case COMMA:
			if ( (token = get_next_token()) != LEX_ERROR && 
				(token == INT_LITERAL || token == DOUBLE_LITERAL || token == STRING_LITERAL || token == ID)){

				if ( (result = func_args_list()) != SYNTAX_OK)
					return result;
				else return SYNTAX_OK;
			}
			if(token == LEX_ERROR)
				return LEX_ERROR;
			return SYNTAX_ERROR;	
			break;
	}
	return SYNTAX_ERROR;
}

// <class-dec>
// 		1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID \
//               [SEMICOLON/ EQ <equal>/LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>] <class-dec>
// 		2) <class-dec> -> RIGHT_VINCULUM
// 		3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>
int class_dec(){
	int result;
	if ( (token = get_next_token()) == LEX_ERROR )
		return LEX_ERROR;
	switch(token){
		case STATIC:
			if ( (token = get_next_token()) == LEX_ERROR )
				return LEX_ERROR;
			switch(token){				
				case INT:
				case DOUBLE:
				case STRING:
					if ( (token = get_next_token()) != LEX_ERROR && token == ID){
						if ( (token = get_next_token()) == LEX_ERROR )
							return LEX_ERROR;
						switch(token){
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID SEMICOLON <class-dec>							
							case SEMICOLON:
								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
								break;
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID  EQ <equal> <class-dec>								
							case EQUAL:
								if ( (result = equal()) != SYNTAX_OK)
									return result;
								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
								break;
// 1) <class-dec> -> STATIC [INT/DOUBLE/SRING] ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>							
							case LEFT_BRACKET:
								if ( (result = func_params()) != SYNTAX_OK)
									return result;
								if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
									if ( (result = statement_list()) != SYNTAX_OK)
										return result;
									if ( (result = class_dec()) != SYNTAX_OK)
										return result;
									else return SYNTAX_OK;
								}
								if(token == LEX_ERROR)
									return LEX_ERROR;
								return SYNTAX_ERROR;	
								break;
						}
						return SYNTAX_ERROR;
					}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;
// 3) <class-dec> -> STATIC VOID ID LEFT_BRACKET <func-params>(we MUST give pointer to funtion) LEFT_VINCULUM <st-list> <class-dec>					
				case VOID:
					if ( (token = get_next_token()) != LEX_ERROR && token == ID)
						if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_BRACKET){
							if ( (result = func_params()) != SYNTAX_OK)
								return result;
							if ( (token = get_next_token()) != LEX_ERROR && token == LEFT_VINCULUM){
								if ( (result = statement_list()) != SYNTAX_OK)
									return result;
								if ( (result = class_dec()) != SYNTAX_OK)
									return result;
								else return SYNTAX_OK;
							}
						}
					if(token == LEX_ERROR)
						return LEX_ERROR;
					return SYNTAX_ERROR;	
					break;		
			}
			return SYNTAX_ERROR;	
			break;
// 2) <class-dec> -> RIGHT_VINCULUM			
		case RIGHT_VINCULUM:
			return SYNTAX_OK;
			break;
	}
	return SYNTAX_ERROR;
}

int bool_expr(){
	return SYNTAX_OK;
}

int math_expr(){
	return SYNTAX_OK;
}

int main(int argc, char ** argv){
	printf("**************************************************************************\n");
	printf("\t100 - SYNTAX_OK\n\t99  - SYNTAX_ERROR\n\t98  - LEX_ERROR\n");
	printf("**************************************************************************\n");

	 printf("result:%d\n", program());
	return SYNTAX_OK;
}