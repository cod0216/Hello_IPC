#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>

#define SZ_STR_BUF 256

char *s_to_c = "fifo_s_to_c";
char *c_to_s = "fifo_c_to_s";

int in_fd, out_fd;
int len;
char cmd_line[SZ_STR_BUF];

void print_err_exit(char *msg)
{
	perror(msg);
	exit(1);
}

void dis_connect() 
{
	close(out_fd);
	close(in_fd);
}

void connect_to_server()
{
	out_fd = open(c_to_s, O_WRONLY);
	if(out_fd < 0)
		print_err_exit(c_to_s);
		in_fd = open(s_to_c, O_RDONLY);
		if(in_fd < 0)
			print_err_exit(s_to_c);
}

int input_send()
{
	if((len = read(0, cmd_line, SZ_STR_BUF)) <= 0)
		return len;
		
	if(write(out_fd, cmd_line, len) != len)
		return -1;
	return len;
}

int recv_output()
{
	len = read(in_fd, cmd_line, SZ_STR_BUF);
	if(len<0) 
		return len;
	
	if(write(1, cmd_line, len) != len)
		return -1;
	
	return len;
}

void single_process()
{
	while(1) {
		if(input_send() <= 0)
			break;
		if (recv_output() <= 0)
			break;
	}
}


int main()
{
	connect_to_server();
	single_process();
	dis_connect();
}












