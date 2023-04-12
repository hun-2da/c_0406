#include<stdio.h>
#include<stdlib.h>
#include <string.h>
#define ITP 1
#define EVAL 2

typedef struct {
	/**����*/
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

/** ���ÿ� Ǫ�ø� ���� �Լ�_ ���� ������ Ǯ�̶�� realloc���־� ����� �����ְ� Ǯ�� �ƴ϶�� ������ top+1�� item���� ����
���ڿ��� �Է¹޾��� ��츦 ����� *���·� ���� ���� ���*/
void push(stack_struct* s, int item) {
	if (is_full(s)) {
		s->capacity = s->capacity * 2;
		s->stack_i = realloc(s->stack_i, s->capacity * sizeof(int));
		return;
	}
	else s->stack_i[++s->top]=item;
}

/**���ÿ� �ִ� ��Ҹ� �ϳ��� ������ ���� �Լ�_ ���� ������ ����ִٸ� empty_ �ƴϸ� ����[top]���� �������ְ� top-1���� */
char pop(stack_struct* s,int p) {
	if (is_empty(s)&& p == EVAL) {
		fprintf(stderr, "�ǿ����ڰ� �����մϴ�. \n");
		exit(1);
	}else if (is_empty(s) && p == ITP) {
		fprintf(stderr, "(�� �����ϴ�.\n");
		exit(1);
	}
	else return s->stack_i[s->top--];
}
char peek(stack_struct* s) {
	return s->stack_i[s->top];
}
/**�켱������ �ű������ �Լ�*/
int prech(char c) {
	switch (c) {
	case '(': case ')': return 0;
	case '+': case '-': return 1;
	case '/': case '*': return 2;
	}
	return -1;
}

/**�Է� ���� ���ڿ��� ���������� �����ϴ� �Լ�*/
void infix_to_postfix(stack_struct* s,char exp[], char output_string []) {
	int number = 0;		// Ǫ�ø� ���� ����_ 
	int len = strlen(exp);		// exp�� ����
	int b_p = 0;
	char ch,top_c;		// ��ȣ�� ������ ���ÿ� ����ִ� ���ڵ��� �ϳ��� pop�ؼ� �����ص� ����_ 

	//���ڿ��� ���̸�ŭ �ݺ��ϴ� �ݺ���
	for (int i = 0; i < len; i++) {
		ch = exp[i];	//i�� �ش��ϴ� ���ڸ� ch������ �����Ͽ� ���

		switch (ch) {

			//ch�� �������� ���
		case '+': case '-': case '*': case '/':
			while (!is_empty(s) && (prech(ch) <= prech(peek(s))))
				output_string[number++] = pop(s, ITP);
				
			output_string[number++] = ' ';
			push(s, ch);
			break;

			//ch�� ��ȣ�� ���
		case '(': push(s, ch); b_p++; break;
		case ')': 
			b_p--;
			top_c = pop(s,ITP);
			while (top_c != '(') {
				output_string[number++] = top_c;
				top_c = pop(s,ITP);
				
			}break;
			
			//ch�� �ǿ������� ���
		default:
			if (!('0' <= ch && ch <= '9')) {
				fprintf(stderr, "�߸��� �Է°��� ���ԵǾ� �ֽ��ϴ�.\n");
				exit(1);
			}
			output_string[number++] = ch;
			break;
		}
	}
	if (b_p != 0) {
		fprintf(stderr, ")�� �����ϴ�. \n");
		exit(1);
	}
	//���ÿ� �����ִ� ��� ������ ���
	while (!is_empty(s)) {
		output_string[number++] = pop(s,ITP);
		output_string[number] = NULL;
	}output_string[number] = '\0';
	return;
}

/**������ ���ڿ��� ����ϱ����� �Լ�*/
int eval(stack_struct* s, char exp[]) {
	int op1, op2, value;
	int len = strlen(exp);
	
	//r�� realloc���� ����, n�� i��� ����� ����,
	//int r = 0,n;
	for (int i = 0; i < len; i++) {
		int op = 0;
		char ch = exp[i];
		
		if ('0' <= ch && ch <= '9') { //ch�� ������ ���

			// �������� ���ڶ�� �����ڸ��� ���ڷ� ���
			if ('0' <= exp[i+1] && exp[i+1] <= '9') {

				// ���ڸ��� �̻��� ���ڸ� ������ ���ڿ� ����
				char* s_array = malloc(sizeof(char));
				for (int a = 0, n = i;; a++, n++) {
					if (/*'0' <= exp[n] && exp[n] <= '9'*/exp[n]!=' ') {
						s_array = realloc(s_array, (a+2) * sizeof(char));
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
		else if(ch!=' ')/*���ڰ� �ƴҰ��*/ {
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
			fprintf(stderr, "�����ڰ� �����Ͽ� �ǿ����� %d ���� �����ϴ�.\n", pop(s,EVAL));
	}
	return pop(s,EVAL);
}
void main() {
	printf("������ ǥ���� : ");

	int n = 100;

	//�Է��� �ޱ����� char�� ���� �迭�� �����ϱ� ���� ������ ���� ����
	char* input_array = malloc(n * sizeof(char));
	if (input_array == NULL) {
		fprintf(stderr, "�Է� ���� ��������� NULL");
		return;
	}
	scanf_s("%s", input_array, n);

	// ���������� ��½����� ����
	char* output_array = malloc(n * sizeof(char));
	if (input_array == NULL) {
		fprintf(stderr, "�Է� ���� ��������� NULL");
		return;
	}

	//element ����ü�� �����Ҵ�޾� ����ϱ� ���� ������ ���� ����
	stack_struct* s_s = malloc(sizeof(stack_struct));
	if (s_s == NULL) {
		fprintf(stderr, "����ü�� NULL");
		return;
	}
	s_s->top = -1;
	s_s->capacity = 10;

	//������ �Ҷ� ����� stack_c �����Ҵ����� ������ ������ �����ϰԲ� �������
	s_s->stack_i = malloc(s_s->capacity * sizeof(char));
	if (s_s->stack_i == NULL) {
		fprintf(stderr, "c������ NULL");
		return;
	}

	//���� ǥ�������� �Է¹��� ���ڿ��� ���������� �����ϱ����� �Լ� ����
	infix_to_postfix(s_s, input_array, output_array);
	printf("������ ǥ���� %s \n", output_array);

	//����ǥ������ �����ϱ����� �Լ�
	printf("��� ���� %d \n", eval(s_s, output_array));

	//�Ҵ��� �޸𸮵��� ��ȯ
	free(s_s->stack_i);
	free(s_s);
	free(input_array);
	free(output_array);

	system("pause");
}