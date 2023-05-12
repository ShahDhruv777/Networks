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

int main(int argc,char* argv[])
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

    int x;
    //argv[1]
    sscanf(argv[1], "%d", &x);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(x);
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

    
            
            int l=recv(sockfd, buf, 10, 0);
            char *buffer=(char *)malloc(l*sizeof(char));
            for(int j=0;j<l;j++)
            buffer[j]=buf[j];
            int len=l;
            while(!(l==0 || buf[l-1]=='\n' || buf[l-1]=='\0'))
            {
                l=recv(sockfd, buf, 10, 0);
                char *temp=(char *)malloc((len+l)*sizeof(char));
                for(int k=0;k<len;k++)
                {
                    temp[k]=buffer[k];
                }
                for(int k=len;k<len+l;k++)
                {
                    temp[k]=buf[k-len];
                }
                len+=l;
                char *t2=buffer;
                buffer=temp;
                free(t2);
            }
        printf("The answer is:\n%s\n",buffer);

    

    close(sockfd);
	return 0;
}