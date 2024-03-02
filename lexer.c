#include <stdio.h>
#include <stdlib.h>
#include "lexer.h"

int bufsize = 1000;
char *begin = NULL;
char *fwd = NULL;
char *twinBuf[2];
int currBuf = 1;
int state = 0;
int lines = 1;
int noInputLeft = 0;
FILE *fp;
symTb *tokenList;
keywordToTk getTkName[27];

void initGetTkName()
{
	getTkName[0].keyword = "with";
	getTkName[0].tk = TK_WITH;
	getTkName[1].keyword = "parameters";
	getTkName[1].tk = TK_PARAMETERS;
	getTkName[2].keyword = "end";
	getTkName[2].tk = TK_END;
	getTkName[3].keyword = "while";
	getTkName[3].tk = TK_WHILE;
	getTkName[4].keyword = "union";
	getTkName[4].tk = TK_UNION;
	getTkName[5].keyword = "endunion";
	getTkName[5].tk = TK_ENDUNION;
	getTkName[6].keyword = "definetype";
	getTkName[6].tk = TK_DEFINETYPE;
	getTkName[7].keyword = "as";
	getTkName[7].tk = TK_AS;
	getTkName[8].keyword = "type";
	getTkName[8].tk = TK_TYPE;
	getTkName[9].keyword = "global";
	getTkName[9].tk = TK_GLOBAL;
	getTkName[10].keyword = "parameter";
	getTkName[10].tk = TK_PARAMETER;
	getTkName[11].keyword = "list";
	getTkName[11].tk = TK_LIST;
	getTkName[12].keyword = "input";
	getTkName[12].tk = TK_INPUT;
	getTkName[13].keyword = "output";
	getTkName[13].tk = TK_OUTPUT;
	getTkName[14].keyword = "int";
	getTkName[14].tk = TK_INT;
	getTkName[15].keyword = "real";
	getTkName[15].tk = TK_REAL;
	getTkName[16].keyword = "endwhile";
	getTkName[16].tk = TK_ENDWHILE;
	getTkName[17].keyword = "if";
	getTkName[17].tk = TK_IF;
	getTkName[18].keyword = "then";
	getTkName[18].tk = TK_THEN;
	getTkName[19].keyword = "endif";
	getTkName[19].tk = TK_ENDIF;
	getTkName[20].keyword = "read";
	getTkName[20].tk = TK_READ;
	getTkName[21].keyword = "write";
	getTkName[21].tk = TK_WRITE;
	getTkName[22].keyword = "return";
	getTkName[22].tk = TK_RETURN;
	getTkName[23].keyword = "call";
	getTkName[23].tk = TK_CALL;
	getTkName[24].keyword = "record";
	getTkName[24].tk = TK_RECORD;
	getTkName[25].keyword = "endrecord";
	getTkName[25].tk = TK_ENDRECORD;
	getTkName[26].keyword = "else";
	getTkName[26].tk = TK_ELSE;
}

FILE *getStream()
{
	if (noInputLeft != 0)
		return NULL;
	currBuf = 1 - currBuf;
	memset(twinBuf[currBuf], EOF, bufsize);
	int count;
	if (!feof(fp))
		count = fread(twinBuf[currBuf], 1, bufsize, fp);

	if (count < bufsize)
	{
		twinBuf[currBuf][count] = '\0';
		noInputLeft = 1;
	}
	if (count < 0)
	{
		printf("Error in reading");
		return NULL;
	}
	return fp;
}

void initializeLexer(FILE *f)
{
	fp = f;
	currBuf = 1;
	state = 0;
	lines = 1;
	noInputLeft = 0;
	begin = NULL;
	fwd = NULL;
	twinBuf[0] = (char *)malloc(bufsize * sizeof(char));
	twinBuf[1] = (char *)malloc(bufsize * sizeof(char));
	return;
}

