/* From Shuichi Yukita's book. client.c client program */


#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#define PORT (u_short) 10001 
#define BUF_LEN 100

char hostname[]="localhost";
char buf[BUF_LEN];

int main()
{
    struct hostent *servhost;
    struct sockaddr_in server;
    int s;

    // 相手のアドレスをserverという構造体にセットする
    servhost = gethostbyname(hostname);
    bzero((char *)&server,sizeof(server));
    server.sin_family = AF_INET;
    server.sin_port = PORT;
    bcopy(servhost->h_addr,
    (char *)&server.sin_addr,servhost->h_length);

    // ソケットを獲得する
    s = socket(AF_INET,SOCK_STREAM,0);

    // 相手のアドレスを指定して接続の要求
    connect(s,&server,sizeof(server));

    write(1,"待ちなさい.\n",strlen("待ちなさい.\n")); fflush(NULL);
    // 対話のループ
    do{
        int n;
        n=read(s,buf,BUF_LEN);
        write(1,buf,n); fflush(NULL);
        n=read(0,buf,BUF_LEN);
        write(s,buf,n); fflush(NULL);
    }while(strncmp(buf,"quit",4)!=0);

    close(s);
}
