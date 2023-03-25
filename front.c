/* front.c - a lexical analyzer system for simple
 arithmetic expressions */
#include<stdio.h>
#include<ctype.h>
#include <stdbool.h>

/* Global declarations */
/* Variables */
int charClass;
char lexeme[100];
char nextChar;
int lexLen;
int token;
int nextToken;
char lastToken;
const char *tokenNames[35];
FILE *in_fp;

/* Function declarations */
int lookup(char ch);
void addChar(void);
void getChar(void);
void getNonBlank(void);
void setTokens(void);
int lex(void);

/* Character classes */
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/* Token codes */
#define Float_Lit 9
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define MOD_OP 25
#define LEFT_PAREN 26
#define RIGHT_PAREN 27
#define EQUALS_OP 28
#define LT_OP 29
#define LTE_OP 30
#define GT_OP 31
#define GTE_OP 32
#define AND_OP 33
#define OR_OP 34

/******************************************************/
/* main driver */
int main(void) {

	
	setTokens();
	/* Open the input data file and process its contents */
	 if ((in_fp = fopen("front.in", "r")) == NULL)
	 	printf("ERROR - cannot open front.in \n");
	 else {
	 	getChar();
	 do {
	 	lex();
	 } while (nextToken != EOF);
	 }
	 return 0;
}
//sets the token name to the position of its definition in an array
void setTokens(void){
tokenNames[LETTER]="LETTER";
 tokenNames[DIGIT ] ="DIGIT";
 tokenNames[UNKNOWN] = "UNKNOWN";
 tokenNames[Float_Lit ] = "FLOATING_POINT_LIT";
 tokenNames[INT_LIT ] = "INT_LIT";
 tokenNames[IDENT] = "IDENT";
 tokenNames[ASSIGN_OP ] = "ASSIGN_OP";
 tokenNames[ADD_OP ] = "ADD_OP";
 tokenNames[SUB_OP ] = "SUB_OP";
 tokenNames[MULT_OP] = "MULT_OP";
 tokenNames[DIV_OP ] = "DIV_OP";
 tokenNames[MOD_OP ] = "MOD_OP";
 tokenNames[LEFT_PAREN ] ="LEFT_PAREN";
 tokenNames[RIGHT_PAREN] ="RIGHT_PAREN";
 tokenNames[EQUALS_OP] ="EQUALS_OP";
 tokenNames[LT_OP ] = "LESS_THAN_OP";
 tokenNames[LTE_OP ]= "LESS_THAN_EQUAL_TO_OP";
 tokenNames[GT_OP]  = "GREATER_THAN_OP";
 tokenNames[GTE_OP] = "GREATER_THAN_EQUAL_TO_OP";
 tokenNames[AND_OP ] = "AND_OP";
 tokenNames[OR_OP] = "OR_OP"; 
}

/******************************************************/
/* getChar - a function to get the next character of
 input and determine its character class */
void getChar(void) {
	 if ((nextChar = getc(in_fp)) != EOF) {
		 if (isalpha(nextChar))
		 	charClass = LETTER;
		 else if (isdigit(nextChar))
		 	charClass = DIGIT;
		 else
		 	charClass = UNKNOWN;
	 } else
	 	charClass = EOF;
}

/******************************************************/
/* lex - a simple lexical analyzer for arithmetic
 expressions */
int lex(void) {
	 lexLen = 0;
	 getNonBlank();
	 switch (charClass) {
		/* Identifiers */
		 case LETTER:
			 addChar();
			 getChar();
			 while (charClass == LETTER || charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 nextToken = IDENT;
			 break;
		/* Integer && floating point literals */
		 case DIGIT:
		 	bool isfloat = false;
			 addChar();
			 getChar();
			 while (charClass == DIGIT) {
				 addChar();
				 getChar();
			 }
			 if (nextChar == '.'){
				addChar();
				getChar();
				isfloat = true;
			 } 
			 while(charClass == DIGIT){
				addChar();
				getChar();
			 }
			 if(!isfloat)
			 	nextToken = INT_LIT;
			 else
			 	nextToken=Float_Lit;
		 	break;
		/* Parentheses and operators */
		 case UNKNOWN:
			 lookup(nextChar);
			 break;
			/* EOF */
		case EOF:
			 nextToken = EOF;
			 lexeme[0] = 'E';
			 lexeme[1] = 'O';
			 lexeme[2] = 'F';
			 lexeme[3] = '\0';
		 	break;
	 } /* End of switch */
	 printf("Next token is: %s, Next lexeme is %s\n",
	 tokenNames[nextToken], lexeme);
	 return nextToken;
} /* End of function lex */

/******************************************************/
/* lookup - a function to look up operators and
 parentheses and return the token */
int lookup(char ch) {
	 switch (ch) {
		 case '(':
			 addChar();
			 getChar();
			 nextToken = LEFT_PAREN;
			 break;
		 case ')':
			 addChar();
			 getChar();
			 nextToken = RIGHT_PAREN;
			 break;
		 case '+':
			 addChar();
			 getChar();
			 nextToken = ADD_OP;
			 break;
		 case '-':
			 addChar();
			 getChar();
			 nextToken = SUB_OP;
			 break;
		 case '*':
			 addChar();
			 getChar();
			 nextToken = MULT_OP;
			 break;
		 case '/':
			 addChar();
			 getChar();
			 nextToken = DIV_OP;
			 break;
		 case '%':
		 	addChar();
			getChar();
			nextToken = MOD_OP;
			break;
		 case '=':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken= ASSIGN_OP;
			if(nextChar=='='){
			addChar();
			nextToken = EQUALS_OP;
			getChar();
			}
			break;
		 case '<':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=LT_OP;
			if(nextChar=='='){
			addChar();
			nextToken=LTE_OP;
			getChar();
			}
		 	break;
		 case '>':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken = GT_OP;
			if(nextChar=='='){
			addChar();
			nextToken=GTE_OP;
			getChar();
			}
			
		 	break;
		 case '&':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=AND_OP;
			if(nextChar=='&'){
			addChar();
			nextToken=AND_OP;
			getChar();
			}
			
		 	break;
		 case '|':
		 	addChar();
			getChar();
			getNonBlank();
			nextToken=OR_OP;
			if(nextChar=='|'){
			addChar();
			nextToken=OR_OP;
			getChar();
			}
			
		 	break;
		 default:
			 addChar();
			 getChar();
			 nextToken = EOF;
			 break;
	 }
	 return nextToken;
}

/******************************************************/
/* addChar - a function to add nextChar to lexeme */
void addChar(void) {
	if (lexLen <= 98) {
		lexeme[lexLen++] = nextChar;
		lexeme[lexLen] = '\0';
	} else
	printf("Error - lexeme is too long \n");
}


/******************************************************/
/* getNonBlank - a function to call getChar until it
 returns a non-whitespace character */
void getNonBlank(void) {
	while (isspace(nextChar))
	getChar();
}