char getChar()
{
	if (begin == NULL && fwd == NULL)
	{
		getStream();
		begin = twinBuf[currBuf];
		fwd = twinBuf[currBuf];
		char res = *fwd;
		if (*fwd == '\n')
			lines++;
		fwd++;
		return res;
	}
	else if (fwd - begin == bufsize - 1)
	{
		getStream(fp);
		fwd = twinBuf[currBuf];
	}
	else
		fwd++;
	if (*fwd == '\n')
		lines++;
	return *fwd;
}

void tokenized()
{
	fwd++;
	begin = fwd;
	fwd--;
}

char *makeLexeme(char *first, char *last)
{
	char *lexeme = (char *)malloc((last - first + 2) * sizeof(char));
	char *curr = first;
	int i;
	for (i = 0; curr <= last; i++)
	{
		lexeme[i] = *curr;
		curr++;
	}
	lexeme[i] = '\0';
	return lexeme;
}

Token *makeToken(TokenName tokenName, char *lexeme, int lineNo, int isNum, number *number)
{
	Token *newTk = (Token *)malloc(sizeof(Token));
	newTk->lexeme = lexeme;
	newTk->isNum = isNum;
	newTk->lineNo = lineNo;
	newTk->tokenName = tokenName;
	newTk->number = number;
	return newTk;
}

int compareString(char *c1, char *c2)
{
	int i = 0;
	for (; c1[i] != '\0' && c2[i] != '\0'; i++)
	{
		if (c1[i] != c2[i])
			return 0;
	}
	if (c1[i] != '\0' || c2[i] != '\0')
		return 0;
	return 1;
}

void retract(int r)
{
	while (r--)
	{
		if (*fwd == '\n')
			lines--;
		fwd--;
	}
}

Token *normalReturn(TokenName tokenName)
{
	retract(1);
	char *lexeme = makeLexeme(begin, fwd);
	Token *token = makeToken(tokenName, lexeme, lines, 0, NULL);
	tokenized();
	return token;
}

symTb *initializeSymTb()
{
	tokenList = (symTb *)malloc(sizeof(symTb));
	if (tokenList != NULL)
	{
		tokenList->count = 0;
		tokenList->size = 50;
		tokenList->tokens = (Token **)malloc(sizeof(Token) * 50);
		if (NULL == tokenList->tokens)
		{
			free(tokenList);
			return NULL;
		}
	}
	return tokenList;
}

void deleteSymTb()
{
	free(tokenList->tokens);
	free(tokenList);
}

void addToSymTb(Token *tk)
{
	int count = tokenList->count;
	int size = tokenList->size;
	if (count >= size)
	{
		size = size + 50;
		tokenList->tokens = (Token **)realloc(tokenList->tokens, sizeof(Token **) * size);
		if (tokenList->tokens == NULL)
			return;
		else
		{
			tokenList->size = size;
		}
	}
	tokenList->tokens[count] = tk;
	count++;
	tokenList->count = count;
}

