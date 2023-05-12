#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

/*
Including all the libraries
*/

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr,cli_addr;

    if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("Socket Creation Failed\n");
        exit(0);
    }
    else
    {
        printf("Socket created\n");
        //exit(0);
    }


    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(20000);
    inet_aton("127.0.0.1", &serv_addr.sin_addr);

    if((connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0))
    {
        printf("Unable to connect to server\n");
        exit(0);
    }

    char buf[110];

    for(int i=0;i<110;i++)
    {
        buf[i]='\0';
    }

    recv(sockfd,buf,100,0);
    printf("%s\n", buf);

    close(sockfd);
	return 0;
}