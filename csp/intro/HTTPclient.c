#include "unp.h"

int main(int argc, char **argv)
{
	int sockfd, n, counter = 0;
	char recvline[MAXLINE + 1], buff[MAXLINE] ;
	struct sockaddr_in servaddr;

	if(argc !=4)
		err_quit("usage: ./httpclient <IPaddress> <Port Number> <Resource Name>");//to run the program using this command

	if((sockfd = socket (AF_INET, SOCK_STREAM, 0)) < 0)
		err_sys("socket error");

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2])); /*daytime server port taken from the command line*/

	if(inet_pton(AF_INET, argv[1], &servaddr.sin_addr) <= 0)
		err_quit("inet_pton error for %s", argv[1]);

	if(connect(sockfd, (SA *) &servaddr, sizeof(servaddr)) < 0)
		err_sys("connect error");

	snprintf(buff, sizeof(buff),"GET /%s HTTP/1.1\r\nHost:localhost\r\n\r\n", argv[3]);
	Write(sockfd, buff, strlen(buff));

	while((n = read(sockfd, recvline, MAXLINE)) > 0)
	{
		counter++;/* count the number of loops completed*/
		recvline[n] = 0;/* null terminate */
		if(fputs(recvline, stdout) == EOF)/*print data (time and data) returned from server*/

			err_sys("fputs error");
		
	}
	
	if(n < 0)
		err_sys("read error");
	
	printf("counter = %d\n", counter);

	exit(0);
}
