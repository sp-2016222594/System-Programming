#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/socket.h>
#include <unistd.h>
#include <string.h>
#include <netdb.h>
#include <netinet/in.h>

#define HOSTLEN 256

int make_server_socket(int port);
read_til_crnl(FILE*);
process_rq(FILE* fd, char* args);
do_head(FILE*);
cannot_do(FILE*);

int main(int ac, char* av[])
{
	int sock, fd;
	FILE* fpin;
	char request[BUFSIZ];

	if( ac == 1 )
	{
		fprintf(stderr, "usage:2016222594_PE9 protnum\n");
		exit(1);
	}
	sock = make_server_socket( atoi(av[1]) );


	if( sock == -1 )
		exit(2);

	while(1)
	{
		fd = accept( sock, NULL, NULL );
		fpin = fdopen( fd, "r" );
		fgets( request, BUFSIZ, fpin );
		printf("got a call: request=%s", request);
		read_til_crnl(fpin);
		process_rq(fd, request);
		fclose(fpin);
	}
}


int make_server_socket(int portnum)
{
	struct sockaddr_in saddr;
	struct hostent* hp;
	char hostname[HOSTLEN];
	int sock_id;

	sock_id = socket( PF_INET, SOCK_STREAM, 0 );
	if( sock_id == -1 )
	 return -1;

	bzero((void*)&saddr, sizeof(saddr));
	gethostname(hostname, HOSTLEN);
	hp=gethostbyname(hostname);
	bcopy((void*)hp->h_addr, (void*)&saddr.sin_addr, hp->h_length);
	saddr.sin_port = htons(portnum);
	saddr.sin_family = AF_INET;
	if( bind(sock_id, (struct sockaddr*)&saddr, sizeof(saddr))!=0 )
	 return -1;
	if( listen(sock_id, 1 ) != 0 )
	 return -1;
	return sock_id;
}


read_til_crnl(FILE* fp)
{
	char buf[BUFSIZ];
	while( fgets(buf, BUFSIZ, fp) != NULL && strcmp(buf, "\r\n")!=0 );
}


process_rq(FILE* fd, char* rq)
{
	char cmd[BUFSIZ], arg[BUFSIZ];

	if( fork() != 0 )
	 return;
	strcpy(arg, "./");
	if( sscanf(rq, "%s%s", cmd, arg+2) != 2 )
	 return;
	if( strcmp(cmd, "HEAD") != 0 )
		cannot_do(fd);
	else
		do_response(fd);
}


do_response(FILE* fp)
{
	fprintf(fp, "HTTP/1.0 200 OK\r\n");
}


cannot_do(FILE* fd)
{
	fprintf(fd, "HTTP/1.0 501 Not Implemented\r\n");
	fprintf(fd, "Content-type:text/plain\r\n");
	fprintf(fd, "\r\n");
	fprintf(fd, "This command is not yet implemented\r\n");
}


