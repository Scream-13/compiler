#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_GRAMMAR_NONTERMINALS 49
#define TOTAL_GRAMMAR_TERMINALS 58
#define NOF_RULES 87
#define HASH_SIZE 1000

typedef struct Node {
    int flag; // terminal = 1, non-terminal = -1, eps = 0
    char* string;
    struct Node* next;
} Node;

typedef struct hash {
    Node* first; // stores firsts
    Node* follow; // stores follows
    int* rule; // number of rules on the rhs side
    int rulenum; // store rule number
    char* value; // non-terminal 
    int flag;
    struct hash* next; // Next element for collision handling
} Hash, *Element;

typedef Element* Hashtable;
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

Hashtable createHashTable() {
    Hashtable ht = (Hashtable)malloc(HASH_SIZE * sizeof(Element));
    for(int i = 0; i < HASH_SIZE; i++) {
        ht[i] = NULL;
    }
    return ht;
}

int hashcode(char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % HASH_SIZE;
}

Element createElement(char* str) {
    Element newElement = (Element)malloc(sizeof(Hash));
    newElement->value = strdup(str);
    newElement->first = NULL;
    newElement->follow = NULL;
    newElement->rule = NULL;
    newElement->rulenum = 0;
    newElement->flag = IsTerminal(str) ? 1 : IsNonTerminal(str) ? -1 : 0;
    newElement->next = NULL;
    return newElement;
}

void insertToHashTable(Hashtable ht, char* str) {
    int index = hashcode(str);
    Element e = createElement(str);
    if (!ht[index]) {
        ht[index] = e;
    } else {
        Element temp = ht[index];
        while (temp->next) {
            temp = temp->next;
        }
        temp->next = e;
    }
}

Element searchTable(Hashtable ht, char* str) {
    int index = hashcode(str);
    Element e = ht[index];
    while (e && strcmp(e->value, str) != 0) {
        e = e->next;
    }
    return e;
}

void addFirstFollow(Node** list, char* str, int flag) {
    Node* newNode = (Node*)malloc(sizeof(Node));
    newNode->string = strdup(str);
    newNode->flag = flag;
    newNode->next = NULL;
    if (*list == NULL) {
        *list = newNode;
    } else {
        Node* temp = *list;
        while (temp->next != NULL) {
            temp = temp->next;
        }
        temp->next = newNode;
    }
}

// Define readGrammar, readFirsts, readFollows, and printHashTable similar to previous descriptions

void freeHashTable(Hashtable ht) {
    for (int i = 0; i < HASH_SIZE; i++) {
        Element e = ht[i];
        while (e) {
            Element toDelete = e;
            e = e->next;
            free(toDelete->value);
            Node* n = toDelete->first;
            while (n) {
                Node* toDeleteN = n;
                n = n->next;
                free(toDeleteN->string);
                free(toDeleteN);
            }
            n = toDelete->follow;
            while (n) {
                Node* toDeleteN = n;
                n = n->next;
                free(toDeleteN->string);
                free(toDeleteN);
            }
            free(toDelete);
        }
    }
    free(ht);
}
void readGrammar(Hashtable ht, const char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char* token = strtok(line, " \n");
        char* lhs = token; // Left-hand side of the grammar rule
        if (!searchTable(ht, lhs)) {
            insertToHashTable(ht, lhs);
        }
        Element e = searchTable(ht, lhs);
        while ((token = strtok(NULL, " \n"))) {
            addFirstFollow(&(e->first), token, IsTerminal(token) ? 1 : -1);
        }
    }
    fclose(fp);
}
void freeNodeList(Node* head) {
    while (head != NULL) {
        Node* temp = head;
        head = head->next;
        free(temp->string); // Free the string
        free(temp); // Then free the node
    }
}
void readFirsts(Hashtable ht, const char* filename) {
    FILE* fp = fopen(filename, "r");
    if (!fp) {
        printf("Unable to open the first set file.\n");
        return;
    }

    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        // Tokenize the first non-terminal
        char* token = strtok(line, " \t\r\n");
        if (!token) continue;

        Element e = searchTable(ht, token);
        if (!e) { // If not found, create a new element
            insertToHashTable(ht, token);
            e = searchTable(ht, token);
        } else {
            // Clear existing first set to avoid adding extra tokens
            freeNodeList(e->first);
            e->first = NULL; // Reset the first list
        }

        // Process the rest of the tokens in the line as firsts
        while ((token = strtok(NULL, " \t\r\n"))) {
            if (strcmp(token, "eps") == 0) {
                addFirstFollow(&(e->first), token, 0); // epsilon has a flag of 0
            } else {
                addFirstFollow(&(e->first), token, IsTerminal(token) ? 1 : -1);
            }
        }
    }
    fclose(fp);
}

void readFollows(Hashtable ht, const char* filename) {
    FILE* fp = fopen(filename, "r");
    char line[1024];
    while (fgets(line, sizeof(line), fp)) {
        char* token = strtok(line, " \n");
        Element e = searchTable(ht, token);
        if (!e) {
            insertToHashTable(ht, token);
            e = searchTable(ht, token);
        }
        while ((token = strtok(NULL, " \n"))) {
            addFirstFollow(&(e->follow), token, 1); // All follows are terminals
        }
    }
    fclose(fp);
}

void printHashTable(Hashtable ht) {
    for (int i = 0; i < HASH_SIZE; ++i) {
        for (Element elem = ht[i]; elem != NULL; elem = elem->next) {
            printf("Key: %s, Flag: %d\n", elem->value, elem->flag);
            printf("  Firsts:");
            for (Node* n = elem->first; n != NULL; n = n->next) {
                printf(" %s", n->string);
            }
            printf("\n  Follows:");
            for (Node* n = elem->follow; n != NULL; n = n->next) {
                printf(" %s", n->string);
            }
            printf("\n");
        }
    }
}

int main() {
    Hashtable ht = createHashTable();
    // Example function calls
    // freeHashTable(ht```c
    // Assume appropriate paths for your grammar, firsts, and follows text files
    readGrammar(ht, "/Users/pragyansingh/Desktop/grammar.txt");
    readFirsts(ht, "/Users/pragyansingh/Downloads/first.txt");
    readFollows(ht, "/Users/pragyansingh/Desktop/follow_set.txt");
    printHashTable(ht);
    freeHashTable(ht); // Clean up all allocated memory
    return 0;
}