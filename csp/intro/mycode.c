#include "unp.h"
#include <stdio.h>

#define name = "admin";
#define pass = "pass";

int main(int argc, char **argv)
{
	int n, listenfd, connfd;//socket Ids; one for the listening socket and one for the connected socket
	struct sockaddr_in servaddr;//address structure to hold this server's address
	char wbuff[MAXLINE], rbuff[MAXLINE] ;//buffer to hold send data
	char username[64], password[64];//array to store username and password
	int counter = 0;//counter to keep track of the attempts

	if (argc != 2) //verifies number of command-line args; two in this instance
		err_quit("usage: <UserName> <Password>");
	
	listenfd = Socket(AF_INET, SOCK_STREAM, 0);//create socket

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = htonl(INADDR_ANY);
	servaddr.sin_port = htons(atoi(argv[1]));

	Bind(listenfd, (SA *) &servaddr, sizeof(servaddr));
	Listen(listenfd, LISTENQ);

	for( ; ; )//start infinite connection loop
	{
		//accept connection request
		connfd = Accept(listenfd, (SA *) NULL, NULL);
		//Start read loop
		while((n = read(connfd, rbuff, MAXLINE)) > 0)
		{
			rbuff[n] = 0; //null terminate
		
			if(fputs(rbuff, stdout) == EOF)
				err_sys("fputs error");

			if(strstr(rbuff,"\r\n\r\n")>0)//look for end of the mesaage
			{
				break;
			}
		}//end read while

		if(n < 0)
			err_sys("read error");

		sscanf(rbuff, "%s %s", username, password);

		if((!strcmp(username, "admin"))||(!strcmp(password, "pass")))
		{
			counter++;
			if(counter == 1)
			{
				snprintf(wbuff, sizeof(wbuff),"1");
				Write(connfd, wbuff, strlen(wbuff));
			}

			if(counter == 2)
			{
				snprintf(wbuff, sizeof(wbuff),"2");
				Write(connfd, wbuff, strlen(wbuff));
			}

			if(counter == 3)
			{
				snprintf(wbuff, sizeof(wbuff), "3");
				Write(connfd, wbuff, strlen(wbuff));
			}

			if(counter > 3)
			{
				snprintf(wbuff, sizeof(wbuff), "4");
				Write(connfd, wbuff, strlen(wbuff));
				close(connfd);
			}
		}
		else
		{
			snprintf(wbuff, sizeof(wbuff), "0");
			Write(connfd, wbuff, strlen(wbuff));
			close(connfd);
		}
	}//end connection for
}//end main
