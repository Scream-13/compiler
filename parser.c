#include "parserDef.h"

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

Hashtable createHashTable()
{
    Hashtable ht = (Hashtable)malloc(HASH_SIZE * sizeof(Element));
    for (int i = 0; i < HASH_SIZE; i++)
    {
        ht[i] = NULL;
    }
    return ht;
}

int hashcode(char *str)
{
    unsigned long hash = 5381;
    int c;
    while ((c = *str++))
    {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

Element createElement(char *str)
{
    Element newElement = (Element)malloc(sizeof(Hash));
    newElement->value = strdup(str);
    newElement->first = NULL;
    newElement->follow = NULL;
    newElement->rule = NULL;
    newElement->rulenum = 0;
    newElement->flag = IsTerminal(str) ? 1 : IsNonTerminal(str) ? -1
                                                                : 0;
    newElement->next = NULL;
    return newElement;
}

void insertToHashTable(Hashtable ht, char *str)
{
    int index = hashcode(str);
    Element e = createElement(str);
    if (!ht[index])
    {
        ht[index] = e;
    }
    else
    {
        Element temp = ht[index];
        while (temp->next)
        {
            temp = temp->next;
        }
        temp->next = e;
    }
}

Element searchTable(Hashtable ht, char *str)
{
    int index = hashcode(str);
    Element e = ht[index];
    while (e && strcmp(e->value, str) != 0)
    {
        e = e->next;
    }
    return e;
}

void addFirstFollow(Node **list, char *str, int flag)
{
    Node *newNode = (Node *)malloc(sizeof(Node));
    newNode->string = strdup(str);
    newNode->flag = flag;
    newNode->next = NULL;
    if (*list == NULL)
    {
        *list = newNode;
    }
    else
    {
        Node *temp = *list;
        while (temp->next != NULL)
        {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Define readGrammar, readFirsts, readFollows, and printHashTable similar to previous descriptions

void freeHashTable(Hashtable ht)
{
    for (int i = 0; i < HASH_SIZE; i++)
    {
        Element e = ht[i];
        while (e)
        {
            Element toDelete = e;
            e = e->next;
            free(toDelete->value);
            Node *n = toDelete->first;
            while (n)
            {
                Node *toDeleteN = n;
                n = n->next;
                free(toDeleteN->string);
                free(toDeleteN);
            }
            n = toDelete->follow;
            while (n)
            {
                Node *toDeleteN = n;
                n = n->next;
                free(toDeleteN->string);
                free(toDeleteN);
            }
            free(toDelete);
        }
    }
    free(ht);
}
void readGrammar(Hashtable ht, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {
        char *token = strtok(line, " \n");
        char *lhs = token; // Left-hand side of the grammar rule
        if (!searchTable(ht, lhs))
        {
            insertToHashTable(ht, lhs);
        }
        Element e = searchTable(ht, lhs);
        while ((token = strtok(NULL, " \n")))
        {
            addFirstFollow(&(e->first), token, IsTerminal(token) ? 1 : -1);
        }
    }
    fclose(fp);
}
void freeNodeList(Node *head)
{
    while (head != NULL)
    {
        Node *temp = head;
        head = head->next;
        free(temp->string); // Free the string
        free(temp);         // Then free the node
    }
}
void readFirsts(Hashtable ht, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    if (!fp)
    {
        printf("Unable to open the first set file.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {
        // Tokenize the first non-terminal
        char *token = strtok(line, " \t\r\n");
        if (!token)
            continue;

        Element e = searchTable(ht, token);
        if (!e)
        { // If not found, create a new element
            insertToHashTable(ht, token);
            e = searchTable(ht, token);
        }
        else
        {
            // Clear existing first set to avoid adding extra tokens
            freeNodeList(e->first);
            e->first = NULL; // Reset the first list
        }

        // Process the rest of the tokens in the line as firsts
        while ((token = strtok(NULL, " \t\r\n")))
        {
            if (strcmp(token, "eps") == 0)
            {
                addFirstFollow(&(e->first), token, 0); // epsilon has a flag of 0
            }
            else
            {
                addFirstFollow(&(e->first), token, IsTerminal(token) ? 1 : -1);
            }
        }
    }
    fclose(fp);
}

void readFollows(Hashtable ht, const char *filename)
{
    FILE *fp = fopen(filename, "r");
    char line[1024];
    while (fgets(line, sizeof(line), fp))
    {
        char *token = strtok(line, " \n");
        Element e = searchTable(ht, token);
        if (!e)
        {
            insertToHashTable(ht, token);
            e = searchTable(ht, token);
        }
        while ((token = strtok(NULL, " \n")))
        {
            addFirstFollow(&(e->follow), token, 1); // All follows are terminals
        }
    }
    fclose(fp);
}

void printHashTable(Hashtable ht)
{
    for (int i = 0; i < HASH_SIZE; ++i)
    {
        for (Element elem = ht[i]; elem != NULL; elem = elem->next)
        {
            printf("Key: %s, Flag: %d\n", elem->value, elem->flag);
            printf("  Firsts:");
            for (Node *n = elem->first; n != NULL; n = n->next)
            {
                printf(" %s", n->string);
            }
            printf("\n  Follows:");
            for (Node *n = elem->follow; n != NULL; n = n->next)
            {
                printf(" %s", n->string);
            }
            printf("\n");
        }
    }
}

//*********TREE CODE BELOW*************

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
    Hashtable ht = createHashTable();
    // Example function calls
    // freeHashTable(ht```c
    // Assume appropriate paths for your grammar, firsts, and follows text files
    readGrammar(ht, "grammar.txt");
    readFirsts(ht, "first.txt");
    readFollows(ht, "follow_set.txt");
    printHashTable(ht);
    freeHashTable(ht); // Clean up all allocated memory
    return 0;
}