#include<iostream>
#include<sys/types.h>
#include<sys/wait.h>
#include<unistd.h>
#include<bits/stdc++.h>
using namespace std;

/*flag is_BG is set true if input command needs to be run in background*/
bool  is_BG=false;



/* Greet the user with "Good Morning","Good Afternoon" or "Good Evening" ,according to the local time */  

void init_shell(){
	time_t curTime=time(NULL);
	struct tm *cTime=localtime(&curTime);
	int hr=cTime->tm_hour;
	string tmofDay;
	if(hr<12)tmofDay="Morning";
	else if(hr>=12 && hr<19) tmofDay="Afternoon";
	else tmofDay="Evening";
	cout<<"***********************************************\n";
	cout<<"           "<<"Good "<<tmofDay<<endl;
	cout<<"***********************************************\n";
	}

/* INPUT  is the space separated input command(1st argument);
   ARGS is array of char* pointers(2nd argument) to store program name and arguments.
   The input command is parsed into its components (program name and arguments) and stored in the 2-D char array .
   If the command is to be run in background(ends with ampersand("&") ,is_BG flag is set. */  

void processString(char* input,char **args){
	int i=0;
	args[i]=new char[100];
	
	//break the input command into words(space acts as delimiter) and store it in 2-D character array
	
	args[i]=strtok(input," ");
	while(args[i]!=NULL){
	   i++;
	   args[i]=strtok(NULL," ");
          //cout<<args[i]<<endl;
	}
	
	//replace the program name(command) with equivalent linux shell command

	if(strcmp(args[0],"newdir")==0){
			string s("mkdir");
			strcpy(args[0],s.c_str());
			}
	else if(strcmp(args[0],"editfile")==0){
			string s("vi");
			strcpy(args[0],s.c_str());
	}
	else if(strcmp(args[0],"content")==0){
			string s("cat");
			strcpy(args[0],s.c_str());
	}
	else if(strcmp(args[0],"info")==0){
			string s("stat");
			strcpy(args[0],s.c_str());
	}
	
	/*check if command asks to be run in background,If true,set background flag 
	and trim the command*/
        
	if(strcmp(args[i-1],"&")==0){
		is_BG=1;
	        args[i-1]=NULL;
	}
}

 /*Spawn a child process running a new program.
 "program" is the name of program to run;
 "arg_list" is null-terminated list of character strings to be passed as program's argument list.
 returns process Id of spawned process. */

		 
int spawn(char *program,char **arg_list){
	
	pid_t child_pid;
	/* duplicate this  process*/
	child_pid=fork();

	if(child_pid!=0){
		/*this is parent process*/
		
		/*if the program is to be run on the foreground , 
		wait till the child process completes its execution.
		Otherwise,if it is a background process,do not wait.
		Set background flag as false for next command */ 
		
		if(!is_BG){
			waitpid(child_pid,nullptr,0);
			}
		is_BG=0;
		return child_pid;
		}
	
	else{	
		/*child process*/

	       /*Execute program searching for it in the path*/
	       execvp(program,arg_list);

	       /*execvp returns only if error occured*/
	
		fprintf(stderr,"invalid command\n");
		abort();
	   }
          
	}
int main(){
        
	init_shell();
        string input;
	char  inputString[1000];
	while(1){
	  
          /*prompt "BCSE!!" to take commands*/
	  cout<<"BCSE!!";
	  
	  /*get user input from stdin*/
	  getline(cin,input);
	  
	  /*convert input string to char array*/
	  char inputString[input.length()+1];
	  strcpy(inputString,input.c_str());
	  char *parsedArgs[100];
	  
	  /*parse the command*/
	  processString(inputString,parsedArgs);
	  
	  /*if command is "exitbcse" , quit shell ;otherwise execute the command using fork() and exec() */
	  if(strcmp(parsedArgs[0],"exitbcse")==0) break;
	  spawn(parsedArgs[0],parsedArgs);  			
	}
	return 0;
}

	
