#include "unp.h"
#include <time.h>

int main(int argc, char **argv)
{
	int listenfd, connfd;//socket Ids; one for the listening socket and one for the connected socket
	struct sockaddr_in servaddr;//address structure to hold this server's address
	char buff[MAXLINE];//buffer to hold send data
	time_t ticks;//required to calculate date and time

	if (argc != 2) //verifies number of command-line args; two in this instance
		err_quit("usage: <Program Name> <Port No.>");
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);//create socket
						   //Note this slightly different to the client code (uses lower case 's') as the
						   //error checking is warpped inside a new function called Socket (upper case'S')

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	
	Listen(listenfd, LISTENQ);

	for( ; ; )
	{
		connfd = Accept(listenfd, (SA *) NULL, NULL);
		
		ticks = time(NULL);
		snprintf(buff, sizeof(buff), "Tim %.24s\r\n", ctime(&ticks));
		Write(connfd, buff, strlen(buff));
		
		getchar();
		
		close(connfd);
	}
}
