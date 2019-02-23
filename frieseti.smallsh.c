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
char args[MAX_CHARS];	//command line args

//prototypes
void CommandLine();

int main(){
  int runShell = 1;
  
  //while shell is running
  do{
    //run command line
    CommandLine();
  
    //built-in entered: exit
    if(strncmp(args, "exit", 4) == 0)
    {
      printf("exiting\n");
      fflush(stdout);
      runShell = 0;
    }
    //built-in entered: status
    else if(strncmp(args, "status", 6) == 0)
    {
      printf("\"status\"entered\n");
      fflush(stdout);
    }
    //built in entered: cd
    else if(strncmp(args, "cd", 2) == 0)
    {
      printf("\"cd\" entered\n");
      fflush(stdout);
    }
    //comments entered
    else if(strncmp(args, "#", 1) == 0)
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
void CommandLine(){
  char *cptr;
  char cline[] = ": ";  
  memset(args, '\0', MAX_CHARS);  //instantiate buffer to null

  //print: and take and parse command line args
  write(1, cline, 2);
  fflush(stdout);
  fflush(stdin);

  fgets(args, MAX_CHARS, stdin);	
}
