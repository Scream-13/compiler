#include "lexer.h"
int bufsize = 4096;
char *begin = NULL;
char *fwd = NULL;
char *twinBuf[2];
int currBuf = 1;
int state = 0;
int lines = 1;
int noInputLeft = 0;
int flag = 0;
FILE *fp;

 symTb *tokenList;
 keywordToTk getTkName[27];
char *Terminal_tk[] = {
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
	"TK_NE"};

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

FILE *getStream(FILE *f1)
{
	FILE *fp = fopen("t2.txt", "r");
	initializeLexer(fp);
	if (noInputLeft != 0)
		return NULL;
	currBuf = 1 - currBuf;
	memset(twinBuf[currBuf], EOF, bufsize);
	int count;
	if (!feof(f1))
		count = fread(twinBuf[currBuf], 1, bufsize, f1);

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
	return f1;
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

void initializeLexer(FILE *f)
{
	fp = f;
	state = 0;
	lines = 1;
	noInputLeft = 0;
	currBuf = 1;
	begin = NULL;
	fwd = NULL;
	twinBuf[0] = (char *)malloc(bufsize * sizeof(char));
	twinBuf[1] = (char *)malloc(bufsize * sizeof(char));
	initializeSymTb();
	initGetTkName();
	return;
}

char getChar(FILE *f1)
{
	if (begin == NULL && fwd == NULL)
	{
		getStream(f1);
		begin = twinBuf[currBuf];
		fwd = twinBuf[currBuf];
		char res = *fwd;
		if (*fwd == '\n')
			lines++;
		fwd++;
		return res;
	}
	else if (fwd == twinBuf[currBuf] + bufsize - 1)
	{
		getStream(f1);
		flag = 1;
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
	int c = 1 - currBuf;
	int i = 0;
	if (flag == 1 && first < twinBuf[c] + bufsize)
	{
		flag = 0;
		for (; curr < twinBuf[c] + bufsize; i++)
		{
			lexeme[i] = *curr;
			curr++;
		}
		for (curr = twinBuf[currBuf]; curr <= last; i++)
		{
			lexeme[i] = *curr;
			curr++;
		}
	}
	else
	{
		for (i = 0; curr <= last; i++)
		{
			lexeme[i] = *curr;
			curr++;
		}
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
		char c = getChar(fp);
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
			Token *token = makeToken(TK_RNUM, lexeme, lines, 1, floatPartOfNum);
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
			retract(1);
			char *lexeme = makeLexeme(begin, fwd);
			float num = 0.0;
			int i = 0;
			for (; lexeme[i] != '.'; i++)
			{
				num = 10 * num + lexeme[i] - '0';
			}
			i++;
			num = lexeme[i] / 10 + lexeme[i + 1] / 100;
			i += 3;
			int neg = 0;
			if (lexeme[i] == '-')
			{
				neg = 1;
				i++;
			}
			else if (lexeme[i] == '+')
				i++;
			int fac = 10, pow = 0;
			if (neg == 1)
				fac = 0.1;
			pow += lexeme[i] - '0';
			pow = 10 * pow + (lexeme[i + 1] - '0');
			for (int d = pow; d >= 0; d--)
			{
				num = num * fac;
			}
			number *floatPartOfNum = (number *)malloc(sizeof(number));
			floatPartOfNum->RNUM = num;
			Token *token = makeToken(TK_RNUM, lexeme, lines, 1, floatPartOfNum);
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

void removeComments(char *tc, char *clean)
{
	FILE *f1 = fopen(tc, "r");
	FILE *f2 = fopen(clean, "a");
	initializeLexer(f1);
	char c = 10;
	int st = 0;
	while (1)
	{
		c = getChar(f1);
		if (c == EOF || c == 0)
			break;
		switch (st)
		{
		case 0:
		{
			if (c == '%')
			{
				st = 1;
			}
			else
			{
				fprintf(f2, "%c", c);
				printf("%c", c);
				st = 0;
			}
			break;
		}
		case 1:
		{
			if (c != '\n')
			{
				break;
			}
			else
			{
				fprintf(f2, "%c", c);
				printf("%c", c);
				st = 0;
			}
		}
		}
	}
	fclose(f1);
	fclose(f2);
}

// int main()
// {
// 	FILE *fp1 = fopen("t5.txt", "r");
// 	initializeLexer(fp1);
// 	Token *t = getNextToken();
// 	while (t != NULL)
// 	{
// 		printf("Line No. %d Lexeme %s      Token %s\n", t->lineNo, t->lexeme, Terminal_tk[t->tokenName]);
// 		addToSymTb(t);
// 		t = getNextToken();
// 	}
// 	// for (int i = 0; i < tokenList->count; i++)
// 	// {
// 	// 	t = tokenList->tokens[i];
// 	// 	printf("Line No. %d Lexeme %s      Token %s\n", t->lineNo, t->lexeme, Terminal_tokens[t->tokenName]);
// 	// }
// 	// char f1[] = "t2.txt";
// 	// char f2[] = "removeCommentsFromT2.txt";
// 	// removeComments(f1, f2);
// 	fclose(fp1);
// 	return 0;
// }