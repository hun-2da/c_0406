#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define ITP 1
#define EVAL 2

typedef struct {
	/**스택*/
	int *stack_i;
	int top;
	int capacity;
}stack_struct;


int is_empty(stack_struct* s) {
	return (s->top == -1);
}
int is_full(stack_struct* s) {
	return (s->top == (s->capacity - 1));
}

/** 스택에 푸시를 해줄 함수_ 만약 스택이 풀이라면 realloc해주어 사이즈를 눌려주고 풀이 아니라면 스택의 top+1에 item값을 저장
문자열을 입력받았을 경우를 대비해 *형태로 받을 것을 명시*/
void push(stack_struct* s, int item) {
	if (is_full(s)) {
		s->capacity = s->capacity * 2;
		s->stack_i = realloc(s->stack_i, s->capacity * sizeof(int));
		return;
	}
	else s->stack_i[++s->top]=item;
}

/**스택에 있는 요소를 하나씩 빼오기 위한 함수_ 만약 스택이 비어있다면 empty_ 아니면 스택[top]값을 리턴해주고 top-1연산 */
char pop(stack_struct* s,int p) {
	if (is_empty(s)&& p == EVAL) {
		fprintf(stderr, "피연산자가 부족합니다. \n");
		exit(1);
	}else if (is_empty(s) && p == ITP) {
		fprintf(stderr, "(가 없습니다.\n");
		exit(1);
	}
	else return s->stack_i[s->top--];
}
char peek(stack_struct* s) {
	return s->stack_i[s->top];
}
/**우선순위를 매기기위한 함수*/
int prech(char c) {
	switch (c) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '/': case '*': return 2;
	}
	return -1;
}

/**입력 받은 문자열을 후위식으로 변경하는 함수*/
void infix_to_postfix(stack_struct* s,char exp[], char output_string []) {
	int number = 0;		// 푸시를 위한 변수_ 
	int len = strlen(exp);		// exp의 길이
	int b_p = 0;
	char ch,top_c;		// 괄호가 닫힐때 스택에 들어있던 문자들을 하나씩 pop해서 대입해둘 변수_ 

	//문자열의 길이만큼 반복하는 반복문
	for (int i = 0; i < len; i++) {
		ch = exp[i];	//i에 해당하는 문자를 ch변수에 저장하여 사용

		switch (ch) {

			//ch가 연산자인 경우
		case '+': case '-': case '*': case '/':
			while (!is_empty(s) && (prech(ch) <= prech(peek(s))))
				output_string[number++] = pop(s, ITP);
				
			output_string[number++] = ' ';
			push(s, ch);
			break;

			//ch가 괄호인 경우
		case '(': push(s, ch); b_p++; break;
		case ')': 
			b_p--;
			top_c = pop(s,ITP);
			while (top_c != '(') {
				output_string[number++] = top_c;
				top_c = pop(s,ITP);
				
			}break;
			
			//ch가 피연산자인 경우
		default:
			if (!('0' <= ch && ch <= '9')) {
				fprintf(stderr, "잘못된 입력값이 포함되어 있습니다.\n");
				exit(1);
			}
			output_string[number++] = ch;
			break;
		}
	}
	if (b_p != 0) {
		fprintf(stderr, ")가 없습니다. \n");
		exit(1);
	}
	//스택에 남아있는 모든 값들을 출력
	while (!is_empty(s)) {
		output_string[number++] = pop(s,ITP);
		output_string[number] = NULL;
	}output_string[number] = '\0';
	return;
}

/**후위식 문자열을 계산하기위한 함수*/
int eval(stack_struct* s, char exp[]) {
	int op1, op2, value;
	int len = strlen(exp);
	
	//r은 realloc해줄 변수, n은 i대신 사용할 변수,
	//int r = 0,n;
	for (int i = 0; i < len; i++) {
		int op = 0;
		char ch = exp[i];
		
		if ('0' <= ch && ch <= '9') { //ch가 숫자일 경우

			// 다음수도 숫자라면 여러자릿수 숫자로 취급
			if ('0' <= exp[i+1] && exp[i+1] <= '9') {

				// 두자리수 이상의 숫자를 저장할 문자열 변수
				char* s_array = malloc(sizeof(char));
				for (int a = 0, n = i;; a++, n++) {
					if (/*'0' <= exp[n] && exp[n] <= '9'*/exp[n]!=' ') {
						s_array = realloc(s_array, (a+1) * sizeof(char));
						//exp[i] = s_array;
						s_array[a] = exp[n];
						
						
					}
					else {
						i = n;
						//s_array[a+1] = '\0';
						break;
					}
				}
				//strcpy(exp[i], s_array);
				value = atoi(s_array);
				free(s_array);
			}
			else
				value = ch - '0';

			push(s, value);
			printf("push : %d\n", value);
		
			
		}
		else if(ch!=' ')/*숫자가 아닐경우*/ {
			op2 = pop(s,EVAL);
			op1 = pop(s,EVAL);

			printf("%d\t%d\t\t%c\n", op1, op2, ch);

			switch (ch) {
			case '+': op = op1 + op2; break;
			case '-': op = op1 - op2; break;
			case '*': op = op1 * op2; break;
			case '/': op = op1 / op2; break;
			}
			push(s, op);
			printf("push : %d\n", op);
		}
	}
	if (s->top != 0) {
		for (; s->top > 0;)
			fprintf(stderr, "연산자가 부족하여 피연산자 %d 값이 남습니다.\n", pop(s,EVAL));
	}
	return pop(s,EVAL);
}
void main() {
	printf("중위식 표현은 : ");

	int n = 100;

	//입력을 받기위한 char형 동적 배열로 구현하기 위해 포인터 변수 생성
	char* input_array = malloc(n * sizeof(char));
	if (input_array == NULL) {
		fprintf(stderr, "입력 문자 저장공간이 NULL");
		return;
	}
	scanf_s("%s", input_array, n);

	// 후위식으로 출력시켜줄 변수
	char* output_array = malloc(n * sizeof(char));
	if (input_array == NULL) {
		fprintf(stderr, "입력 문자 저장공간이 NULL");
		return;
	}

	//element 구조체를 동적할당받아 사용하기 위한 포인터 변수 선언
	stack_struct* s_s = malloc(sizeof(stack_struct));
	if (s_s == NULL) {
		fprintf(stderr, "구조체가 NULL");
		return;
	}
	s_s->top = -1;
	s_s->capacity = 10;

	//연산을 할때 사용할 stack_c 동적할당으로 사이즈 조절이 가능하게끔 만들었음
	s_s->stack_i = malloc(s_s->capacity * sizeof(char));
	if (s_s->stack_i == NULL) {
		fprintf(stderr, "c스택이 NULL");
		return;
	}

	//중위 표현식으로 입력받은 문자열을 후위식으로 변형하기위한 함수 스택
	infix_to_postfix(s_s, input_array, output_array);
	printf("후위식 표현은 %s \n", output_array);

	//후위표현식을 연산하기위한 함수
	printf("결과 값은 %d \n", eval(s_s, output_array));

	//할당한 메모리들을 반환
	free(s_s->stack_i);
	free(s_s);
	free(input_array);
	free(output_array);

	system("pause");
}
