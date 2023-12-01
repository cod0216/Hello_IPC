#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>
#include <sys/wait.h>
#include <sys/resource.h>
#include <sys/time.h>
#include <err.h>


#define SZ_STR_BUF 256
char *s_to_c = "fifo_s_to_c";
char *c_to_s = "fifo_c_to_s";

int in_fd, out_fd;
int len;
char cmd_line[SZ_STR_BUF];

void print_err_exit(char*msg)
{
	perror(msg);
	exit(1);
}

void connect_to_client()
{
	mkfifo(c_to_s, 0600);
	mkfifo(s_to_c, 0600);
	in_fd = open(c_to_s, O_RDONLY);
	if(in_fd < 0 )
		print_err_exit(c_to_s);

	out_fd = open(s_to_c, O_WRONLY);
	if(out_fd < 0)
		print_err_exit(s_to_c);
	dup2(out_fd, 2);
}

void dis_connect()
{
	close(0);
	close(1);
	close(2);
}

void daemonize()
{
	int i;
	pid_t pid;
	struct rlimit rl;
	
	umask(0);

	if((pid = fork()) <0)
		print_err_exit("fork");
	else if (pid > 0) 
		exit(0);
		
	setsid();

	signal(SIGHUP, SIG_IGN);
	
	if((pid = fork()) < 0)
		print_err_exit("fork");
	else if (pid > 0) 
		exit(0);

	if (getrlimit(RLIMIT_NOFILE, &rl) < 0)
		print_err_exit("getrlimit: can't get file limit");

 if (rl.rlim_max == RLIM_INFINITY)
		rl.rlim_max = 1024;

	for (i = 0; i < rl.rlim_max; ++i)
		close(i);

}


int main()
{
	daemonize();
	while(1) {
		connect_to_client();
		pid_t pid = fork();
		if (pid < 0)
			print_err_exit("cmd_server_Daemon: fork() ");
		else if (pid == 0 )
			execl("./cmd", "cmd", NULL);
		else {
			dis_connect();
			waitpid(pid, NULL, 0);
			sleep(2);
		}	
	}
}
	




