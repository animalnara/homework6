/** 
 * postfix.c
 *
 * School of Computer Science, 
 * Chungbuk National University
 */

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

#define MAX_STACK_SIZE 10
#define MAX_EXPRESSION_SIZE 20

/* stack 내에서 우선순위, lparen = 0 가장 낮음 */
typedef enum{
	lparen = 0,  /* ( 왼쪽 괄호 */
	rparen = 9,  /* ) 오른쪽 괄호*/
	times = 7,   /* * 곱셈 */
	divide = 6,  /* / 나눗셈 */
	plus = 5,    /* + 덧셈 */
	minus = 4,   /* - 뺄셈 */
	operand = 1 /* 피연산자 */
} precedence;

char infixExp[MAX_EXPRESSION_SIZE];   	/* infix expression을 저장하는 배열 */
char postfixExp[MAX_EXPRESSION_SIZE];	/* postfix로 변경된 문자열을 저장하는 배열 */
char postfixStack[MAX_STACK_SIZE];	/* postfix로 변환을 위해 필요한 스택 */
int evalStack[MAX_STACK_SIZE];		/* 계산을 위해 필요한 스택 */

int postfixStackTop = -1;  /* postfixStack용 top */
int evalStackTop = -1;	   /* evalStack용 top */

int evalResult = 0;	   /* 계산 결과를 저장 */

void postfixpush(char x);
char postfixPop();
void evalPush(int x);
int evalPop();
void getInfix();
precedence getToken(char symbol);
precedence getPriority(char x);
void charCat(char* c);
void toPostfix();
void debug();
void reset();
void evaluation();

int main()
{
	char command;

	do{
		printf("----------------------------------------------------------------\n");
		printf("               Infix to Postfix, then Evaluation               \n");
		printf("[----- 강태광 2021041098 -----]\n");
		printf("----------------------------------------------------------------\n");
		printf(" Infix=i,   Postfix=p,  Eval=e,   Debug=d,   Reset=r,   Quit=q \n");
		printf("----------------------------------------------------------------\n");

		printf("Command = ");
		scanf(" %c", &command);

		switch(command) {
		case 'i': case 'I':
			getInfix();
			break;
		case 'p': case 'P':
			toPostfix();
			break;
		case 'e': case 'E':
			evaluation();
			break;
		case 'd': case 'D':
			debug();
			break;
		case 'r': case 'R':
			reset();
			break;
		case 'q': case 'Q':
			break;
		default:
			printf("\n       >>>>>   Concentration!!   <<<<<     \n");
			break;
		}

	}while(command != 'q' && command != 'Q');

	return 1;
}

void postfixPush(char x)
{
    postfixStack[++postfixStackTop] = x;
}

char postfixPop()
{
    char x;
    if(postfixStackTop == -1)
        return '\0';
    else {
    	x = postfixStack[postfixStackTop--];
    }
    return x;
}

void evalPush(int x)
{
    evalStack[++evalStackTop] = x;
}

int evalPop()
{
    if(evalStackTop == -1)
        return -1;
    else
        return evalStack[evalStackTop--];
}

/**
 * infix expression을 입력받는다.
 * infixExp에는 입력된 값을 저장한다.
 */
void getInfix()
{
    printf("Type the expression >>> ");
    scanf("%s",infixExp);
}

precedence getToken(char symbol)
{
	switch(symbol) {
	case '(' : return lparen;
	case ')' : return rparen;
	case '+' : return plus;
	case '-' : return minus;
	case '/' : return divide;
	case '*' : return times;
	default : return operand;
	}
}

precedence getPriority(char x)
{
	return getToken(x);
}

/**
 * 문자하나를 전달받아, postfixExp에 추가
 */
void charCat(char* c)
{
	if (postfixExp == '\0')
		strncpy(postfixExp, c, 1);
	else
		strncat(postfixExp, c, 1);
}

/**
 * infixExp의 문자를 하나씩 읽어가면서 stack을 이용하여 postfix로 변경한다.
 * 변경된 postfix는 postFixExp에 저장된다.
 */
void toPostfix()
{
	/* infixExp의 문자 하나씩을 읽기위한 포인터 */
	char *exp = infixExp;
	char x; /* 문자하나를 임시로 저장하기 위한 변수 */

	/* exp를 증가시켜가면서, 문자를 읽고 postfix로 변경 */
    while (*exp != '\0') {
    if (isdigit(*exp)) {
        charCat(exp); // 피연산자일 경우 그대로 추가합니다
    } else {
        if (*exp == '(') {
            postfixPush(*exp); // 왼쪽 괄호는 스택에 push합니다
        } else if (*exp == ')') {  
            // 오른쪽 괄호일 경우 왼쪽 괄호가 나올 때까지 스택에서 pop하여 후위 표기식에 추가
            while (postfixStack[postfixStackTop] != '(') {
                charCat(&postfixStack[postfixStackTop--]);
            }
            postfixPop(); // 왼쪽 괄호를 스택에서 제거합니다
        } else {
            // 연산자일 경우, 스택의 top에 있는 연산자의 우선순위가 현재 연산자의 우선순위보다 높으면 스택에 push
            while (postfixStackTop != -1 && getPriority(postfixStack[postfixStackTop]) >= getPriority(*exp)) {
                charCat(&postfixStack[postfixStackTop--]);
            }
            postfixPush(*exp); // 현재 연산자를 스택에 push합니다
        }
    }
    exp++; // 다음 문자로 이동합니다
}

// 스택에 남아 있는 모든 연산자를 pop 하고 출력합니다.
while (postfixStackTop != -1) {
    charCat(&postfixStack[postfixStackTop--]);
}

}
void debug()
{
	printf("\n---DEBUG\n");
	printf("infixExp =  %s\n", infixExp);
	printf("postExp =  %s\n", postfixExp);
	printf("eval result = %d\n", evalResult);

	printf("postfixStack : ");
	for(int i = 0; i < MAX_STACK_SIZE; i++)
		printf("%c  ", postfixStack[i]);

	printf("\n");

}

void reset()
{
	infixExp[0] = '\0';
	postfixExp[0] = '\0';

	for(int i = 0; i < MAX_STACK_SIZE; i++)
		postfixStack[i] = '\0';
         
	postfixStackTop = -1;
	evalStackTop = -1;
	evalResult = 0;
}

void evaluation()
{
    evalStackTop = -1; // 스택 초기화
    char *exp = postfixExp; // 후위 표기식을 읽기 위한 포인터

    while (*exp != '\0') { // 후위 표기식을 왼쪽에서 오른쪽으로 읽으면서 계산합니다.
        if (isdigit(*exp)) { // 현재 문자가 피연산자인 경우
            evalPush(*exp - '0'); // 문자를 숫자로 변환하여 스택에 push
        } else {  // 두 개의 피연산자를 스택에서 pop하여 계산합니다
            int operand2 = evalPop();
            int operand1 = evalPop();
            int result = 0; // 계산 결과 변수

            // 연산자에 따라 계산 수행
            switch (*exp) {
                case '+': result = operand1 + operand2; break;
                case '-': result = operand1 - operand2; break;
                case '*': result = operand1 * operand2; break;
                case '/': result = operand1 / operand2; break;
            }

            evalPush(result); // 계산 결과를 스택에 push합니다
        }
        exp++; // 다음 문자로 이동
    }

    evalResult = evalPop(); // 최종 결과를 evalResult에 저장
    printf("결과: %d\n", evalResult);
}