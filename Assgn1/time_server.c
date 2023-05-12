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
    serv_addr.sin_addr.s_addr=INADDR_ANY;

    if((bind(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr)))<0)
    {
        perror("Binding Failed\n");
        exit(0);
    }
    else
    {
        printf("Binding done\n");
    }

    listen(sockfd,5);

    char buf[110];
    int i=1;
    while(1)
    {
        int clilen = sizeof(cli_addr);
        int newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

        if(newsockfd<0)
        {
            printf("Unable to connect to client, Accept Error\n");
            return 0;
        }
        printf("%d client",i++);
        struct tm* ptr;
		time_t t;
		t = time(NULL);
		ptr = localtime(&t);
		//printf("%s", asctime(ptr));
		strcpy(buf,asctime(ptr));
		send(newsockfd, buf, strlen(buf) + 1, 0);

        close(newsockfd);
    }

    close(sockfd);
    return 0;

}