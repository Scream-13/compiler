#include<stdio.h>
#include<stdlib.h>
#define MAXSIZE 200

typedef struct{
    int ele[MAXSIZE];
    int top;
}Stack;

void initialize( Stack *stack){
    stack->top=-1;
}

int is_empty(Stack *stack){
    return (stack->top==    -1);
}

int is_full(Stack *stack){
    return (stack->top == MAXSIZE-1);
}

int peek(Stack *stack){
    if(is_empty(stack)){
        printf("stack is empty");
        return -1 ;
    }
    return stack->ele[stack->top] ;
}

void push(Stack *stack , int value){

    if (is_full(stack)){
        printf("Stack is full\n");
        return ;
    }
    stack->ele[++stack->top]=value;
}

int  pop(Stack *stack){
    if(is_empty(stack)){
        printf("stack is empty");
        return -1 ;
    }
    return stack->ele[stack->top--];

}


int main(){
    Stack stack;
    initialize(&stack);

    push(&stack ,4);
    
    int a = pop(&stack);

    printf("%d",a);




}