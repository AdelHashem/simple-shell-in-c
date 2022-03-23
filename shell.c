#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>


void Fetch();
int Background_detect();
void process();
void Change_Dir(const char *param);
char *arg[20] = {};
int main(){

    welcomeScreen();
    while(1)
    {
        memset ( arg, '\0', 20 );
        char cwd[300];
        getcwd(cwd,sizeof(cwd));
        printf("%s>",cwd);
        Fetch();

        if(strcmp(arg[0],"cd") == 0){
            char para[256];
            strcpy(para,arg[1]);
        Change_Dir(para);
        continue;
        }else
        {
        process();
        }
    }
    return 0;
}

void Fetch(){ // Get The I/P and split it to arguments
    char buffer[512];
    memset ( buffer, '\0', 512 ); // set the values to Null
    fgets(buffer, 512, stdin);

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
       if(execvp(arg[0], arg) < 0)
       {
           perror("execvp");
            exit(EXIT_FAILURE);
       }

    }
    else   //Parent (the main process)
    {

        if (flag == 1)   //see if there is "&"
        {
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

void Change_Dir(const char* param){
    char path[512] = {};
    if(param[0] == '/')
    {
        if(chdir(param) < 0)
            perror("chdir");

    }else if(param[0] == '~')
    {
        char username[50];
        getlogin_r(username,sizeof(username));
        strcpy(path,"/home/");
        strcat(path,username);
        strcat(path,param+1);
        if(chdir(path) < 0)
            perror("chdir");
    }else{
        getcwd(path,sizeof(path));
        strcat(path,"/");
        strcat(path,param);
        if(chdir(path) < 0)
            perror("chdir");
    }

}

void welcomeScreen(){
        printf("\n\t============================================\n");
        printf("\t               Simple C Shell\n");
        printf("\t--------------------------------------------\n");
        printf("\t             Adel Hashem (18010880)\n");
        printf("\t============================================\n");
        printf("\n\n");
}
