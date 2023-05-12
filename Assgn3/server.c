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

    int x;
    //argv[1]
    sscanf(argv[1], "%d", &x);

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

    printf("The server address is %d\n",x);

    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(x);
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
        //printf("%d client",i++);

        char buf[110];

            for(int i=0;i<110;i++)
            {
                buf[i]='\0';
            }
            
            int l=recv(newsockfd, buf, 10, 0);
            printf("%d\n",l);
            char *buffer=(char *)malloc(l*sizeof(char));
            for(int j=0;j<l;j++)
            buffer[j]=buf[j];
            int len=l;
            while(!(l==0 || buf[l-1]=='\n' || buf[l-1]=='\0'))
            {
                l=recv(newsockfd, buf, 10, 0);
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

        if(buffer[0]=='S' && buffer[1]=='e' && buffer[2]=='n' && buffer[3]=='d' && buffer[4]==' ' && buffer[5]=='L' && buffer[6]=='o' && buffer[7]=='a' && buffer[8]=='d')
        {
            srand(time(0));
            int y=rand()%100+1;
            printf("Sent Load %d\n",y);
            if(x%2)y++;
            char r[4];
            r[0]='\0';
            r[1]='\0';
            r[2]='\0';
            r[3]='\0';
            sprintf(r, "%d", y);
            send(newsockfd, r, strlen(r) + 1, 0);

        }
        else
        {
             struct tm* ptr;
            time_t t;
            t = time(NULL);
            ptr = localtime(&t);
            //printf("%s", asctime(ptr));
            strcpy(buf,asctime(ptr));
            send(newsockfd, buf, strlen(buf) + 1, 0);
        }

        // struct tm* ptr;
		// time_t t;
		// t = time(NULL);
		// ptr = localtime(&t);
		// //printf("%s", asctime(ptr));
		// strcpy(buf,asctime(ptr));
		// send(newsockfd, buf, strlen(buf) + 1, 0);

        close(newsockfd);
    }

    close(sockfd);
    return 0;

}