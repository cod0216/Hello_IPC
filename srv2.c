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

void print_err_exit(char*msg)
{
	perror(msg);
	exit(1);
}

void connect_to_client()
{
	in_fd = open(c_to_s, O_RDONLY);
	if(in_fd < 0 )
		print_err_exit(c_to_s);

	out_fd = open(s_to_c, O_WRONLY);
	if(out_fd < 0)
		print_err_exit(s_to_c);
}

void dis_connect()
{
	close(out_fd);
	close(in_fd);
}

void duplicate_IO()
{
	dup2(in_fd, 0);
	dup2(out_fd, 1);
	dup2(out_fd, 2);

	setbuf(stdin, NULL);
	setbuf(stdout, NULL);
 	dis_connect();
}

int main()
{
	connect_to_client();
	char ret_buf[SZ_STR_BUF];
	duplicate_IO();
	


	while(1) {
		len = read(0, cmd_line, SZ_STR_BUF);
		if(len <= 0)
			break;
		cmd_line[len] = '\0';
		if(strncmp(cmd_line, "exit", 4) == 0)
			break;
		sprintf(ret_buf, "server: %s", cmd_line);
		len = strlen(ret_buf);

		if(write(1, ret_buf, len) != len)
			break;
	}
	dis_connect();
}




