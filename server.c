/* From Shuichi Yukita's book. server.c server program */


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
    struct hostent *myhost;
    struct sockaddr_in me;
    int s_waiting, s;

    /* 自分のアドレスをme という変数に作り出す。*/
    myhost = gethostbyname (hostname); 
    bzero((char *)&me, sizeof(me));
    me.sin_family = AF_INET; 
    me.sin_port = PORT;
    bcopy (myhost->h_addr,
    (char *)&me.sin_addr, myhost->h_length);

    /* ソケットを獲得し、自分のアドレスを結び付ける。 */
    s_waiting = socket (AF_INET, SOCK_STREAM, 0);
    bind(s_waiting, &me, sizeof(me));

    /* 接続の確立 */
    listen (s_waiting, 1);
    s = accept (s_waiting, NULL, NULL);
    close(s_waiting);

    write(1, "挨拶を送ろう.\n",strlen("挨拶を送ろう.\n")); fflush(NULL);
    /* 対話のループ　*/
    do{
        int n;
        n=read(0,buf,BUF_LEN);
        write(s,buf,n); fflush(NULL);
        n=read(s,buf,BUF_LEN);
        write(1,buf,n); fflush(NULL);
    }while(strncmp(buf,"quit",4)!=0);

    close(s);
}















