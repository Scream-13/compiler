#ifndef LEX_
#define LEX_

#include "lexerDef.h"

void initGetTkName();
FILE *getStream(FILE *f1);
symTb *initializeSymTb();
void initializeLexer(FILE *f);
char getChar(FILE *f1);
void tokenized();
char *makeLexeme(char *first, char *last);
Token *makeToken(TokenName tokenName, char *lexeme, int lineNo, int isNum, number *number);
int compareString(char *c1, char *c2);
void retract(int r);
Token *normalReturn(TokenName tokenName);
void deleteSymTb();
void addToSymTb(Token *tk);
Token *getNextToken();
void removeComments(char *tc, char *clean);

#endif