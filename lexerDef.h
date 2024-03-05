#ifndef LEXERDEF_
#define LEXERDEF_
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
extern int bufsize;
extern char *begin;
extern char *fwd;
extern char *twinBuf[2];
extern int currBuf;
extern int state;
extern int lines;
extern int noInputLeft;
extern int flag;
extern FILE *fp;

typedef enum TokenName
{
    TK_ASSIGNOP,
    TK_COMMENT,
    TK_FIELDID,
    TK_ID,
    TK_NUM,
    TK_RNUM,
    TK_FUNID,
    TK_RUID,
    TK_WITH,
    TK_PARAMETERS,
    TK_END,
    TK_WHILE,
    TK_UNION,
    TK_ENDUNION,
    TK_DEFINETYPE,
    TK_AS,
    TK_TYPE,
    TK_MAIN,
    TK_GLOBAL,
    TK_PARAMETER,
    TK_LIST,
    TK_SQL,
    TK_SQR,
    TK_INPUT,
    TK_OUTPUT,
    TK_INT,
    TK_REAL,
    TK_COMMA,
    TK_SEM,
    TK_COLON,
    TK_DOT,
    TK_ENDWHILE,
    TK_OP,
    TK_CL,
    TK_IF,
    TK_THEN,
    TK_ENDIF,
    TK_READ,
    TK_WRITE,
    TK_RETURN,
    TK_PLUS,
    TK_MINUS,
    TK_MUL,
    TK_DIV,
    TK_CALL,
    TK_RECORD,
    TK_ENDRECORD,
    TK_ELSE,
    TK_AND,
    TK_OR,
    TK_NOT,
    TK_LT,
    TK_LE,
    TK_EQ,
    TK_GT,
    TK_GE,
    TK_NE
} TokenName;


typedef union number
{
    int NUM;
    float RNUM;
} number;

typedef struct Token
{
    TokenName tokenName;
    char *lexeme;
    int lineNo;
    int isNum;
    number *number;
} Token;

typedef struct symTb
{
    Token **tokens;
    int count;
    int size;
} symTb;

typedef struct keywordToTk
{
    char *keyword;
    TokenName tk;
} keywordToTk;


#endif