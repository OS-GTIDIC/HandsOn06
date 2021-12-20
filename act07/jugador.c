#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include <signal.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <errno.h>

void end_handler(); 
void end_handler(){
    exit(0);
}

int main()
{
    signal(SIGUSR1, end_handler);

    int n = 4;
    char pass[n];
    
    srand((unsigned int)(time(NULL)));

    int i = 0;
    while(1){
    for(i = 0; i < n; i++) {
        pass[i] = 65 + rand() % 62;
    }
    write(3,pass,n);
    }
    
}