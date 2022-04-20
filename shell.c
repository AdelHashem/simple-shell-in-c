#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <signal.h>
#include <time.h>
#include <sys/wait.h>

//Funs
void Fetch();
int Background_detect();
void process();
void Change_Dir(const char *param);
void logfile(int sgn);
void welcomeScreen();
//Global Var
char *arg[20] = {}; // to carry the parameters
char logpath[256]; // to carry the log file path
FILE *f; // to carry the log file

int main(){
    getcwd(logpath,sizeof(logpath));
    strcat(logpath,"/log.txt");

    welcomeScreen();
    signal(SIGCHLD,logfile); //signal Handler
    while(1)
    {
        memset ( arg, '\0', sizeof(arg) ); // avoid noise and last loop
        char cwd[300]; // to carry the path
        getcwd(cwd,sizeof(cwd)); // get current path
        printf("%s>",cwd);
        Fetch(); // get line and spliting it

        if(strcmp(arg[0],"cd") == 0){ //is it a cd command?
            char para[256];
            strcpy(para,arg[1]); // just copying the parameter to pass it safly to the fun
            Change_Dir(para);
            continue;
        }
        else if (strcmp(arg[0],"exit") == 0){ //terminate
            exit(0);
        }
        else
        {
        process(); // fork and exevp
        }
        
    }
    return 0;
}

void Fetch(){ // Get The I/P and split it to arguments
    char buffer[512];
    memset ( buffer, '\0', 512 ); // set the values to Null
    fgets(buffer, 512, stdin); //avoid gets problems

    char *token = strtok(buffer, " "); //split every word when catch space
    int i = 0;
    while(token)
    {
        token[strcspn(token, "\n")] = 0; // Delete Every /n in the string
        arg[i++] = token;
        token = strtok(NULL, " ");
    }
    arg[++i]= NULL;
}

int Background_detect()// detect The & in the command
{
    int i = 0;
    while(arg[i])
    {
        if(arg[i][0] == '&'){
            arg[i] = NULL;
            return 1;}
        i++;
    }
    return 0;
}

void process()
{
    pid_t pid = fork();
    int waitStatus;
    int flag = 0;
    flag = Background_detect();
    if(pid == -1)
        printf("error occured in fork\n");
    else if(pid == 0)//child
    {
       if(execvp(arg[0], arg) < 0)  // handle errors and print the details
       {
           perror("execvp");
            exit(EXIT_FAILURE);
       }

    }
    else   //Parent (the main process)
    {

        if (flag == 1)   //see if there is "&"
        {
            sleep(1);
            return;  //don't wait the child to end
        }
        else // wait the child
        {
            if (waitpid(pid, &waitStatus, WUNTRACED | WCONTINUED) == -1) {
                printf("error occured in waitpidn");
                exit(EXIT_FAILURE);
            }
        }
    }

}

/* Handling cd Command in 3 Cases
1. when enter full path ex: /home/username/Desktop (always starts with "/")
2. when enter "~" then A folder in the /home/username/ ex: ~/Desktop  (always starts with "~/" )
3. when enter direct the folder name in the cwd ex: /home/user> cd Desktop
(cd ..) is handled by dafult fun chdir in the 3rd case
*/
void Change_Dir(const char* param){
    char path[512] = {};
    //case 1
    if(param[0] == '/')
    {
        if(chdir(param) < 0) // handle errors and print the details
            perror("chdir");
    //case 2
    }else if(param[0] == '~')
    {
        char username[50];
        getlogin_r(username,sizeof(username));
        strcpy(path,"/home/");
        strcat(path,username);
        strcat(path,param+1);
        if(chdir(path) < 0) // handle errors and print the details
            perror("chdir");
    //case 3
    }else{
        getcwd(path,sizeof(path));
        strcat(path,"/");
        strcat(path,param);
        if(chdir(path) < 0) // handle errors and print the details
            perror("chdir");
    }

}

void logfile(int sgn){
    f = fopen(logpath,"a");
    time_t rawtime;
    struct tm * timeinfo;
    time ( &rawtime );
    timeinfo = localtime ( &rawtime );
    fprintf(f,"Child process was terminated\t [%s]\n",asctime(timeinfo));
    fclose(f);

}

void welcomeScreen(){
        printf("\n\t============================================\n");
        printf("\t               Simple C Shell\n");
        printf("\t--------------------------------------------\n");
        printf("\t             Adel Hashem   (18010880)\n");
        printf("\t             Mahmoud Ahmed (18011654)\n");
        printf("\t             Mostafa Ahmed (18011775)\n");
        printf("\t============================================\n");
        printf("\n\n");
}
