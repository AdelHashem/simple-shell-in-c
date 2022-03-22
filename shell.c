#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>

char *arg[10] = {} ;
void Fetch();

int main(){
    //arg = (char*) malloc(512 * sizeof(char));
    Fetch();
    printf("%s",arg[0]);
    printf("%s",arg[1]);
    printf("%s",arg[3]);


    return 0;
}

void Fetch(){
    char buffer[512];
    fgets(buffer, 512, stdin);
    char *token = strtok(buffer, " ");
    int i = 0;
    while(token)
    {
        arg[i++] = token;
        token = strtok(NULL, " ");
    }
    arg[++i]= "\0" ;
}