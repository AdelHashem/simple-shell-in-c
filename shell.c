#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

char *arg[5] = {};
void Fetch();

int main(){
    while(1)
    {
        Fetch();
        execvp(arg[0], arg);
    }
    return 0;
}

void Fetch(){ // Get The I/P and split it to arguments
    char *temp;
    char buffer[512];
    memset ( buffer, '\0', 512 ); // set the values to Null
    fgets(buffer, 512, stdin);
    
    char *token = strtok(buffer, " "); //split every word when catch space
    int i = 0;
    while(token)
    {
        token[strcspn(token, "\n")] = 0; // Delete Every /n in the string
        arg[i++] = token;
        temp = token;
        token = strtok(NULL, " ");
    }
    arg[++i]= NULL;
}

int Background_detect()// detect The & in the command 
{
    int i = 0;
    while(arg[i])
    {
        if(arg[i][0] == '&')
        return 1;
        i++;
    }
    return 0;
}