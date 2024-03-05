#include "parser.h"
#include <ctype.h>
int stack_max = 10000;
int parserError = 0;
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

char *getCorrespondingString(int f)
{
    switch (f)
    {
    case 1:
        return "TK_ASSIGNOP";
    case 2:
        return "TK_COMMENT";
    case 3:
        return "TK_FIELDID";
    case 4:
        return "TK_ID";
    case 5:
        return "TK_NUM";
    case 6:
        return "TK_RNUM";
    case 7:
        return "TK_FUNID";
    case 8:
        return "TK_RUID";
    case 9:
        return "TK_WITH";
    case 10:
        return "TK_PARAMETERS";
    case 11:
        return "TK_END";
    case 12:
        return "TK_WHILE";
    case 13:
        return "TK_UNION";
    case 14:
        return "TK_ENDUNION";
    case 15:
        return "TK_DEFINETYPE";
    case 16:
        return "TK_AS";
    case 17:
        return "TK_TYPE";
    case 18:
        return "TK_MAIN";
    case 19:
        return "TK_GLOBAL";
    case 20:
        return "TK_PARAMETER";
    case 21:
        return "TK_LIST";
    case 22:
        return "TK_SQL";
    case 23:
        return "TK_SQR";
    case 24:
        return "TK_INPUT";
    case 25:
        return "TK_OUTPUT";
    case 26:
        return "TK_INT";
    case 27:
        return "TK_REAL";
    case 28:
        return "TK_COMMA";
    case 29:
        return "TK_SEM";
    case 30:
        return "TK_COLON";
    case 31:
        return "TK_DOT";
    case 32:
        return "TK_ENDWHILE";
    case 33:
        return "TK_OP";
    case 34:
        return "TK_CL";
    case 35:
        return "TK_IF";
    case 36:
        return "TK_THEN";
    case 37:
        return "TK_ENDIF";
    case 38:
        return "TK_READ";
    case 39:
        return "TK_WRITE";
    case 40:
        return "TK_RETURN";
    case 41:
        return "TK_PLUS";
    case 42:
        return "TK_MINUS";
    case 43:
        return "TK_MUL";
    case 44:
        return "TK_DIV";
    case 45:
        return "TK_CALL";
    case 46:
        return "TK_RECORD";
    case 47:
        return "TK_ENDRECORD";
    case 48:
        return "TK_ELSE";
    case 49:
        return "TK_AND";
    case 50:
        return "TK_OR";
    case 51:
        return "TK_NOT";
    case 52:
        return "TK_LT";
    case 53:
        return "TK_LE";
    case 54:
        return "TK_EQ";
    case 55:
        return "TK_GT";
    case 56:
        return "TK_GE";
    case 57:
        return "TK_NE";
    case 58:
        return "$";

    case 100:
        return "program";
    case 101:
        return "inputparameters";
    case 102:
        return "typedefinitions";
    case 103:
        return "iterativestmt";
    case 104:
        return "singleorrecid";
    case 105:
        return "lowprecedenceoperators";
    case 106:
        return "stmts";
    case 107:
        return "factor";
    case 108:
        return "optionalreturn";
    case 109:
        return "operator";
    case 110:
        return "declaration";
    case 111:
        return "fielddefinition";
    case 112:
        return "highprecedenceoperators";
    case 113:
        return "constructeddatatype";
    case 114:
        return "termprime";
    case 115:
        return "global_or_not";
    case 116:
        return "input_par";
    case 117:
        return "term";
    case 118:
        return "outputparameters";
    case 119:
        return "A";
    case 120:
        return "expprime";
    case 121:
        return "output_par";
    case 122:
        return "booleanexpression";
    case 123:
        return "morefields";
    case 124:
        return "iostmt";
    case 125:
        return "returnstmt";
    case 126:
        return "otherfunctions";
    case 127:
        return "parameter_list";
    case 128:
        return "stmt";
    case 129:
        return "definetypestmt";
    case 130:
        return "typedefinition";
    case 131:
        return "arithmeticexpression";
    case 132:
        return "funcallstmt";
    case 133:
        return "remaining_list";
    case 134:
        return "datatype";
    case 135:
        return "fielddefinitions";
    case 136:
        return "more_ids";
    case 137:
        return "var";
    case 138:
        return "highprecedenceoperator";
    case 139:
        return "highPrecedenceOperators";
    case 140:
        return "otherstmts";
    case 141:
        return "relationalop";
    case 142:
        return "function";
    case 143:
        return "mainfunction";
    case 144:
        return "logicalop";
    case 145:
        return "assignmentstmt";
    case 146:
        return "conditionalstmt";
    case 147:
        return "idlist";
    case 148:
        return "primitivedatatype";
    case 149:
        return "declarations";
    default:
    {

        return "Nothing";
    }
    }
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
treeNode createTreeNode(char *lexeme, int line, int id)
{
    treeNode node = (treeNode)malloc(sizeof(treeNode));
    node->lexeme = strdup(lexeme);
    node->line = line;
    node->id = id;
    node->child = NULL;
    node->next = NULL;
    node->parent = NULL;
    return node;
}

treeNode addChildRule(treeNode parent, node n, int line, char *lexeme)
{
    treeNode root = createTreeNode(n->string, line, getId(n->string));
    root->line = line;
    root->next = NULL;
    treeNode t1 = root;
    root->parent = parent;
    n = n->next;
    while (n != NULL)
    {
        t1->next = createTreeNode(n->string, line, getId(n->string));
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
void storeGrammar(const char *filename)
{
    FILE *file = fopen(filename, "r");
    if (!file)
    {
        printf("Failed to open the file\n");
        return;
    }
    char line[256];
    while (fgets(line, sizeof(line), file))
    {
        // Removing newline character if present
        line[strcspn(line, "\n")] = 0;

        // Reset production count for the current rule
        grammarRules[grammarRuleCount].prodCount = 0;

        // Splitting the line into non-terminal and productions directly, assuming " " is the delimiter
        char *restOfLine;
        char *nonTerminal = strtok_r(line, " ", &restOfLine);

        if (nonTerminal != NULL && grammarRuleCount < MAX_RULES)
        {
            strcpy(grammarRules[grammarRuleCount].nonTerminal, nonTerminal);

            // Assuming the rest of the line after the first space until newline is the production
            // and splitting it based on "|" delimiter for multiple productions
            if (restOfLine != NULL)
            {
                char *saveptr;
                char *prodToken = strtok_r(restOfLine, "|", &saveptr);
                while (prodToken != NULL && grammarRules[grammarRuleCount].prodCount < MAX_SYMBOLS)
                {
                    // Trim leading spaces for each production token
                    while (isspace((unsigned char)*prodToken))
                        prodToken++;
                    strcpy(grammarRules[grammarRuleCount].production[grammarRules[grammarRuleCount].prodCount++], prodToken);
                    prodToken = strtok_r(NULL, "|", &saveptr);
                }
            }
            grammarRuleCount++;
        }
    }
    fclose(file);
}
void printGrammar()
{
    printf("Grammar Rules:\n");
    for (int i = 0; i < grammarRuleCount; i++)
    {
        printf("%s", grammarRules[i].nonTerminal);
        for (int j = 0; j < grammarRules[i].prodCount; j++)
        {
            printf(" %s", grammarRules[i].production[j]);
            if (j < grammarRules[i].prodCount - 1)
            {
                printf(" |");
            }
        }
        printf("\n");
    }
}

int epsinFirst(char *nTermStr, hashtable h)
{
    Element e = searchTable(h, nTermStr); // Use searchTable to find the element for nTermStr
    if (e == NULL)
    {
        return 0; // Non-terminal not found in hashtable
    }
    Node *cur = e->first; // Access the FIRST set from the found element

    while (cur != NULL)
    {
        if (strcmp(cur->string, "eps") == 0)
        {
            return 1; // Found epsilon in FIRST set
        }
        cur = cur->next;
    }
    return 0; // Epsilon not found in FIRST set
}
void mergeLists(node list1, node list2)
{
    node l2 = list2;
    node cur;
    node prev;
    while (l2 != NULL)
    {
        cur = list1;
        int flag = 0;
        while (cur != NULL)
        {

            if (!strcmp(cur->string, l2->string))
            {
                flag = 1;
                break;
            }
            prev = cur;
            cur = cur->next;
        }
        if (flag == 0 && strcmp(l2->string, "eps") != 0)
        {
            prev->next = (node)malloc(sizeof(struct Node));
            prev->next->string = (char *)malloc(sizeof(char) * 40);
            strcpy(prev->next->string, l2->string);
            prev->next->flag = l2->flag;
            prev->next->next = NULL;
        }
        l2 = l2->next;
    }
}

Node *getAlpha(int ruleIndex, hashtable h)
{
    if (ruleIndex < 0 || ruleIndex >= grammarRuleCount)
    {
        return NULL; // Rule index out of bounds
    }

    Node *alpha = NULL, *last = NULL;

    for (int i = 0; i < grammarRules[ruleIndex].prodCount; ++i)
    {
        Node *newNode = (Node *)malloc(sizeof(Node));
        if (!newNode)
        {
            fprintf(stderr, "Memory allocation failed\n");
            exit(EXIT_FAILURE);
        }
        newNode->string = strdup(grammarRules[ruleIndex].production[i]);
        newNode->flag = IsTerminal(newNode->string) ? -1 : IsNonTerminal(newNode->string) ? 1
                                                                                          : 0; // Update flag based on terminal/non-terminal check
        newNode->next = NULL;

        if (alpha == NULL)
        {
            alpha = newNode; // First node in the list
        }
        else
        {
            last->next = newNode; // Append to the end
        }
        last = newNode; // Update last pointer
    }

    return alpha; // Return the head of the linked list
}
parseTable initParseTb(hashtable h)
{
    parseTable pt = (parseTable)malloc(sizeof(int *) * TOTAL_GRAMMAR_NONTERMINALS);
    for (int i = 0; i < TOTAL_GRAMMAR_NONTERMINALS; ++i)
    {
        pt[i] = (int *)malloc(sizeof(int) * TOTAL_GRAMMAR_TERMINALS);
        memset(pt[i], -1, sizeof(int) * TOTAL_GRAMMAR_TERMINALS); // Initialize all entries to -1
    }

    for (int ruleIndex = 0; ruleIndex < grammarRuleCount; ++ruleIndex)
    {

        int rowIndex = getRow(grammarRules[ruleIndex].nonTerminal);
        for (int p = 0; p < grammarRules[ruleIndex].prodCount; ++p)
        {
            char *symbol = grammarRules[ruleIndex].production[p];
            int colIndex = getCol(symbol);

            if (colIndex != -1)
            { // If it's a terminal or ε
                pt[rowIndex][colIndex] = ruleIndex;
                printf("%d", pt[rowIndex][colIndex]);
            }
            else
            { // Handle non-terminal symbols
                // Assuming getFirstSet returns an array of flags indicating if ε is in the FIRST set
                int hasEpsilon = epsinFirst(symbol, h);
                if (hasEpsilon)
                {
                    // Now we handle the FOLLOW set of the non-terminal
                    Element e = searchTable(h, grammarRules[ruleIndex].nonTerminal);
                    Node *followSet = e->follow;
                    while (followSet != NULL)
                    {
                        colIndex = getCol(followSet->string);
                        if (colIndex != -1)
                        {
                            pt[rowIndex][colIndex] = ruleIndex; // Use the rule for all symbols in the FOLLOW set
                        }
                        followSet = followSet->next;
                    }
                }
            }
        }
    }

    return pt;
}

// Main function to parse the code.
treeNode parseCode(parseTable pt, GrammarRule *grammarRules, Token *curTk, hashtable h, int *error)
{
    stack parseStack = initStack();
    *error = 0;

    // Push the start symbol and the end marker onto the stack.
    push(parseStack, getRow("program")); // Assuming "program" is the start symbol's row in pt.
    push(parseStack, -1);                // End marker.

    treeNode root = createTreeNode("program", curTk->lineNo, 0); // Starting with the root node.
    treeNode currentNode = root;

    curTk = getNextToken();

    while (!isEmpty(parseStack) && curTk != NULL)
    {
        int stackTop = top(parseStack);

        if (stackTop == -1)
        { // End marker reached.
            if (curTk->tokenName == TK_END)
            { // Assuming TK_EOF is the end-of-file token ID.
                break;
            }
            else
            {
                printf("Error: Extra tokens found after program's end.\n");
                *error = 1;
                break;
            }
        }

        if (stackTop == getCol(curTk->lexeme))
        { // Terminal matches the token.
            pop(parseStack);
            // Moving "currentNode" to its next sibling or up to the parent as needed here.
            treeNode newTerminalNode = createTreeNode(curTk->lexeme, curTk->lineNo, curTk->tokenName);
            if (currentNode->child == NULL)
            {
                currentNode->child = newTerminalNode;
            }
            else
            {
                treeNode sibling = currentNode->child;
                while (sibling->next != NULL)
                {
                    sibling = sibling->next;
                }
                sibling->next = newTerminalNode;
            }
            newTerminalNode->parent = currentNode; // Linking the new node to the tree.
            curTk = getNextToken();                // Move to the next token.
        }
        else if (stackTop >= 0 && stackTop < TOTAL_GRAMMAR_NONTERMINALS)
        { // Non-terminal
            int ruleIndex = pt[stackTop][getCol(curTk->lexeme)];
            if (ruleIndex != -1)
            {
                pop(parseStack); // Pop the non-terminal.
                GrammarRule selectedRule = grammarRules[ruleIndex];

                for (int i = selectedRule.prodCount - 1; i >= 0; --i)
                {
                    int symbolIndex = getCol(selectedRule.production[i]); // Mapping symbol to column index.
                    push(parseStack, symbolIndex);                        // Pushing symbols onto the stack in reverse order.
                }

                // Assuming we are adding children nodes for the non-terminal's production.
                treeNode newNode = createTreeNode(selectedRule.nonTerminal, curTk->lineNo, ruleIndex);
                if (currentNode->child == NULL)
                {
                    currentNode->child = newNode;
                }
                else
                {
                    treeNode temp = currentNode->child;
                    while (temp->next != NULL)
                    {
                        temp = temp->next;
                    }
                    temp->next = newNode; // Adding the new node as the next sibling.
                }
                newNode->parent = currentNode; // Setting the parent of the new node.
                currentNode = newNode;         // Moving to the newly created node to continue building the tree.
            }
            else
            {
                printf("Syntax Error: Unexpected token '%s'.\n", curTk->lexeme);
                *error = 1;
                break;
            }
        }
        // Additional logic to handle moving back up the tree when a production is fully processed.
    }

    if (*error)
    {
        // Cleanup on error.
        return NULL;
    }

    return root; // Return the constructed parse tree.
}
void printParseTree(treeNode node, int depth);

// Function to print the parse tree
void printParseTree(treeNode node, int depth)
{
    if (node == NULL)
        return;

    // Print the current node with indentation based on its depth in the tree
    for (int i = 0; i < depth; ++i)
        printf("  "); // Indentation
    printf("%s\n", node->lexeme);

    // Recursively print the first child
    if (node->child != NULL)
    {
        printParseTree(node->child, depth + 1);
    }

    // Recursively print the next sibling (but at the same depth)
    if (node->next != NULL)
    {
        printParseTree(node->next, depth);
    }
}
int calculateTreeDepth(treeNode root)
{
    if (root == NULL)
    {
        return 0; // The depth of an empty tree is 0.
    }
    int maxDepth = 0;
    treeNode child = root->child;
    while (child != NULL)
    {
        int childDepth = calculateTreeDepth(child);
        if (childDepth > maxDepth)
        {
            maxDepth = childDepth; // Update maxDepth if the current child's depth is greater.
        }
        child = child->next; // Move to the next sibling.
    }
    return maxDepth + 1; // Add 1 to include the root node in the depth count.
}
int main()
{
    storeGrammar("grammar.txt");
    Hashtable h = createHashTable();
    parseTable pt = initParseTb(h);
    int *error = 0;
    treeNode root = parseCode(pt, grammarRules, getNextToken(), h, error);
    printParseTree(root, calculateTreeDepth(root));
    return 0;
}