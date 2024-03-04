#include "parserDef.h"

hashtable createHashTable()
{
    hashtable ht = (hashtable)malloc(sizeof(hashtable) * 501);
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
    entry->first = (node)malloc(sizeof(node));
    entry->follow = (node)malloc(sizeof(node));
    entry->value = (char *)malloc(sizeof(char) * 100); // tree branch selection
    entry->rulenum = 0;
    entry->flag = 0;
    if (strcmp(str, "eps") == 0)
    {
        entry->flag = 0;
    }
    else if (IsTerminal(str))
    {
        entry->flag = 1;
    }
    else if (IsNonTerminal(str))
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
    char gramLine[400];
    char delims[] = " \t\r\n\v\f";
    int i = 0;
    while (fgets(gramLine, 400, fp) != NULL)
    {
        token = strtok(gramLine, delims); // first element of the rule
        node temp = (node)malloc(sizeof(node));
        temp->string = (char *)malloc(sizeof(char) * strlen(token));
        strcpy(temp->string, token);
        temp->flag = -1; // as first token of any rule will be a nonterminal

        // pushing it to hashtable
        element h = searchTable(ht, token);
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
        element lhs = searchTable(ht, token);
        token = strtok(NULL, delims);

        lhs->first = (node)malloc(sizeof(struct Node));
        node temp = lhs->first;
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
        element lhs = searchTable(ht, token);
        token = strtok(NULL, delims);
        lhs->follow = (node)malloc(sizeof(struct Node));
        node temp = lhs->follow;
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
void printrule(node ls)
{

    node temp = ls;
    while (temp != NULL)
    {
        printf("%s %d\t", temp->string, temp->flag);

        temp = temp->next;
    }
    printf("\n");

    return;
}

void printRuleNo(struct hash h)
{
    for (int i = 0; i < h.rulenum; i++)
    {
        printf("%d \n", h.rule[i]);
    }
}

stack initStack()
{
    stack s = (stack)malloc(sizeof(stack));
    s->arr = (int *)malloc(sizeof(int) * stack_max);
    s->top = -1;
    return s;
}

void push(stack s, int rule)
{
    s->top = s->top + 1;
    if (s->top == stack_max)
        printf("Stack size reached %d\n", s->top);
    s->arr[s->top] = rule;
}

int top(stack s)
{
    return s->arr[s->top];
}

int pop(stack s)
{
    int top = s->arr[s->top];
    s->top--;
    return top;
}

int isEmpty(stack s)
{
    return s->top == -1;
}

treeNode createTreeNode(char *lexeme, int id)
{
    treeNode newNode = (treeNode)malloc(sizeof(struct TreeNode));

    newNode->child = NULL;
    newNode->next = NULL;
    newNode->parent = NULL;
    newNode->prev = NULL;
    newNode->lexeme = (char *)malloc(sizeof(char) * 30);
    newNode->id = id;
    return newNode;
}

treeNode addChildRule(treeNode parent, node n, int line, char *lexeme)
{
    treeNode root = createTreeNode(n->string, getId(n->string), lexeme);
    root->line = line;
    root->next = NULL;
    treeNode t1 = root;
    root->parent = parent;
    n = n->next;
    while (n != NULL)
    {
        t1->next = createTreeNode(n->string, getId(n->string), lexeme);
        t1->next->line = line;
        t1->next->next = NULL;
        t1->next->parent = parent;
        n = n->next;
        t1 = t1->next;
    }
    return root;
}

int getCol(char *str)
{
    for (int i = 0; i < 58; i++)
    {
        if (strcmp(str, CI[i]) == 0)
            return i + 1;
    }
    return -1;
}
int getRow(char *str)
{
    for (int i = 0; i < 49; i++)
    {
        if (strcmp(str, Non_Terminal_tokens[i]) == 0)
            return i + 1;
    }
    return -1;
}

int getId(char *str)
{
    int i;
    if (str[0] >= 'A' && str[0] <= 'Z')
        i = getCol(str);
    else
        i = getRow(str) + 100;
    return i;
}

int isFollow(int C, int curTkName, hashtable h)
{

    node follow = h[hashcode(getCorrespondingString(C))]->follow;
    while (follow != NULL)
    {
        if (curTkName == getId(follow->string))
        {
            return 1;
        }
        follow = follow->next;
    }

    return 0;
}

treeNode parseCode(FILE *fp, parseTable parseTb, node *gramRule, Token *curTk, hashtable h, int *error)
{
    if (fp == NULL)
    {
        printf("Error with file\n");
        return NULL;
    }
    while (curTk->tokenName == TK_COMMENT)
        curTk = getNextToken();
    int curTkName = curTk->tokenName;
    stack s = initStack();
    push(s, 58);
    push(s, 100);

    treeNode root = createTreeNode("program", 100);
    treeNode cur = root;
    int C = top(s);

    do
    {
        if (C == curTkName)
        {
            int t = pop(s);
            cur->line = curTk->lineNo;
            strcpy(cur->lexeme, curTk->lexeme);
            curTk = getNextToken(fp);
            while (curTk->tokenName == TK_COMMENT)
                curTk = getNextToken();
            curTkName = curTk->tokenName;
            while (cur->next == NULL)
            {
                if (cur == root)
                    return root;
                cur = cur->parent;
            }
            cur = cur->next;
        }
        else if (C < 100)
        {
            pop(s);
            while (cur->next == NULL)
            {
                if (cur == root)
                    return root;
                cur = cur->parent;
            }
            cur = cur->next;
            *error = 1;
            parserError++;
            printf("Line No:%d The token %s for lexeme %s  does not match with the expected token %s\n", curTk->lineNo, getCorrespondingString(curTkName), curTk->lexeme, getCorrespondingString(C));
        }
        else if (parseTb[C % 100][curTkName] == -1)
        {
            while (parseTb[C % 100][curTkName] == -1)
            {
                curTk = getNextToken(fp);
                curTkName = curTk->tokenName;
                if (isFollow(C, curTkName, h) == 1)
                {
                    pop(s);
                    while (cur->next == NULL)
                    {
                        if (cur == root)
                            return root;
                        cur = cur->parent;
                    }
                    cur = cur->next;
                }
                parserError++;
                *error = 1;
                printf("Line No: %d Error: Invalid token %s encountered with value %s stack top %s\n", curTk->lineNo, getCorrespondingString(curTkName), curTk->lexeme, getCorrespondingString(C));
            }
        }
        else if (parseTb[C % 100][curTkName] == -2)
        {
            pop(s);
            while (cur->next == NULL)
            {
                if (cur == root)
                    return root;
                cur = cur->parent;
            }
            cur = cur->next;
            *error = 1;
            parserError++;
            printf("Line No: %d Error: Invalid token %s encountered with value %s stack top %s\n", curTk->lineNo, getCorrespondingString(curTkName), curTk->lexeme, getCorrespondingString(C));
        }
        else
        {
            int rule_id = parseTb[C % 100][curTkName];

            node rule = gramRule[rule_id];

            rule = rule->next;

            if (strcmp(rule->string, "eps") != 0)
            {
                cur->child = (treeNode)malloc(sizeof(struct TreeNode));
                cur->child = addChildRule(cur, rule, curTk->lineNo, curTk->lexeme);

                cur = cur->child;

                pop(s);

                stack tStack = createStack();

                node temp = rule;
                while (temp != NULL)
                {
                    int temp_id = getId(temp->string);
                    push(tStack, temp_id);
                    temp = temp->next;
                }
                while (!isEmpty(tStack))
                {
                    push(s, pop(tStack));
                }
            }

            else
            {
                while (cur->next == NULL)
                {
                    if (cur == root)
                    {
                        return root;
                    }
                    cur = cur->parent;
                }
                cur = cur->next;
                pop(s);
            }
        }
        C = getTop(s);
    } while (C != 54);
}

int main()
{
    hashtable rules = createHashTable();
    node *reading = readGrammar(rules);
    first(rules, "first.txt");
    follow(rules, "Follow_set.txt");
    printRuleNo(*rules[2]);
    return 0;
}