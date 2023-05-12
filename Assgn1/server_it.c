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

double operation(double a1,double a2,char op)
{
    if(op=='+')
    {
        return a1+a2;
    }
    if(op=='-')
    {
        return a1-a2;
    }
    if(op=='*')
    {
        return a1*a2;
    }
    if(op=='/')
    {
        return a1/a2;
    }
    return 0;

}

double solv(char* s)
{
    int cnt=0;
    char *ptr;
    char op='\0';
    double a=0;
    ptr=s;

    while(*ptr!='\0' && *ptr!='\n')
    {
        if(*ptr == ' ')
        {
            ptr++;
        }
        else if(*ptr=='+' || *ptr=='-' || *ptr=='*' || *ptr=='/')
        {
            op=*ptr;
            ptr++;
        }
        else if(*ptr == '(')
        {
            int len=-1;
            char *ptr2=ptr;
            while(*ptr!=')')
            {
                len++;
                *ptr++;
            }
            ptr++;
            //printf("")
            char *s1=(char *)malloc((len+1)*sizeof(char));
            s1[len]='\0';
            for(int i=1;i<=len;i++)
            {
                s1[i-1]=*(ptr2+i);
            }
            double ret=solv(s1);
            //printf("%s\n",s1);
            if(cnt==0)
            {
                a=ret;
                cnt++;
            }
            else if(cnt==1)
            {
                a=operation(a,ret,op);
                cnt=1;
            }
            free(s1);
        }
        else if(*ptr >='0' && *ptr<='9')
        {
            char *ptr2;
            double ret=strtod(ptr,&ptr2);
            if(cnt==0)
            {
                a=ret;
                cnt++;
            }
            else if(cnt==1)
            {
                a=operation(a,ret,op);
            }
            //printf("%lf\n",a);
            //printf("ptr val %d\n",*ptr2);
            ptr=ptr2;
            //printf("ptr val %d\n",*ptr);
            //break;
        }
        //printf("ptr val %d\n",*ptr);
    }
    return a;
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

    char buf[10];//=(char *)malloc(10*sizeof(char));
    char *buffer=(char *)malloc(10*sizeof(char));
    int len=0;
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
        for(int j=0; j< 10; j++) buf[j] = '\0';
		while(1)
        {
            for(int j=0; j< 10; j++) buf[j] = '\0';
            int l=recv(newsockfd, buf, 10, 0);
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
            //printf("%s",buffer);
            //printf("jay Shree Ram\n");
            double ans=solv(buffer);
            //printf("%lf\n",ans);

            char *snd;
            int dec, sign;

            // Function Call
            snd = ecvt(ans, 25, &dec, &sign);
            //printf("The converted string value is: %c%c.%sX10^%d\n",sign == 0 ? '+' : '-', '0', snd, dec);
            char exp[50];
            for(int mo=0;mo<50;mo++)exp[i]='\0';
            sprintf(exp, "%d", dec);
            int lt=3;
            lt+=strlen(exp);
            lt+=strlen(snd);
            lt+=4;

            char *sans=(char *)malloc((lt+1)*sizeof(char));
            sans[0]=sign == 0 ? '+' : '-';
            sans[1]='0';
            sans[2]='.';
            sans[lt]='\0';
            int g=0;
            for(;g<strlen(snd);g++)
            {
                sans[g+3]=snd[g];
            }
            sans[g+3]='X';
            sans[g+4]='1';
            sans[g+5]='0';
            sans[g+6]='^';

            for(int ju=0;ju<strlen(exp);ju++)
            {
                sans[g+7+ju]=exp[ju];
            }
            //printf("Converted the string %s\n",sans);
            send(newsockfd, sans, lt+1, 0);
            // free(sans);
            // //free(exp);
            // free(snd);
            // free(buffer);
        }
        close(newsockfd);
    }
    
    close(sockfd);
    return 0;

}