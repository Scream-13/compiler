#include <stdlib.h>
#include<stdio.h>
#define GramRules 87
#define MaxLenRule 125

char ** Loading_gram(){   
    char** rules;
    rules = (char**) malloc(GramRules * sizeof(char*));
    for (int i = 0; i < GramRules; i++)
        rules[i] = (char*)malloc(MaxLenRule * sizeof(char));
    FILE *fp;
    fp = fopen("/Users/pragyansingh/Desktop/grammar.txt", "r");
    if(fp==NULL){
        printf("error in opening file\n");
    }
    int counter=0;
    while(!feof(fp) && !ferror(fp)){
        if(fgets(rules[counter],MaxLenRule,fp)!=NULL){
            counter++;
        }
    }
    for(int i =0; i<GramRules; i++)
        printf("%d %s", i,rules[i]);
    return rules;
}
void freeMem(char** arr ){
    free(*arr);
    free(arr);
}



int main(){
    char** rules = Loading_gram();
    freeMem(rules);
    return 0;
}


char* Terminal_tokens[] = {
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
    "TK_NE"   
};
char* Non_Terminal_tokens[] = {
'typedefinition', 
'tk_id', 'tk_num', 
'tk_gt', 'tk_endunion', 
'tk_dot', 'tk_endrecord', 
'tk_global', 'tk_endwhile', 
'term', 'outputparameters', 
'operator', 'input_par', 
'tk_sem', 'a', 'tk_input', 
'tk_list', 'tk_sql', 'factor', 
'function', 'output_par', 'tk_real', 
'tk_rnum', 'tk_op', 'iterativestmt', 
'tk_and', 'datatype', 'program', 
'tk_output', 'tk_write', 
'parameter_list', 'tk_mul', 
'returnstmt', 'logicalop', 
'more_ids', 'termprime', 
'declarations', 'mainfunction', 
'fielddefinitions', 'tk_record', 
'funcallstmt', 'lowprecedenceoperators', 
'tk_not', 'tk_parameter', 'tk_ruid', 
'otherfunctions', 'tk_type', 
'constructeddatatype', 'tk_fieldid', 
'tk_sqr', 'tk_cl', 'fielddefinition', 
'optionalreturn', 'tk_definetype', 
'expprime', 'tk_then', 'idlist', 
'tk_with', 'relationalop', 
'tk_ge', 'primitivedatatype', 
'tk_main', 'tk_colon', 'tk_call', 
'singleorrecid', 'tk_end', 
'booleanexpression', 'tk_minus', 
'tk_read', 'iostmt', 'remaining_list',
 'tk_parameters', 'tk_endif', 
'otherstmts', 'arithmeticexpression',
'var', 'eps', 'tk_ne',
'declaration', 'assignmentstmt', 
'tk_lt', 'typedefinitions', 
'tk_eq', 'tk_plus', 'tk_as', 
'inputparameters', 'tk_or', 
'conditionalstmt', 'tk_return', 
'tk_int', 'highprecedenceoperator', 
'definetypestmt', 'morefields', 
'tk_funid', 'tk_comma', 'stmt', 
'stmts', 'tk_le', 'tk_if', 
'tk_assignop', 'tk_while', 
'highprecedenceoperators', 'tk_div',
'tk_else', 'global_or_not', 'tk_union'
};
