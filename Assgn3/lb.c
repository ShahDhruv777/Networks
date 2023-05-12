#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h> 
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <poll.h>

/*
Including all the libraries
*/

int main(int argc,char* argv[])
{
    int sockfd_send,sockfd_r1,sockfd_r2;
    struct sockaddr_in serv1_addr,serv2_addr,serv_addr,cli_addr;

    int x;
    //argv[1]

    if((sockfd_send=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        printf("Socket Creation for lb Failed\n");
        exit(0);
    }
    else
    {
        printf("Socket created for lb\n");
        //exit(0);
    }

    //sscanf(argv[1], "%d", &x);
    x=atoi(argv[1]);
    printf("%d \n",x);
    serv_addr.sin_family=AF_INET;
    serv_addr.sin_port=htons(x);
    serv_addr.sin_addr.s_addr=INADDR_ANY;
    x=atoi(argv[2]);
    x=atoi(argv[3]);

    if((bind(sockfd_send,(struct sockaddr *)&serv_addr,sizeof(serv_addr)))<0)
    {
        perror("Binding Failed\n");
        exit(0);
    }
    else
    {
        printf("Binding done\n");
    }

    listen(sockfd_send,5);
    int i=1;
    int time_d=5000;

    while(i<2)
    {
        char buf[110];
        int load1=0,load2=0;
        char * rl;
        int l1_load;
        int len_1_load;

        time_t start_t, end_t;
            int diff_int=0;
            double diff_t;
            time(&start_t);

            struct pollfd fdset;
            fdset.fd=sockfd_send;
            fdset.events=POLLIN;
            int ret=poll(&fdset,1,5000);
            if(ret<0)
            {
                printf("There is an error in poll fundtion\n");
                return 0;
            }
            if(ret==0)
            {
                time_d=5000;

                if(time_d==5000)
                {
                    if((sockfd_r1=socket(AF_INET,SOCK_STREAM,0))<0)
                    {
                        printf("Socket Creation Failed for server 1\n");
                        exit(0);
                    }
                    else
                    {char buf[110];
                        printf("Socket created for server 1\n");
                        //exit(0);
                    }

                    x=atoi(argv[2]);
                    //printf("Server Address of 1 is %d\n",x);
                    serv1_addr.sin_family=AF_INET;
                    serv1_addr.sin_port=htons(x);
                    inet_aton("127.0.0.1", &serv1_addr.sin_addr);

                    if((connect(sockfd_r1,(struct sockaddr *)&serv1_addr,sizeof(serv1_addr))<0))
                    {
                        printf("Unable to connect to server 1\n");
                        exit(0);
                    }

                    

                    for(int i=0;i<110;i++)
                    {
                        buf[i]='\0';
                    }

                    //printf("Here we are asking for send load\n");
                    //printf("%d\n",send(sockfd_r1,"Send Load\0",strlen("Send Load\0")+1,0));
                    send(sockfd_r1, "Send Load\0", 10, 0);
                    char * rl;
                    l1_load=recv(sockfd_r1,buf,10,0);
                    //printf("%s\n",buf);
                    rl=(char *)malloc(l1_load*sizeof(char));
                    len_1_load=l1_load;

                    for(int i=0;i<l1_load;i++)
                    {
                        rl[i]=buf[i];
                    }

                    while(!(l1_load==0 || buf[l1_load-1]=='\0'))
                    {
                        l1_load=recv(sockfd_r1, buf, 50, 0);
                            if(l1_load==0)break;
                            char *temp=(char *)malloc((len_1_load+l1_load)*sizeof(char));
                            for(int k=0;k<len_1_load;k++)
                            {
                                temp[k]=rl[k];
                            }
                            for(int k=len_1_load;k<len_1_load+l1_load;k++)
                            {
                                temp[k]=buf[k-len_1_load];
                            }
                            len_1_load+=l1_load;
                            char *t2=rl;
                            rl=temp;
                            free(t2);
                    }

                    load1=atoi(rl);
                    free(rl);
                    printf("load recieved from server port :%d is %d\n",x,load1);


                    if((sockfd_r2=socket(AF_INET,SOCK_STREAM,0))<0)
                    {
                        printf("Socket Creation Failed\n");
                        exit(0);
                    }
                    else
                    {
                        printf("Socket created for server 2\n");
                        //exit(0);
                    }

                    sscanf(argv[3], "%d", &x);
                    serv2_addr.sin_family=AF_INET;
                    serv2_addr.sin_port=htons(x);
                    inet_aton("127.0.0.1", &serv2_addr.sin_addr);

                    if((connect(sockfd_r2,(struct sockaddr *)&serv2_addr,sizeof(serv2_addr))<0))
                    {
                        printf("Unable to connect to server 2\n");
                        exit(0);
                    }

                    for(int i=0;i<110;i++)
                    {
                        buf[i]='\0';
                    }
                    //printf("Here we are asking for send load\n");
                    //printf("%d\n",send(sockfd_r2,"Send Load\0",strlen("Send Load\0")+1,0));
                    send(sockfd_r2, "Send Load\0", 10, 0);
                    l1_load=recv(sockfd_r2,buf,10,0);
                    //printf("%s\n",buf);
                    rl=(char *)malloc(l1_load*sizeof(char));
                    len_1_load=l1_load;

                    for(int i=0;i<l1_load;i++)
                    {
                        rl[i]=buf[i];
                    }
                    while(!(l1_load==0 || buf[l1_load-1]=='\0'))
                    {
                        l1_load=recv(sockfd_r2, buf, 50, 0);
                        //printf("%d\n",l1_load);
                            if(l1_load==0)break;
                            char *temp=(char *)malloc((len_1_load+l1_load)*sizeof(char));
                            for(int k=0;k<len_1_load;k++)
                            {
                                temp[k]=rl[k];
                            }
                            for(int k=len_1_load;k<len_1_load+l1_load;k++)
                            {
                                temp[k]=buf[k-len_1_load];
                            }
                            len_1_load+=l1_load;
                            char *t2=rl;
                            rl=temp;
                            free(t2);
                    }

                    load2=atoi(rl);
                    free(rl);

                    printf("load recieved from server port :%d is %d\n",x,load2);
                    //printf("The loads are %d %d\n",load1,load2);
                    close(sockfd_r1);
                    close(sockfd_r2);
                }
            }
            if(ret>0)
            {
                if(fork()==0)
                {
                    int clilen=sizeof(cli_addr);
                    int newsockfd=accept(sockfd_send,(struct sockaddr *)&cli_addr,&clilen);
                    close(sockfd_send);
                    printf("HERE IT IS working with client\n");



                    if(load1<load2)
                    {

                    if((sockfd_r1=socket(AF_INET,SOCK_STREAM,0))<0)
                    {
                        printf("Socket Creation Failed for server 1\n");
                        exit(0);
                    }
                    else
                    {
                        printf("Socket created for server 1\n");
                        //exit(0);
                    }

                    x=atoi(argv[2]);
                    //printf("Server Address of 1 is %d\n",x);
                    serv1_addr.sin_family=AF_INET;
                    serv1_addr.sin_port=htons(x);
                    inet_aton("127.0.0.1", &serv1_addr.sin_addr);

                    if((connect(sockfd_r1,(struct sockaddr *)&serv1_addr,sizeof(serv1_addr))<0))
                    {
                        printf("Unable to connect to server 1\n");
                        exit(0);
                    }
                    printf("Sending client service request to server port %d\n",x);
                    send(sockfd_r1, "Send Time\0", 10, 0);
                    for(int i=0;i<110;i++)
                    {
                        buf[i]='\0';
                    }
                    l1_load=recv(sockfd_r1,buf,10,0);
                    //printf("%s\n",buf);
                    rl=(char *)malloc(l1_load*sizeof(char));
                    int len_1_load=l1_load;

                    for(int i=0;i<l1_load;i++)
                    {
                        rl[i]=buf[i];
                    }

                    while(!(l1_load==0 || buf[l1_load-1]=='\0'))
                    {
                        
                        l1_load=recv(sockfd_r1, buf, 50, 0);
                            if(l1_load==0)break;
                            char *temp=(char *)malloc((len_1_load+l1_load)*sizeof(char));
                            for(int k=0;k<len_1_load;k++)
                            {
                                temp[k]=rl[k];
                            }
                            for(int k=len_1_load;k<len_1_load+l1_load;k++)
                            {
                                temp[k]=buf[k-len_1_load];
                            }
                            len_1_load+=l1_load;
                            char *t2=rl;
                            rl=temp;
                            free(t2);
                    }

                    send(newsockfd,rl,strlen(rl)+1,0);
                    free(rl);
                    close(sockfd_r1);
                }
                else
                {
                    if((sockfd_r2=socket(AF_INET,SOCK_STREAM,0))<0)
                    {
                        printf("Socket Creation Failed\n");
                        exit(0);
                    }
                    else
                    {
                        printf("Socket created for server 2\n");
                        //exit(0);
                    }

                    sscanf(argv[3], "%d", &x);
                    serv2_addr.sin_family=AF_INET;
                    serv2_addr.sin_port=htons(x);
                    inet_aton("127.0.0.1", &serv2_addr.sin_addr);

                    if((connect(sockfd_r2,(struct sockaddr *)&serv2_addr,sizeof(serv2_addr))<0))
                    {
                        printf("Unable to connect to server 2\n");
                        exit(0);
                    }
                    printf("Sending client service request to server port %d\n",x);
                    send(sockfd_r2, "Send Time\0", 10, 0);
                    l1_load=recv(sockfd_r2,buf,10,0);
                    //printf("%s\n",buf);
                    rl=(char *)malloc(l1_load*sizeof(char));
                    len_1_load=l1_load;

                    for(int i=0;i<l1_load;i++)
                    {
                        rl[i]=buf[i];
                    }

                    while(!(l1_load==0 || buf[l1_load-1]=='\0'))
                    {
                        l1_load=recv(sockfd_r2, buf, 50, 0);
                            if(l1_load==0)break;
                            char *temp=(char *)malloc((len_1_load+l1_load)*sizeof(char));
                            for(int k=0;k<len_1_load;k++)
                            {
                                temp[k]=rl[k];
                            }
                            for(int k=len_1_load;k<len_1_load+l1_load;k++)
                            {
                                temp[k]=buf[k-len_1_load];
                            }
                            len_1_load+=l1_load;
                            char *t2=rl;
                            rl=temp;
                            free(t2);
                    }

                    send(newsockfd,rl,strlen(rl)+1,0);
                    free(rl);
                    close(sockfd_r2);
                }




                    
                    close(newsockfd);
                    return 0;
                }
                    time(&end_t);
                    diff_t = difftime(end_t, start_t);
                    if(diff_t>0)
                    //printf("%f\n",diff_t);
                    diff_int=(int)(diff_t*1000);
                    time_d-=diff_int;
                    if(time_d<=0)
                    time_d=5000;
            }
        }
        

    close(sockfd_send);
    return 0;

}