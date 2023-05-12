#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <dirent.h>
#include <errno.h>

/*
Including all the libraries
*/

int min(int a,int b)
{
    if(a<b)return a;
    return b;
}


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

    char buf[51];//=(char *)malloc(10*sizeof(char));
    char *buffer=NULL;
    int len=0;
    int i=0;
    while(1)
    {
        int clilen = sizeof(cli_addr);
        int newsockfd=accept(sockfd,(struct sockaddr *)&cli_addr,&clilen);

        if(newsockfd<0)
        {
            printf("Unable to connect to client, Accept Error\n");
            return 0;
        }
        
        if(fork()==0)
        {
            close(sockfd);
            send(newsockfd,"LOGIN:\0",7,0);
            int lk=recv(newsockfd,buf,26,0);
            int lenn;
            buffer=(char *)malloc(len*sizeof(char));
            for(int j=0;j<lk;j++)
                buffer[j]=buf[j];
                lenn=lk;
                while(!(lk==0 || buf[lk-1]=='\n' || buf[lk-1]=='\0'))
                {
                    lk=recv(newsockfd, buf, 10, 0);
                    char *temp=(char *)malloc((lenn+lk)*sizeof(char));
                    for(int k=0;k<lenn;k++)
                    {
                        temp[k]=buffer[k];
                    }
                    for(int k=len;k<lenn+lk;k++)
                    {
                        temp[k]=buf[k-len];
                    }
                    lenn+=lk;
                    char *t2=buffer;
                    buffer=temp;
                    free(t2);
                }
            //printf("%s\n",buffer);

            FILE  *file_ptr=fopen ("users.txt", "r");
            int t=0;
            while (fgets (buf, 26, file_ptr)!=NULL)
            {
                //printf("buf %s %d\n",buf,strlen(buf));
                if(buf[strlen(buf)-1]=='\n')
                 buf[strlen(buf)-1]='\0';
                else
                buf[strlen(buf)]='\0';
                if(strcmp(buf,buffer)==0)
                {
                    t=1;
                    send(newsockfd,"FOUND\0",6,0);
                    break;
                }
            }
            if(t==0)
            {
                send(newsockfd,"NOT-FOUND\0",10,0);
                free(buffer);
                close(newsockfd);
                return 0;
            }
            buffer[i]='\0';

            free(buffer);
            i=0;
            while(1)
            {
                for(int j=0; j<=50; j++) buf[j] = '\0';
                int l=recv(newsockfd, buf, 50, 0);
                if(l<=0)break;
                buffer=(char *)malloc((l+1)*sizeof(char));
                for(int j=0;j<l;j++)
                buffer[j]=buf[j];
                len=l;
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
            
                if(buffer[0]=='c' && buffer[1]=='d')
                {
                    if(buffer[2]=='\0')
                    {
                        //printf("Dj Bravo\n");
                        send(newsockfd,"Directory Changed Successfully\0",50,0);
                    }
                    else
                    {
                        if(buffer[2]!=' ')
                        {
                            send(newsockfd,"$$$$\0",5,0);
                        }
                        else
                        {
                            int k=2;

                            while(buffer[k]==' ')k++;
                            int ret=chdir(buffer+k);
                            //printf("%s",buffer+3);
                            if(ret<0)
                            {
                                send(newsockfd,"####\0",5,0);
                            }
                            else
                            {
                                send(newsockfd,"Directory Changed Successfully\0",50,0);
                            }
                        }
                        
                    }
                    
                    free(buffer);
                }
                else if(buffer[0]=='p' && buffer[1]=='w' && buffer[2]=='d')
                {
                    if(buffer[3]!='\0')
                    {
                        send(newsockfd,"$$$$\0",5,0);
                    }
                    else
                    {
                        free(buffer);
                        buffer=NULL;
                        if((buffer=getcwd(buffer,0))==NULL)
                        {
                            send(newsockfd,"####\0",5,0);
                        }
                        else
                        {
                            //printf("pwd answer %s %d\n",buffer,strlen(buffer));
                            for(int i=0;i<strlen(buffer)+1;i+=50)
                            send(newsockfd, buffer+i, min(strlen(buffer)+1-i,50),0);
                        }
                    }
                    
                    free(buffer);
                }
                else if(buffer[0]=='d' && buffer[1]=='i' && buffer[2]=='r')
                {
                    DIR *dir;
                    int k=3;

                    while(buffer[k]==' ')k++;
                    if ((dir = opendir(buffer+k)) == NULL)
                    {
                        send(newsockfd,"####\0",5,0);
                    }
                    else
                    {
                        struct dirent *entry;
                        char * ans=NULL;
                        int l=0;
                        while ((entry = readdir(dir)) != NULL) 
                        {
                            if (entry->d_name[0] != '.') {
                                if(ans==NULL)
                                {
                                    ans=(char *)malloc((strlen(entry->d_name)+1)*sizeof(char));
                                    for(int op=0;op<strlen(entry->d_name);op++)
                                    {
                                        ans[op]=entry->d_name[op];
                                    }
                                    l=strlen(entry->d_name);
                                    ans[l]='\0';
                                    //ans=entry->d_name;
                                }
                                else
                                {
                                    ans[l]='\n';
                                    char *temp=(char *)malloc((l+strlen(entry->d_name)+2)*sizeof(char));
                                    for(int op=0;op<=l;op++)
                                    {
                                        temp[op]=ans[op];
                                    }
                                    for(int op=1;op<=strlen(entry->d_name);op++)
                                    {
                                        temp[op+l]=entry->d_name[op-1];
                                    }
                                    temp[l+strlen(entry->d_name)+1]='\0';
                                    l+=strlen(entry->d_name)+1;
                                    char *t2=ans;
                                    ans=temp;
                                    free(t2);
                                }
                            }
                        }
                        //strncat(ans,"\0",1);
                        for(int i=0;i<strlen(ans)+1;i+=50)
                        send(newsockfd, ans+i, min(strlen(ans)+1-i,50),0);
                        //printf("%s",ans);
                        free(ans);
                    }
                    free(buffer);
                }
                else
                {
                    send(newsockfd,"$$$$\0",5,0);
                    free(buffer);
                }
                
            }
            close(newsockfd);
            return 0;
            

        }
        close(newsockfd);
    }
    
    close(sockfd);
    return 0;

}