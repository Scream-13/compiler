#ifndef PARSE_
#define PARSE_
#include"parserDef.h"

int IsTerminal(char *str);
int IsNonTerminal(char *str);
Hashtable createHashTable();
int hashcode(char *str);
Element createElement(char *str);
void insertToHashTable(Hashtable ht, char *str);
Element searchTable(Hashtable ht, char *str);
void addFirstFollow(Node **list, char *str, int flag);
void freeHashTable(Hashtable ht);
void readGrammar(Hashtable ht, const char *filename);
void freeNodeList(Node *head);
void readFirsts(Hashtable ht, const char *filename);
void readFollows(Hashtable ht, const char *filename);
void printHashTable(Hashtable ht);
stack initStack();
void push(stack s, int rule);
int top(stack s);
int pop(stack s);
int isEmpty(stack s);
treeNode createTreeNode(char *lexeme, int id);
treeNode addChildRule(treeNode parent, node n, int line, char *lexeme);
int getCol(char *str);
int getRow(char *str);
int getId(char *str);
int isFollow(int C, int curTkName, hashtable h);
treeNode parseCode(FILE *fp, parseTable parseTb, node *gramRule, Token *curTk, hashtable h, int *error);


#endif