#include "unp.h"

int main(int argc, char **argv)
{
	char *ptr, **pptr;
	int sockfd, n;
	char recvline[MAXLINE +1];
	struct sockaddr_in servaddr;
	struct hostent *hptr;

	if(argc != 6) {
		err_quit("usage: <prog name> <url> <port number> <to> <from> <message>");
	}
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ) {
		err_sys("socket error");
	}

	bzero(&servaddr, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_port = htons(atoi(argv[2]));

	ptr = *(argv+1);
	if((hptr = gethostbyname(ptr)) == NULL) 
	{
		err_sys("gethostbyname error for host: %s: %s", ptr, hstrerror(h_errno));
	}

	pptr = hptr->h_addr_list;
	memcpy(&servaddr.sin_addr, *pptr, sizeof(*pptr));

	if(connect (sockfd, (SA*) &servaddr, sizeof(servaddr)) < 0) {
		err_sys("connect error");
	}

	snprintf(recvline, sizeof(recvline), "HELO aisling.student.comp.dit.ie\r\nMAIL FROM:%s\r\nRCPT TO:%s\r\nDATA\r\nFROM:%s\r\nTO:%s\r\nSubject:Urgent\r\n\r\n\r\n%s\r\n.\r\nQUIT\r\n", argv[4], argv[3], argv[4], argv[4], argv[5]);
	Write(sockfd, recvline, strlen(recvline));

	while((n = read(sockfd, recvline, MAXLINE)) > 0){
		recvline[n] = 0;
		if(fputs(recvline, stdout) == EOF){
			err_sys("fputs error");
		}
	}

	if(n < 0){
		err_sys("read error");
	}

	exit(0);
}
