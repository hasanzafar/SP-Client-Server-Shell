#include <string.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <math.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <time.h>

#define MAXLIMIT 1000

//STRUCTURE FOR PROCESS LIST
typedef struct pList{
    char processName[50];
    int process_id;
    time_t startTime;
    time_t endTime;
    bool isActive;
    }p_list;
struct pList proclist[MAXLIMIT];
int plist_index=0;




int main()
{
    bool program=true;



    write(STDOUT_FILENO,"___________________________________________________\n",sizeof("___________________________________________________\n"));
    write(STDOUT_FILENO,"Welcome! [[SP Final Project - Hasan Zafar 13105]]\nWe await your command!\nEnter 'help' for more info on how to use this.",sizeof("Welcome! [[SP Assignment 1 - Hasan Zafar 13105]]\nWe await your command!\nEnter 'help' for more info on how to use this."));
    char s[2000]; //Declaring array of characters (string)
    write(STDOUT_FILENO,"\n___________________________________________________\n\n",sizeof("\n___________________________________________________\n\n"));


    while(program==true) //LOOPING PROGRAM TILL PERSON ENTERS EXIT
    {

    //User will input string to be tokenized and then executed
    int count=read(STDIN_FILENO,s,sizeof(s));
    s[count-1]='\0';

    //Assigning pointers that will stimulate what to do in console:
    char* point;
    char* num1;
    char* add="add";
    char* sub="sub";
    char* mul="mul";
    char* div="div";
    char* run="run";
    char* list="list";
    char* help="help";
    char* Exit="terminate";

    //Using strtok to divide words/numbers into tokens by the spaces to differentiate between them
    point=strtok(s," ");
    num1=point; //first token is saved in num1



    //ADD FUNCTIONALITY
    int sum=0;
    if(strcmp(num1,add)==0){ //compared first token with add, if it is add, then statements will execute
        while (point!=NULL){
                 sum=sum+atoi(point); //converting char to int using atoi.
                point=strtok(NULL," ");
    }
    //int nDigits = floor(log10(abs(sum))) + 1; //USE this function to find out number of digits in the sum result
    char a[100]; //array of same size as no. of digits to be stored is created.
    int n=sprintf(a,"%i",sum); //sprintf formats output and stores to string
    write(STDOUT_FILENO,a,n); //prints output
    write(STDOUT_FILENO,"\n",sizeof("\n"));
    }



    //SUBTRACT
    int diff=0;
    if(strcmp(num1,sub)==0){
            //if(point!=NULL){
            point=strtok(NULL," ");
            //}
            diff=atoi(point);
            point=strtok(NULL," ");
            while(point!=NULL){
                diff-=atoi(point);
                point=strtok(NULL," ");
                }
            char a[100];
            int n=sprintf(a,"%d",diff); //sprintf formats output and stores to string
            int num=write(STDOUT_FILENO,a,n);
            write(STDOUT_FILENO,"\n",sizeof("\n"));
            if (num==-1){
                perror("no output");
                    }
                }



     //MULTIPLY
    int res=1;
    //point="1";
    if(strcmp(num1,mul)==0){
            point=strtok(NULL," ");
            while(point!=NULL){
            res=res*atoi(point);
            point=strtok(NULL," ");
        }
        char a[100];
        int n=sprintf(a,"%d",res); //sprintf formats output and stores to string
        int num=write(STDOUT_FILENO,a,n);
        write(STDOUT_FILENO,"\n",sizeof("\n"));
    }



    //DIVIDE
    point="1";
    if(strcmp(num1,div)==0){
        point = strtok(NULL," ");
        res = atoi(point);

        if(point!=NULL){
            point=strtok(NULL," ");}
            while(point!=NULL){
            res=res/atoi(point);
            point=strtok(NULL," ");
        }
        char a[100];
        int n=sprintf(a,"%d",res); //sprintf formats output and stores to string
        int num=write(STDOUT_FILENO,a,n);
        write(STDOUT_FILENO,"\n",sizeof("\n"));
    }



    //RUN function
    char procname[50];
    char buff[1000];
    if(strcmp(num1,run)==0){
        int pipe1[2];
        pipe(pipe1);
        pid_t pid = fork();
        point=strtok(NULL," ");

        char* ptoken=point;//goes to next token, stores it in variable. used to run process
        sprintf(procname,"%s",point);

        if (pid == -1) {
            perror("FORK FAILED");
            exit(0);
           // return;
            }
        if (pid==0){
            //child process
            close(pipe1[0]);
            char cpid[12]={0x0};
            int j=(int)getpid();
            int n=sprintf(cpid,"%d",j);
            write(pipe1[1],cpid,n);
            write(STDOUT_FILENO,"CHILD ID: ",sizeof("CHILD ID: "));
            write(STDOUT_FILENO,cpid,sizeof(cpid));
            write(STDOUT_FILENO,"\n",sizeof("\n"));
            int num=execlp(ptoken,ptoken,NULL); //executes program
            if(num==-1){
                perror("Error at exec: ");
                exit(0);
            }

        }
        //parent
        else{
            close(pipe1[1]);
            int c_pid;
            int n = read(pipe1[0],buff,1000);
            if(n<0){
                perror("Parent read error");
                exit(0);
            }
            n=sscanf(buff,"%d",&c_pid);
            proclist[plist_index].process_id=c_pid;
            sprintf(proclist[plist_index].processName,"%s",procname);
            proclist[plist_index].isActive=true;
            time(&proclist[plist_index].startTime);
            plist_index++;
        }
        //wait(NULL);
    }
    if(strcmp(num1,list)==0){
        char buff[1000];
        int n = sprintf(buff,"Number of processes: %d\n",plist_index);
        write(STDOUT_FILENO,buff,n);
        n = sprintf(buff,"\nProcess Name\tProcess ID\tActive Status\tStart Time\tEnd Time\n");
        write(STDOUT_FILENO,buff,n);
        for(int i=0;i<plist_index;i++){
            char stime[9];
            char etime[9];
            strftime(stime,9,"%X",gmtime(&proclist[i].startTime));
            strftime(etime,9,"%X",gmtime(&proclist[i].endTime));
            n=sprintf(buff,"%s\t\t%d\t\t%d\t\t%s\t%s\n",proclist[i].processName,proclist[i].process_id,proclist[i].isActive,stime,etime);
            write(STDOUT_FILENO,buff,n);
        }
    }



    //HELP
    //declare string of what needs to be printed to help
    char h[300]="HERE TO ASSIST YOU!\n\nUse 'add' before writing string of numbers with space to perform add function on those numbers.\nFor e.g ''add 5 7 8''\n\n'run gedit '\n\nTo exit the program, write '0 ' ((space after exit)) and enter.\n\nHope you like our program! :)";
    if(strcmp(num1,help)==0){
        write(STDOUT_FILENO,h,sizeof(h));
            }



    //EXIT function
    if(strcmp(num1,Exit)==0){
            program=false; // break;
    }



    } //End of program loop
    return 0;
} //End of main method
