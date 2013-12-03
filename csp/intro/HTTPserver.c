#include "unp.h"
#include <time.h>
#include <stdio.h>
#define HOME_PAGE "202: <html></html><body></body>\n"

int main(int argc, char **argv)
{
	int n, listenfd, connfd;//socket Ids; one for the listening socket and one for the connected socket
	char cmd[16], path[64], vers[16], path1[64] = {"."}; 
	struct sockaddr_in servaddr;//address structure to hold this server's address
	char wbuff[MAXLINE], rbuff[MAXLINE] ;//buffer to hold send data
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

		while((n = read(connfd, rbuff, MAXLINE)) > 0)
		{
			rbuff[n] = 0; //null terminate
		
			if(fputs(rbuff, stdout) == EOF)
				err_sys("fputs error");

			if(strstr(rbuff,"\r\n\r\n")>0)//looks for a needle in a haystack char *strstr(const
			{//char *haystack, const char *needle; it returns a pointer
				break;//to the location needle within haystack otherwise
			}//if not found it returns zero
			//strcmp returns zero if "index" is in path.
		}//end while

		if(n < 0)
			err_sys("read error");

		sscanf(rbuff, "%s %s %s", cmd, path, vers);

		if(!strcmp(path, "/index"))
		{
			snprintf(wbuff, sizeof(wbuff), "%s", HOME_PAGE);//using a hash-define string
			Write(connfd, wbuff, strlen(wbuff));
		}
		else
		{
			printf("%s could not be found\n", path);
			snprintf(wbuff, sizeof(wbuff), "ERROR 404: page not found\r\n");
			Write(connfd, wbuff, strlen(wbuff));
		}
		
		ticks = time(NULL);
		snprintf(wbuff, sizeof(wbuff), "Tim %.24s\r\n", ctime(&ticks));
		Write(connfd, wbuff, strlen(wbuff));
		
		close(connfd);
	}//end for
}//end main
