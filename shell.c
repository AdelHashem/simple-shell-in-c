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

void Fetch(){
    char *temp;
    char buffer[512];
    memset ( buffer, '\0', 512 );
    fgets(buffer, 512, stdin);
    
    char *token = strtok(buffer, " ");
    int i = 0;
    while(token)
    {
        token[strcspn(token, "\n")] = 0;
        arg[i++] = token;
        temp = token;
        token = strtok(NULL, " ");
    }
    arg[++i]= NULL;
}