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
	char buf[51];

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

	for(int i=0; i <= 50; i++) buf[i] = '\0';

    char *buffer=NULL;

    int l=recv(sockfd, buf, 10, 0);
    buffer=(char *)malloc(l*sizeof(char));
            for(int j=0;j<l;j++)
            buffer[j]=buf[j];

    printf("%s\n",buffer);
    free(buffer);
    buffer=NULL;

    int sz1=0;
    printf("Enter the username\n");
    int len1=getline(&buffer,&sz1,stdin);
    buffer[len1-1]='\0';
    send(sockfd, buffer, len1,0);
    free(buffer);
    buffer=NULL;
    
    l=recv(sockfd, buf, 10, 0);
    buffer=(char *)malloc(l*sizeof(char));
    for(int j=0;j<l;j++)
        buffer[j]=buf[j];

    printf("%s\n",buffer);
    if(buffer[0]=='N')
    {
        printf("Invalid username\n");
        close(sockfd);
	    return 0;
    }
    free(buffer);
    sz1=0;
    for(int i=0;i<=50;i++)buf[i]='\0';

	
    while(1)
    {
        buffer=NULL;
        int sz1=0;
        printf("Enter a shell command to be executed on the server side\n");
        int len1=getline(&buffer,&sz1,stdin);
        if(len1>=4 && (buffer[0]=='e' && buffer[1]=='x' && buffer[2]=='i' && buffer[3]=='t'))break;
        
        buffer[strlen(buffer)-1]='\0';
        //printf("%s %d\n", buffer,len1);

        for(int i=0;i<len1;i+=50)
        send(sockfd, buffer+i, min(len1-i,50),0);
            
        free(buffer);
        buffer=(char *)malloc(50*sizeof(char));
        int len=0;


        

            //printf("jay Shree Ram\n");
            for(int j=0; j<=50; j++) buf[j] = '\0';
            int l=recv(sockfd, buf, 50, 0);
            //printf("%d %s %c\n",l,buf,buf[l-1]);
            for(int j=0;j<l;j++)
            buffer[j]=buf[j];
            len=l;
            while(!(l==0 || buf[l-1]=='\0'))
            {
                //printf("The answer is:\n%s\n",buffer);
                l=recv(sockfd, buf, 50, 0);
                if(l==0)break;
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
            if(len>=4 && buffer[0]=='$' && buffer[1]=='$' && buffer[2]=='$' && buffer[3]=='$')
            printf("Invalid Command\n");
            else if(len>=4 && buffer[0]=='#' && buffer[1]=='#' && buffer[2]=='#' && buffer[3]=='#')
            printf("Error in running command\n");
            else
            printf("%s\n",buffer);
            free(buffer);
    }
    //printf("It would have been great if you continued using the server. Nevermind, will be hoping to meet you soon again! Till then good Bye! :) :) ;)\n");
	close(sockfd);
	return 0;
    
}