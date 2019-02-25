/**************************************************************************
 * Title: frieseti.smallsh.c						  *
 * Name: Tiffani Auer							  *
 * Due: Mar 4, 2019							  *
 * ***********************************************************************/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <sys/stat.h>	
#include <sys/types.h>	
#include <sys/wait.h>
#include <errno.h>	
#include <fcntl.h>
#include <signal.h>	
#include <unistd.h>	

//max amounts for command line
#define MAX_ARGS 512 	  
#define MAX_CHARS 2048   

//global vars
char input[MAX_CHARS];	//command line args
char *args[MAX_ARGS];

//prototypes
int CommandLine();

int main(){
  int runShell = 1,
      argc = 0;
  
  //while shell is running
  do{
    //run command line
    argc = CommandLine();
  
    //built-in entered: exit
    if(strncmp(args[0], "exit", 4) == 0)
    {
      printf("exiting\n");
      fflush(stdout);
      runShell = 0;
    }
    //built-in entered: status
    else if(strncmp(args[0], "status", 6) == 0)
    {
      printf("\"status\" entered\n");
      fflush(stdout);
    }
    //built in entered: cd
    else if(strncmp(args[0], "cd", 2) == 0)
    {
      printf("\"cd\" entered\n");
      fflush(stdout);
    }
    //comments entered
    else if(strncmp(args[0], "#", 1) == 0)
    {
      printf("comments entered");
      fflush(stdout);
    }
    
   
   
  
  }while(runShell == 1);
  
  return 0;
}

/**************************************************************************
 *Function:
 *Description: 
 *************************************************************************/

/**************************************************************************
 *Function: CommandLine()
 *Description: Prints : as the command line, then takes in user input as
 *as array.
 *************************************************************************/
CommandLine(){
  char *token = NULL;
  char cline[] = ": ";  
  int i = 0;
  memset(input, '\0', MAX_CHARS);  //instantiate buffer to null
  memset(args, '\0', MAX_ARGS);  //instantiate pointers to args to null
 
 //print and take command line args
  write(1, cline, 2);
  fflush(stdout);
  fflush(stdin);
  fgets(input, MAX_CHARS, stdin);

  //tokenize input, save args to array
  token = strtok(input, " ");
  while(token != NULL)
  {
    printf("%s\n", token);
    args[i] = token;
    printf("args[%d]: %s\n", i, args[i]);
    token = strtok(NULL, " ");
    i++;
  }
  
  return i;
}
