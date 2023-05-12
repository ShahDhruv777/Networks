#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

int min(int a,int b)
{
    if(a<b)return a;
    return b;
}

int main()
{
    int sockfd;
    struct sockaddr_in serv_addr;
    // char *buffer=NULL;

    // if((sockfd=socket(AF_INET,SOCK_STREAM,0))<0)
    // {
    //     printf("Socket Creation Failed\n");
    //     exit(0);
    // }
    // else
    // {
    //     printf("Socket created\n");
        
    // }


    // serv_addr.sin_family=AF_INET;
    // inet_aton("127.0.0.1", &serv_addr.sin_addr);
    // serv_addr.sin_port=htons(20000);

    // if((connect(sockfd,(struct sockaddr *)&serv_addr,sizeof(serv_addr))<0))
    // {
    //     printf("Unable to connect to server\n");
    //     exit(0);
    // }

    // char *buffer=NULL;
    // int sz1=0;

    // int len1=getline(&buffer,&sz1,stdin);
    // printf("%s\n", buffer);
	// send(sockfd, buffer, len1+ 1, 0);
    // int			sockfd ;
	// struct sockaddr_in	serv_addr;

	int i;
	char buf[100];

	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0) {
		perror("Unable to create socket\n");
		exit(0);
	}

	serv_addr.sin_family	= AF_INET;
	inet_aton("127.0.0.1", &serv_addr.sin_addr);
	serv_addr.sin_port	= htons(20000);

	if ((connect(sockfd, (struct sockaddr *) &serv_addr,
						sizeof(serv_addr))) < 0) {
		perror("Unable to connect to server\n");
		exit(0);
	}

	for(i=0; i < 100; i++) buf[i] = '\0';
	
    while(1)
    {
        char *buffer=NULL;
        int sz1=0;
        printf("Enter the expression\n");
        int len1=getline(&buffer,&sz1,stdin);
        if(len1>=2 && (buffer[0]=='-' && buffer[1]=='1'))break;
        //printf("%s\n", buffer);
        for(int i=0;i<len1+1;i+=10)
        send(sockfd, buffer+i, min(len1+ 1-i,10),0);
            
        free(buffer);
        buffer=(char *)malloc(10*sizeof(char));
        int len=0;


        for(int j=0; j< 10; j++) buf[j] = '\0';
            
            int l=recv(sockfd, buf, 10, 0);
            for(int j=0;j<l;j++)
            buffer[j]=buf[j];
            len=l;
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
            //printf("jay Shree Ram\n");
            free(buffer);
    }
    printf("It would have been great if you continued using the server. Nevermind, will be hoping to meet you soon again! Till then good Bye! :) :) ;)\n");
	close(sockfd);
	return 0;
    
}