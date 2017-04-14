#include <stdlib.h> 
#include <stdio.h> 
#include <unistd.h>
//参考Chapter5 listing5.7
void writer (const char* message, int count, FILE* stream) { 
	for (; count > 0; --count) { 
	fprintf (stream, "%s\n", message);
	fflush (stream); //清空缓存区
	sleep (1); 
	} 
}
void reader (FILE* stream) { 
	char buffer[1024]; 
	while (!feof (stream) && !ferror (stream) && fgets (buffer, sizeof (buffer), stream) != NULL) //
		fputs (buffer, stdout); 
}
int main () { 
	int fds[2]; 
	pid_t pid;
	pipe (fds); //建立一个pipe
	pid = fork (); //创建子进程
	if(pid == (pid_t) -1){ //如果子进程创建失败
        perror("fork");
        exit(1);
    }
	if (pid == (pid_t) 0) { //如果是子进程
		FILE* stream; 
		close (fds[1]); 	//关闭fds[1]，从父进程接受数据，父进程关闭fds[0]
		stream = fdopen (fds[0], "r"); 
		reader (stream);	//父进程关闭fds[1]时结束
		close (fds[0]);
	} 
	else { //父进程
	FILE* stream; 
	close (fds[0]); //父进程关闭fds[1]向子进程传递数据
	stream = fdopen (fds[1], "w"); 
	writer ("Hello, world.", 5, stream); 
	close (fds[1]); //父进程关闭fds[1],reader读到eof结束,然后子进程结束，关闭fds[0]
	}
return 0;
}
