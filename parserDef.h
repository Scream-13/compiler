#ifndef PARSERDEF_
#define PARSERDEF_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "lexer.h"

#define TOTAL_GRAMMAR_NONTERMINALS 49
#define TOTAL_GRAMMAR_TERMINALS 58
#define NOF_RULES 87
typedef struct Node *node;
typedef struct hash *element;
typedef struct TreeNode *treeNode;
typedef element *hashtable;
typedef int **parseTable;
typedef struct Stack *stack;

int stack_max = 10000;
int parserError = 0;
char *Terminal_tokens[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "eps"};
char *Non_Terminal_tokens[] = {"inputparameters", "typedefinitions",
                               "iterativestmt", "singleorrecid", "lowprecedenceoperators", "stmts",
                               "factor", "optionalreturn", "operator", "declaration", "fielddefinition",
                               "highprecedenceoperators", "constructeddatatype", "termprime", "program",
                               "global_or_not", "input_par", "term", "outputparameters", "A", "expprime",
                               "output_par", "booleanexpression", "morefields", "iostmt", "returnstmt",
                               "otherfunctions", "parameter_list", "stmt", "definetypestmt",
                               "typedefinition", "arithmeticexpression", "funcallstmt", "remaining_list",
                               "datatype", "fielddefinitions", "more_ids", "var", "highprecedenceoperator",
                               "otherstmts", "relationalop", "function", "mainfunction", "logicalop", "assignmentstmt",
                               "conditionalstmt", "idlist", "primitivedatatype", "declarations"};
struct Node // stores gram
{
    int flag; // terminal =1; nt=-1 , eps=0
    char *string;
    struct Node *next;
};
struct hash
{
    node first;   // stores firsts
    node follow;  // stores follows
    int rule[50]; // number of rules on the rhs side
    int rulenum;  // store rule number
    char *value;  // non terminal
    int flag;
};

struct TreeNode
{
    treeNode next;
    treeNode child;
    treeNode parent;
    treeNode prev;
    int line;
    char *lexeme;
    int id;
};

struct Stack
{
    int *arr;
    int top;
};

char *CI[] = {
    "TK_ASSIGNOP",
    "TK_COMMENT",
    "TK_FIELDID",
    "TK_ID",
    "TK_NUM",
    "TK_RNUM",
    "TK_FUNID",
    "TK_RUID",
    "TK_WITH",
    "TK_PARAMETERS",
    "TK_END",
    "TK_WHILE",
    "TK_UNION",
    "TK_ENDUNION",
    "TK_DEFINETYPE",
    "TK_AS",
    "TK_TYPE",
    "TK_MAIN",
    "TK_GLOBAL",
    "TK_PARAMETER",
    "TK_LIST",
    "TK_SQL",
    "TK_SQR",
    "TK_INPUT",
    "TK_OUTPUT",
    "TK_INT",
    "TK_REAL",
    "TK_COMMA",
    "TK_SEM",
    "TK_COLON",
    "TK_DOT",
    "TK_ENDWHILE",
    "TK_OP",
    "TK_CL",
    "TK_IF",
    "TK_THEN",
    "TK_ENDIF",
    "TK_READ",
    "TK_WRITE",
    "TK_RETURN",
    "TK_PLUS",
    "TK_MINUS",
    "TK_MUL",
    "TK_DIV",
    "TK_CALL",
    "TK_RECORD",
    "TK_ENDRECORD",
    "TK_ELSE",
    "TK_AND",
    "TK_OR",
    "TK_NOT",
    "TK_LT",
    "TK_LE",
    "TK_EQ",
    "TK_GT",
    "TK_GE",
    "TK_NE",
    "$"};

#endif