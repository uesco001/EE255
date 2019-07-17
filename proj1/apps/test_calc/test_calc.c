#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 
#define GNU_SOURCE 
#include <unistd.h> 
#include <sys/syscall.h> 
#include <sys/types.h> 
#include <signal.h>

int main(int argc, char* argv[])

{
    if(argc > 4)
    { printf("too many arguments but will continue\n");}
    
    else if(argc < 4)
    { printf("too little arguments good bye\n"); return 0;}
    else{

 	char *num1;
    	char *num2;
    	char *op; 
    	int  inum1,inum2,answer;
	char op1;

    	num1 = argv[1];
    	num2 = argv[3];
    	op  = argv[2];
    	answer = 0;

    	int i = 0;
// check if valid numbers 
	for(i = 0; i<strlen(num1); i++)
	{
	    if(isalpha( *(num1+i) ))
		{ printf("first number is not a digit\n"); 
		  return 0;
		}
	} 
	for(i = 0; i<strlen(num2); i++)
	{
	    if(isalpha( *(num2+i) ))
		{ printf("second number is not a digit\n"); 
		  return 0;
		}
	}
// check if valid operation input and stores it
	int op_size = strlen(op);
	if(op_size == 1){
	    op1 = *op;
	}else if(op_size == 3){
	    if(*(op) == '\'' && *(op + 1) == '\'')
	    op1 = *(op + 1);
	}else{ printf("Invalid operation input\n");
	    return 0;
	}
// char to int
    	inum1 = atoi(num1);
    	inum2 = atoi(num2);
// calls sys_calc  and ouputs NaN or result  
	if(syscall(398, inum1,inum2,op1, &answer))
	{
	  printf("NaN\n");
	}
	else{
  	  printf("%d\n",answer);
	}

  
}   
  return 0;

}
