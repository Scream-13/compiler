#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define TOTAL_GRAMMAR_NONTERMINALS 49
#define TOTAL_GRAMMAR_TERMINALS 58
#define NOF_RULES 87
typedef struct Node *node;
typedef struct hash *element;
typedef element *hashtable;
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
    node next;
};
struct hash
{
    node first;  // stores firsts
    node follow; // stores follows
    int *rule;   // number of rules on the rhs side
    int rulenum; // store rule number
    char *value; // non terminal
    int flag;
};
hashtable createHashTable()
{
    hashtable ht = (hashtable)malloc(sizeof(hashtable));
    if (ht == NULL)
        printf("Memory Error!\n");
    return ht;
}
int IsTerminal(char *str)
{
    for (int i = 0; i < TOTAL_GRAMMAR_TERMINALS; i++)
    {
        if (strcmp(str, Terminal_tokens[i]) == 0)
            return 1;
    }
    return 0;
}
int IsNonTerminal(char *str)
{
    for (int i = 0; i < TOTAL_GRAMMAR_NONTERMINALS; i++)
    {
        if (strcmp(str, Non_Terminal_tokens[i]) == 0)
            return 1;
    }
    return 0;
}

element ElemsForHT(char *str)
{
    element entry = (element)malloc(sizeof(struct hash));
    entry->first = NULL;
    entry->follow = NULL;
    entry->value = (char *)malloc(sizeof(char) * 100); // tree branch selection
    entry->rule = (int *)malloc(sizeof(int) * 50);     // storing
    entry->rulenum = 0;
    entry->flag = 0;
    if (strcmp(str, "eps") == 0)
    {
        entry->flag = 0;
    }
    if (IsTerminal(str))
    {
        entry->flag = 1;
    }
    if (IsNonTerminal(str))
    {
        entry->flag = -1;
    }
    return entry;
}
// calculating hashcode
int hashcode(char *str)
{
    const int p = 17;
    const int hash_size = 500;
    int val = 0;
    int power = p;
    for (int i = 0; i < strlen(str); i++)
    {
        val += (str[i] * power) % hash_size;
        power *= p;
    }
    return hash_size;
}
// check if the element is already in the table
element searchTable(hashtable h, char *str)
{
    int hcd = hashcode(str);
    if (h[hcd] == NULL)
        return NULL;
    else
        return h[hcd];
}
// inserting to the table
void insertToHashTable(element x, hashtable h)
{
    if (searchTable(h, x->value) != NULL)
    {
        printf("collision \n");
    }
    int value = hashcode(x->value);
    h[value] = x;
}

// reading grammar
node *readGrammar(hashtable ht)
{
    node *gramRules = (node *)malloc(sizeof(node) * NOF_RULES);

    // reading the file storing gram rules
    FILE *fp = fopen("grammar.txt", "r");
    if (fp == NULL)
    {
        printf("FILE ERROR\n");
        exit(1);
    }

    char *token; // tokens of gram rules
    token = (char *)malloc(sizeof(char) * 50);
    char gramLine[200];
    char delims[] = " \t\r\n\v\f";
    int i = 0;
    while (fgets(gramLine, 200, fp) != NULL)
    {
        token = strtok(gramLine, delims); // first element of the rule
        struct Node *temp = (struct Node *)malloc(sizeof(struct Node));
        temp->string = (char *)malloc(sizeof(char) * 50);
        strcpy(temp->string, token);
        temp->flag = -1; // as first token of any rule will be a nonterminal

        // pushing it to hashtable
        struct hash *h = searchTable(ht, token);
        if (h == NULL)
        {
            h = ElemsForHT(token);
            insertToHashTable(h, ht);
        }
        h->rule[h->rulenum] = i;
        h->rulenum++;
        token = strtok(NULL, delims);
        while (token != NULL)
        {
            temp->next = (node)malloc(sizeof(struct Node));
            ;
            temp = (temp->next);
            temp->string = (char *)malloc(50 * sizeof(char));
            strcpy(temp->string, token);
            if (IsTerminal(token))
            {
                temp->flag = 1;
            }
            else if (strcmp(token, "eps") == 0)
            {
                temp->flag = 0;
            }
            else
                temp->flag = -1;

            element h = searchTable(ht, token);
            if (h == NULL)
            {
                h = ElemsForHT(token);
                insertToHashTable(h, ht);
            }

            token = strtok(NULL, delims);
        }
        gramRules[i] = temp;
        i++;
    }
    rewind(fp);
    fclose(fp);
    return gramRules;
}
// reading firsts of different non terminals
void first(hashtable ht, char *firstfile)
{
    FILE *fpfirst = fopen(firstfile, "r");
    if (fpfirst == NULL)
    {
        printf("Unable to read follow file.\n");
        return;
    }
    char *token; // tokens of gram rules
    token = (char *)malloc(sizeof(char) * 50);
    char gramLine[300];
    char delims[] = " \t\r\n\v\f";

    while (fgets(gramLine, 300, fpfirst) != NULL)
    {
        token = strtok(gramLine, delims); // first element of the rule
        // pushing it to hashtable
        struct hash *lhs = searchTable(ht, token);
        token = strtok(NULL, delims);

        lhs->first = (node)malloc(sizeof(struct Node));
        struct Node *temp = lhs->first;
        while (token != NULL)
        {
            temp->string = (char *)malloc(sizeof(char) * 50);
            temp->flag = 1;
            strcpy(temp->string, token);
            token = strtok(NULL, delims);
            if (token != NULL)
            {
                temp->next = (node)malloc(sizeof(struct Node));
                temp = temp->next;
            }
        }
        temp = NULL;
    }
    rewind(fpfirst);
    fclose(fpfirst);
}

// reading follow
void follow(hashtable ht, char *followfile)
{
    FILE *fpfollow = fopen(followfile, "r");
    if (fpfollow == NULL)
    {
        printf("Unable to read follow file.\n");
        return;
    }
    char *token; // tokens of gram rules
    token = (char *)malloc(sizeof(char) * 50);
    char gramLine[500];
    char delims[] = " \t\r\n\v\f";

    while (fgets(gramLine, 500, fpfollow) != NULL)
    {
        token = strtok(gramLine, delims); // first element of the rule
        // pushing it to hashtable
        struct hash *lhs = searchTable(ht, token);
        token = strtok(NULL, delims);
        lhs->follow = (node)malloc(sizeof(struct Node));
        struct Node *temp = lhs->follow;
        while (token != NULL)
        {
            temp->string = (char *)malloc(sizeof(char) * 50);
            temp->flag = 1;
            strcpy(temp->string, token);
            token = strtok(NULL, delims);
            if (token != NULL)
            {
                temp->next = (node)malloc(sizeof(struct Node));
                temp = temp->next;
            }
        }
        temp = NULL;
    }
    rewind(fpfollow);
    fclose(fpfollow);
}
void printrule(node ls){

	node temp=ls;
	while(temp!=NULL){
		printf("%s %d\t",temp->string, temp->flag );
		
		temp=temp->next;
	}
	printf("\n");

return ;


}

void printRuleNo(struct hash h){
	for(int i=0;i<h.rulenum;i++){
		printf("%d \n", h.rule[i]);
	}
}

int main()
{
    hashtable rules;
    rules = createHashTable();
    node *reading = readGrammar(rules);
    first(rules,"/Users/pragyansingh/Downloads/first.txt");
    follow(rules,"/Users/pragyansingh/Downloads/Follow_set.txt");
    printRuleNo(*rules[2]);
    return 0;
}