#include <fcntl.h>
#include <iostream>
#include <sys/wait.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
using namespace std;
int main(){
	int fd[2];
	if(-1 == pipe(fd)){
		perror("pipe");
		// tells you line and file
	}
	cerr << "fd[0] = "<< fd[0] << endl;
	cerr << "fd[1] = "<< fd[1] << endl;
	char** womp = (char**)malloc(BUFSIZ);
	string ls = "ls";
	womp[0] = (char*)ls.c_str();
	womp[1] = NULL;
	int pid = fork();
	if (pid == 0){
		close(1);
		dup(fd[1]);
		execvp(womp[0], womp);
	}
	else if(pid > 0){
		wait(0);
	}
	char** ew = (char**)malloc(BUFSIZ);
	string grep = "grep";
	string a = "a";
	ew[0] = (char*)grep.c_str();
	ew[1] = (char*)a.c_str();
	ew[2] = NULL;
	int pid2 = fork();
	if(pid2 == 0){
		close(0);
		dup(fd[0]);
		execvp(ew[0], ew);
	}
	else if(pid2 >0){
		wait(0);
	}
	return 0;

}

