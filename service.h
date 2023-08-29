#ifndef __SER_H
#define __SER_H

#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>
#include<arpa/inet.h>
#include<netinet/in.h>
#include<arpa/inet.h>
#include<unistd.h>
#include<sys/time.h>
#include<unistd.h>
#include<poll.h>
#include<sys/epoll.h>
#include <pthread.h>
#include <unistd.h>
#include<mysql/mysql.h>
#include<string.h>
#include <stdlib.h>
#include <signal.h>
#include<time.h>
#define MAX_EVENTS  50

typedef struct {
    char mes1[1000];
    char type[5];
    int __type;
    char account1[16];
}MES;


typedef struct message {
    char name[32];
    
    char password[10];
    int mes_type;
    int mes_type2;
    int acc_user;
    int acc_user1;
    char mes[256];
    char mes_time[50];
}MESSAGE;

int logout(MESSAGE *buf);
int login(MESSAGE *buf);
int enter(MESSAGE *buf,int ID);
void *data(void *argv);
void SEND(MESSAGE *buf,int addr);
void send_everyone(MESSAGE *buf,int addr);
int out(MESSAGE *buf,int ID);
void PRINTF_DATA(void);
void personal(MESSAGE *buf,int addr);
void group(MESSAGE *buf,int addr);
void set_ban(MESSAGE *buf,int addr);
void Unmute(MESSAGE *buf,int addr);
void set_administrator(MESSAGE *buf,int addr);
 MYSQL *conn; // MySQL连接对象
// MYSQL_RES *res; // MySQL查询结果对象
// MYSQL_ROW row; // MySQL行数据对象
 //MYSQL_FIELD *field; // MySQL列信息对象



struct tm *chat_time;
char time_h[300];

#endif