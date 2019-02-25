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
pid_t proc[128];


//prototypes
void CommandLine();
void ChangeDir();
int KillProcesses(int);
void fgExitStatus(int);



int main(){
    int runShell = 1,
        pCount = 0,
        status = -7;
   



    //while shell is running
    do{
      //run command line
      CommandLine();
  
      //built-in entered: exit
      if(strncmp(args[0], "exit", 4) == 0)
        runShell = KillProcesses(pCount);
      //built-in entered: status
      else if(strncmp(args[0], "status", 6) == 0)
      { 
         //if no foreground processes have run yet
         if(status == -7)
           write(1, "exit status 0\n", 14);
         else
           fgExitStatus(status);
      }
      //built in entered: cd
      else if(strncmp(args[0], "cd", 2) == 0)
        ChangeDir();
      //comments entered
      else if(strncmp(args[0], "#", 1) == 0)
       runShell = 1;
      //all other commands
      else
       runShell = 1;

    
   
   
  
    }while(runShell == 1);
  

    
  
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
void CommandLine(){
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
      args[i] = token;
      token = strtok(NULL, " ");
      i++;
    }
}

/**************************************************************************
 *Function: ChangeDir()                                                   *
 *Description: if command "cd" was given with no args change directory to *
 *home. If a directory path is given, change directory to that directory, *
 *if it is valid. If it is invalid, write an error message to stderr.     *
 *************************************************************************/
void ChangeDir(){
    char *home = NULL; 

    //cd with no args
    if(args[1] == NULL)
    {
      printf("in cd no args\n");
      fflush(stdout);
      home = getenv("HOME");
      chdir(home);
    }
    //cd with arg
    else
    {
      printf("in cd with arg\n");
      fflush(stdout);
      //if unsuccessful
    if(chdir(args[1]) != 0)
    { 
      fprintf(stderr, "%s: no such file or directory\n", args[1]);
    }
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
int KillProcesses(int pCount){
    int i = 0;

    //kills all running background processes
    for(i; i < pCount; i++)
    {
      kill(proc[pCount], SIGTERM);
    }  

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