Token *getNextToken()
{
	state = 0;
	char c = 10;
	while (1)
	{
		char c = getChar();
		switch (state)
		{
		case 0:
		{
			if (c == '<')
				state = 1;
			else if (c >= 48 && c <= 57)
				state = 7;
			else if (c == '_')
				state = 18;
			else if (c == 98 || c == 99 || c == 100)
				state = 22;
			else if (c == 97 || (c >= 100 && c <= 122))
				state = 26;
			else if (c == '%')
				state = 54;
			else if (c == '#')
				state = 56;
			else if (c == '[')
				state = 28;
			else if (c == ']')
				state = 29;
			else if (c == ',')
				state = 30;
			else if (c == ';')
				state = 31;
			else if (c == '.')
				state = 32;
			else if (c == '(')
				state = 33;
			else if (c == ')')
				state = 34;
			else if (c == '+')
				state = 35;
			else if (c == '-')
				state = 36;
			else if (c == '*')
				state = 37;
			else if (c == '/')
				state = 38;
			else if (c == '&')
				state = 39;
			else if (c == '@')
				state = 42;
			else if (c == '~')
				state = 45;
			else if (c == '=')
				state = 46;
			else if (c == ':')
				state = 48;
			else if (c == '!')
				state = 49;
			else if (c == '>')
				state = 51;
			else if (c == 32 || c == '\t' || c == '\v' || c == '\r' || c == '\f')
			{
				begin++;
				state = 0;
			}
			else if (c == '\n')
			{
				begin++;
				state = 0;
			}
			else if (c == 0 || c == EOF)
			{
				return NULL;
			}
			else
			{
				printf("Unrecognizable character %c at line %d\n", c, lines);
				state = 65;
			}
			break;
		}
		case 1:
		{
			if (c == '-')
				state = 2;
			else if (c == '=')
				state = 6;
			else
				state = 5;
			break;
		}
		case 2:
		{
			if (c == '-')
				state = 3;
			else
				state = 59;
			break;
		}
		case 3:
		{
			if (c == '-')
				state = 4;
			else
				state = 60;
			break;
		}
		case 4:
			return normalReturn(TK_ASSIGNOP);
		case 5:
		{
			retract(1);
			return normalReturn(TK_LT);
		}
		case 6:
			return normalReturn(TK_LE);
		case 7:
		{
			if (c >= 48 && c <= 57)
				state = 7;
			else if (c == '.')
				state = 9;
			else
				state = 8;
			break;
		}
		case 8:
		{
			retract(2);
			char *lexeme = makeLexeme(begin, fwd);
			int num = 0, i = 0;
			for (; lexeme[i] != '\0'; i++)
			{
				num = 10 * num + lexeme[i] - '0';
			}
			number *intPartOfNum = (number *)malloc(sizeof(number));
			(*intPartOfNum).NUM = num;
			Token *token = makeToken(TK_NUM, lexeme, lines, 1, intPartOfNum);
			tokenized();
			return token;
		}
		case 9:
		{
			if (c >= 48 && c <= 57)
				state = 10;
			else
				state = 61;
			break;
		}
		case 10:
		{
			if (c >= 48 && c <= 57)
				state = 11;
			else
				state = 64;
			break;
		}
		case 11:
		{
			if (c == 'E')
				state = 13;
			else
				state = 12;
			break;
		}
		case 12:
		{
			retract(2);
			char *lexeme = makeLexeme(begin, fwd);
			float num = 0.0;
			int i = 0;
			for (; lexeme[i] != '.'; i++)
			{
				num = 10 * num + lexeme[i] - '0';
			}
			i++;
			num = lexeme[i] / 10 + lexeme[i + 1] / 100;
			number *floatPartOfNum = (number *)malloc(sizeof(number));
			(*floatPartOfNum).RNUM = num;
			Token *token = makeToken(TK_NUM, lexeme, lines, 1, floatPartOfNum);
			tokenized();
			return token;
		}

		case 13:
		{
			if (c == '+' || c == '-')
				state = 14;
			else if (c >= 48 && c <= 57)
				state = 15;
			else
				state = 64;
			break;
		}
		case 14:
		{
			if (c >= 48 && c <= 57)
				state = 15;
			else
				state = 64;
			break;
		}
		case 15:
		{
			if (c >= 48 && c <= 57)
				state = 16;
			else
				state = 64;
			break;
		}
		case 16:
		{
			// tokenise RNUM with exponent part
			retract(2);
			char *lexeme = makeLexeme(begin, fwd);
			float f;
			sscanf(lexeme, "%f", &f);
			number *floatPartOfNum = (number *)malloc(sizeof(number));
			(*floatPartOfNum).RNUM = f;
			Token *token = makeToken(TK_NUM, lexeme, lines, 1, floatPartOfNum);
			tokenized();
			return token;
			break;
		}
		case 18:
		{
			if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
				state = 19;
			else
				state = 64;
			break;
		}
		case 19:
		{
			if (c >= 48 && c <= 57)
				state = 20;
			else if ((c >= 97 && c <= 122) || (c >= 65 && c <= 90))
				state = 19;
			else
				state = 21; // check for _main
			break;
		}
		case 20:
		{
			if (c >= 48 && c <= 57)
				state = 20;
			else
				state = 21;
			break;
		}
		case 21:
		{
			retract(2);
			char *lexeme = makeLexeme(begin, fwd);
			char tkmain[] = "_main";
			Token *token = (Token *)malloc(sizeof(Token *));
			if (compareString(tkmain, lexeme) == 1)
				token = makeToken(TK_MAIN, lexeme, lines, 0, NULL);
			else
				token = makeToken(TK_FUNID, lexeme, lines, 0, NULL);
			tokenized();
			return token;
		}
		case 22:
		{
			if (c >= 50 && c <= 55)
				state = 23;
			else if (c >= 97 && c <= 122)
				state = 26;
			else
				state = 63;
			break;
		}
		case 23:
		{
			if (c == 'b' || c == 'c' || c == 'd')
				state = 23;
			else if (c >= 50 && c <= 55)
				state = 24;
			else
				state = 25;
			break;
		}
		case 24:
		{
			if (c >= 50 && c <= 55)
				state = 24;
			else
				state = 25;
			break;
		}
		case 25:
		{
			retract(2);
			int l = fwd - begin + 1;
			if (l > 20)
			{
				state = 66;
				break;
			}
			else
			{
				char *lexeme = makeLexeme(begin, fwd);
				Token *token = makeToken(TK_ID, lexeme, lines, 0, NULL);
				tokenized();
				return token;
			}
		}
		case 26:
		{
			if (c >= 97 && c <= 122)
				state = 26;
			else
				state = 27;
			break;
		}
		case 27:
		{
			retract(1);
			retract(1);
			char *lexeme = makeLexeme(begin, fwd);
			for (int i = 0; i < 27; i++)
			{
				if (compareString(getTkName[i].keyword, lexeme) == 1)
				{
					Token *token = makeToken(getTkName[i].tk, lexeme, lines, 0, NULL);
					tokenized();
					return token;
				}
			}
			Token *token = makeToken(TK_FIELDID, lexeme, lines, 0, NULL);
			tokenized();
			return token;
		}
		case 28:
			return normalReturn(TK_SQL);
		case 29:
			return normalReturn(TK_SQR);
		case 30:
			return normalReturn(TK_COMMA);
		case 31:
			return normalReturn(TK_SEM);
		case 32:
			return normalReturn(TK_DOT);
		case 33:
			return normalReturn(TK_OP);
		case 34:
			return normalReturn(TK_CL);
		case 35:
			return normalReturn(TK_PLUS);
		case 36:
			return normalReturn(TK_MINUS);
		case 37:
			return normalReturn(TK_MUL);
		case 38:
			return normalReturn(TK_DIV);
		case 39:
		{
			if (c == '&')
				state = 40;
			else
				state = 64;
			break;
		}
		case 40:
		{
			if (c == '&')
				state = 41;
			else
				state = 64;
			break;
		}
		case 41:
			return normalReturn(TK_AND);
		case 42:
		{
			if (c == '@')
				state = 43;
			else
				state = 64;
			break;
		}
		case 43:
		{
			if (c == '@')
				state = 44;
			else
				state = 64;
			break;
		}
		case 44:
			return normalReturn(TK_OR);
		case 45:
			return normalReturn(TK_NOT);
		case 46:
		{
			if (c == '=')
				state = 47;
			else
				state = 64;
			break;
		}
		case 47:
			return normalReturn(TK_EQ);
		case 48:
			return normalReturn(TK_COLON);
		case 49:
		{
			if (c == '=')
				state = 50;
			else
				state = 64;
			break;
		}
		case 50:
			return normalReturn(TK_NE);
		case 51:
		{
			if (c == '=')
				state = 53;
			else
				state = 52;
			break;
		}
		case 52:
		{
			retract(1);
			return normalReturn(TK_GT);
		}
		case 53:
			return normalReturn(TK_GE);
		case 54:
		{
			if (c != '\n')
				state = 54;
			else
				state = 55;
			break;
		}
		case 55:
		{
			retract(2);
			char *lexeme = makeLexeme(begin, begin);
			Token *token = makeToken(TK_COMMENT, lexeme, lines, 0, NULL);
			tokenized();
			return token;
		}
		case 56:
		{
			if (c >= 97 && c <= 122)
				state = 57;
			else
				state = 64;
			break;
		}
		case 57:
		{
			if (c >= 97 && c <= 122)
				state = 57;
			else
				state = 58;
			break;
		}
		case 58:
		{
			retract(1);
			return normalReturn(TK_RUID); // not checking for existence in symbol table
		}
		case 59:
		{
			retract(2);
			return normalReturn(TK_LT);
		}
		case 60:
		{
			retract(3);
			return normalReturn(TK_LT);
		}
		case 61:
		{
			retract(3);
			char *lexeme = makeLexeme(begin, fwd);
			int num = 0, i = 0;
			for (; lexeme[i] != '\0'; i++)
			{
				num = 10 * num + lexeme[i] - '0';
			}
			number *intPartOfNum = (number *)malloc(sizeof(number));
			(*intPartOfNum).NUM = num;
			Token *token = makeToken(TK_NUM, lexeme, lines, 1, intPartOfNum);
			tokenized();
			return token;
		}
			// case 62:
			// {
			// 	fwd--;
			// retract -= 1;
			// 	fwd--;
			// retract -= 1;
			// 	fwd--;
			// retract -= 1;
			// 	fwd--;
			// retract -= 1;
		// 	char *lexeme = makeLexeme(begin, fwd);
		// 	int num = 0, i = 0;
		// 	for (; lexeme[i] != '\0'; i++)
		// 	{
		// 		num = 10 * num + lexeme[i] - '0';
		// 	}
		// 	number *intPartOfNum = (number *)malloc(sizeof(number));
		// 	(*intPartOfNum).NUM = num;
		// 	Token *token = makeToken(TK_NUM, lexeme, lines, 1, intPartOfNum);
		// 	tokenized();
		// 	return token;
		// }
		case 63:
		{
			retract(2);
			return normalReturn(TK_FIELDID);
		}
		case 64:
		{
			retract(2);
			char *lexeme = makeLexeme(begin, fwd);
			printf("Unrecognizable pattern %s at line %d\n", lexeme, lines);
			fwd++;
			begin = fwd;
			fwd--;
			state = 0;
			break;
		}
		case 65:
		{
			retract(1);
			char *lexeme = makeLexeme(begin, fwd);
			fwd++;
			begin = fwd;
			fwd--;
			state = 0;
			break;
		}
		case 66:
		{
			printf("Error :Variable Identifier is longer than the prescribed length of 20 characters.\n");
			tokenized();
			state = 0;
			break;
		}
		}
	}
}

int main()
{
	FILE *fp1 = fopen("t1.txt", "r");
	initializeLexer(fp1);
	initializeSymTb();
	initGetTkName();
	Token *t = getNextToken();
	while (t != NULL)
	{
		// printf("lexeme - %s  tokenNumber - %d  lineNumber - %d\n\n", t->lexeme, t->tokenName, t->lineNo);
		addToSymTb(t);
		t = getNextToken();
	}
	for (int i = 0; i < tokenList->count; i++)
	{
		t = tokenList->tokens[i];
		printf("lexeme - %s  token - %s  lineNumber - %d\n\n", t->lexeme, Terminal_tokens[t->tokenName], t->lineNo);
	}
	fclose(fp1);
	return 0;
}