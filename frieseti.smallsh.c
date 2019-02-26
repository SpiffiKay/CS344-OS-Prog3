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

//prototypes
void CommandLine(char*, char**);
void ChangeDir(char**);
int KillProcesses(pid_t[], int);
void fgExitStatus(int);



int main(){
    int runShell = 1,
        pCount = 0,
        status = -7;
    char *input = malloc(MAX_CHARS * sizeof(char));
    char **args = malloc(MAX_ARGS * sizeof(char));
    memset(input, '\0', MAX_CHARS);
    memset(args, '\0', MAX_ARGS);
    pid_t proc[128];




    //while shell is running
    do{
      //run command line
      CommandLine(input, args);
  
      //built-in entered: exit
      if(strcmp(args[0], "exit") == 0)
        runShell = KillProcesses(proc, pCount);
      //built-in entered: status
      else if(strcmp(args[0], "status") == 0)
      { 
         //if no foreground processes have run yet
         if(status == -7)
           write(1, "exit status 0\n", 14);
         else
           fgExitStatus(status);
      }
      //built in entered: cd
      else if(strcmp(args[0], "cd") == 0)
        ChangeDir(args);
      //comments entered
      else if(strncmp(args[0], "#", 1) == 0)
       continue;
      //all other commands
      else
      {
        printf("all other input\n");
        continue;

      }
   
   
  
    }while(runShell == 1);
  

    //free alloc mem
    free(input);
    free(args);
  
    return 0;
}

/**************************************************************************
 *Function:
 *Description: 
 *************************************************************************/

/**************************************************************************
 *Function: CommandLine()
 *Description: Prints ": " as the command line, then 
 *************************************************************************/
void CommandLine(char *inputStr, char **args){
    char *token = NULL;
    char cLine[] = ": ";
    char *input = NULL;  
    int i = 0,
        numChars = -1;
    size_t bufSize = 0;
    memset(args, '\0', MAX_ARGS);  //instantiate pointers to args to null
    memset(inputStr, '\0', MAX_CHARS);
 
   //print and take command line args
    write(1, cLine, 2);
    fflush(stdout);
    fflush(stdin);
    numChars = getline(&input, &bufSize, stdin);
     //if getline error
    if(numChars == -1)
      clearerr(stdin);
    //copy string from getline to permanent array
    else
      sprintf(inputStr, input);

    //tokenize input, save args to array
    token = strtok(inputStr, " ");
    
    do
    {
      //remove trailing \n from getline
      token[strcspn(token, "\n")] = '\0';
      args[i] = token;
       
      //run in background
      if(strcmp(args[i], "&") == 0)
      {
        printf("backround process\n");
      }
      //redirect input
      else if(strncmp(args[i], "<", 1) == 0)
      {
        printf("input redirection\n");
      }
      //redirect output
      else if(strncmp(args[i], ">", 1) == 0)
      {
        printf("output redirection\n");
      }
      else if(strcmp(args[i], "$$") == 0)
      {
        printf("$$ to pid\n");
      }

      printf("args[%d]: %s\n", i, args[i]);
      token = strtok(NULL, " ");
      fflush(stdout);
      i++;
    }while(token != NULL);

    //free alloc mem
    free(input);
}

/**************************************************************************
 *Function: ChangeDir()                                                   *
 *Description: if command "cd" was given with no args change directory to *
 *home. If a directory path is given, change directory to that directory, *
 *if it is valid. If it is invalid, write an error message to stderr.     *
 *************************************************************************/
void ChangeDir(char **args){
    char *home = NULL; 

    //cd with no args
    if(args[1] == NULL)
    {
      printf("cd no args\n");
      fflush(stdout);
      home = getenv("HOME");
      chdir(home);
    }
    //cd with arg
    else
    {  


      //if dir doesn't exist
      if(chdir(args[1]) != 0)  
        fprintf(stderr, "%s: no such file or directory\n", args[1]);
    }
}




/**************************************************************************
 *Function: KillProcesses()                                               *
 *Description: Takes the number of running background processes as an     *
 *argument. Then goes through an array of pointers to the processes and   *
 *uses the kill function to send the SIGTERM signal to terminate all the  *
 *currently running background programs. It then returns zero to end the  *
 *program.                                                                *
 *************************************************************************/
int KillProcesses(pid_t proc[], int pCount){
    int i = 0;

    //kills all running background processes
    for(i; i < pCount; i++)
    {
      kill(proc[pCount], SIGTERM);
    }  

    printf("kill program\n");
    return 0;
}

/**************************************************************************
 *Function: fgStatus()
 *Description: 
 *************************************************************************/
void fgExitStatus(int status){
    char exitStatus[100];
    memset(exitStatus, '\0', 100);    

    //exited by status
    if(WIFEXITED(status))
    {
      sprintf(exitStatus,"exit status %d\n", WEXITSTATUS(status));
      write(1, exitStatus, strlen(exitStatus));
    }  
    //terminated by signal 
    else
    {
      sprintf(exitStatus, "terminated by signal %d\n", WTERMSIG(status));
      write(1, exitStatus, strlen(exitStatus));
    }
}

