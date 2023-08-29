#include "service.h"

/************************************************/
////      注册函数        /////////////
/************************************************/

int login(MESSAGE *buf)
{
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
                        //
                        //  MYSQL *conn1; // MySQL连接对象
    // MYSQL *conn2; // MySQL连接对象
    int account = buf->acc_user;
    char name[32] = {0};
    strcpy(name, buf->name);
    char password[10] = {0};
    strcpy(password, buf->password);

    char path[200];
    sprintf(path, "SELECT * FROM user where account = %d;", account);
    mysql_query(conn, path);
    res = mysql_use_result(conn);

    // 检查结果集中是否存在行
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        printf("该信息已存在，程序退出\n");
        mysql_free_result(res);
        // mysql_close(conn);
        return -1;
    }

    char query[1000];
    sprintf(query, "insert into user (name,account,password) values ('%s',%d,'%s');", name, account, password);
    // 存入数据
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "信息存入失败: %s\n", mysql_error(conn));
        return -1;
    }
    // mysql_free_result(res); // 释放结果对象

    sleep(1);
    return 0;
}

/************************************************/
////      注销函数        /////////////
/************************************************/

int logout(MESSAGE *buf)
{
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象

    // MYSQL *conn1; // MySQL连接对象
    // MYSQL *conn2; // MySQL连接对象
    int account = buf->acc_user;
    char name[32] = {0};
    strcpy(name, buf->name);
    char password[10] = {0};
    strcpy(password, buf->password);

    // 拼接查找
    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where account = %d and name = '%s' and password = '%s';", account, name, password);
    mysql_query(conn, path);
    res = mysql_use_result(conn);
    char query[1000] = {0};

    // 检查结果集中是否存在行
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        mysql_free_result(res); // 释放结果对象
        sprintf(query, "delete from user where account = %d;", account);
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "信息删除失败: %s\n", mysql_error(conn));
            return -1;
        }
    }
    else
    {
        return -1;
    }

    sleep(1);

    return 0;
}

/************************************************/
////      登录函数        /////////////
/************************************************/

int enter(MESSAGE *buf, int ID)
{
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象

    // MYSQL *conn1; // MySQL连接对象
    //  MYSQL *conn2; // MySQL连接对象
    int account = buf->acc_user;

    char password[10] = {0};
    strcpy(password, buf->password);

    // 拼接查找
    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where account = %d and password = '%s';", account, password);
    mysql_query(conn, path);
    res = mysql_use_result(conn);
    char query[1000] = {0};
    // 检查结果集中是否存在行
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        strcpy(buf->name, row[0]);
        mysql_free_result(res); // 释放结果对象
        sprintf(query, "update user set ID = %d where account = %d;", ID, account);
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "ID存入失败: %s\n", mysql_error(conn));
            return -1;
        }
    }
    else
    {
        return -1;
    }

    return 0;
}

/************************************************/
////      退出登陆函数        /////////////
/************************************************/

int out(MESSAGE *buf, int ID)
{
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象

    // MYSQL *conn1; // MySQL连接对象
    // MYSQL *conn2; // MySQL连接对象
    int account = buf->acc_user;
    char password[10] = {0};
    strcpy(password, buf->password);

    // 拼接查找
    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where account = %d and password = '%s';", account, password);
    mysql_query(conn, path);
    res = mysql_use_result(conn);

    char query[100] = {0};
    // 检查结果集中是否存在行
    if ((row = mysql_fetch_row(res)) != NULL)
    {
        mysql_free_result(res); // 释放结果对象
        sprintf(query, "update user set ID = %d where account = %d;", -1, account);
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "ID存入失败: %s\n", mysql_error(conn));
            return -1;
        }
    }
    else
    {
        printf("123\n");
        return -1;
    }
    // mysql_free_result(res);
    return 0;
}

/************************************************/
////      人数查询函数        /////////////
/************************************************/

void PRINTF_DATA(void)
{
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象

    // MYSQL *conn1; // MySQL连接对象
    //  MYSQL *conn2; // MySQL连接对象
    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where ID>0;");
    mysql_query(conn, path);
    res = mysql_store_result(conn);
    my_ulonglong a = mysql_num_rows(res);
    printf("当前在线人数%ld\n", a);
    mysql_free_result(res); // 释放结果对象
}

/************************************************/
////      私聊函数        /////////////
/************************************************/

void SEND(MESSAGE *buf, int addr)
{
    char time_h[300];
    time_t mytime;
    time(&mytime);
    chat_time = localtime(&mytime);
    // MYSQL *conn; // MySQL连接对象
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象

    // MYSQL *conn1; // MySQL连接对象
    // MYSQL *conn2; // MySQL连接对象
    MESSAGE buf1 = *buf;

    if (strcmp(buf1.mes, "quit") == 0)
    {
        return;
    }
    int account = buf->acc_user1;
    char password[10] = {0};
    strcpy(password, buf->password);

    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where account = %d ;", account);
    mysql_query(conn, path);
    res = mysql_use_result(conn);

    if ((row = mysql_fetch_row(res)) != NULL)
    {
        // 释放结果对象
        mysql_free_result(res);
        int a = atoi(row[3]);

        // strcpy();
        // printf("%d\n",a);
        write(a, &buf1, sizeof(buf1));

        char query[1000];
        sprintf(time_h, "%d-%02d-%02d %02d时%02d分%02d秒", chat_time->tm_year + 1900, chat_time->tm_mon + 1, chat_time->tm_mday, chat_time->tm_hour, chat_time->tm_min, chat_time->tm_sec);
        sprintf(query, "insert into message (send,rec,mes,time) values (%d,%d,'%s','%s');", buf->acc_user, buf->acc_user1, buf->mes, time_h);
        // 存入数据
        if (mysql_query(conn, query))
        {
            fprintf(stderr, "信息存入失败: %s\n", mysql_error(conn));
            return;
        }
    }
    else
    {
        mysql_free_result(res);
        return;
    }
}

/************************************************/
////      群聊函数        /////////////
/************************************************/

void send_everyone(MESSAGE *buf, int addr)
{
    char time_h[300];
    time_t mytime;
    time(&mytime);
    chat_time = localtime(&mytime);
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_RES *res1; 
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = *buf;

     char path1[200] = {0};
    sprintf(path1, "SELECT * FROM silence where acc = %d;",buf1.acc_user);
    mysql_query(conn, path1);
    res1 = mysql_store_result(conn);
  
    if (res1 == NULL)
    {
        //printf("你没有权限\n");
        // strcpy(buf1.mes,"你已被禁言\n");
        // write(addr, &buf1, sizeof(buf1));
        fprintf(stderr, "Error storing query result: %s\n", mysql_error(conn));
        mysql_free_result(res1);
        return;
    }
     if ((row = mysql_fetch_row(res1)) != NULL){
        mysql_free_result(res1);
        strcpy(buf1.mes,"你已被禁言\n");
        write(addr, &buf1, sizeof(buf1));
        
        return;

     }
    //mysql_free_result(res1);

    char path[200] = {0};
    sprintf(path, "SELECT * FROM user where ID>0;");
    mysql_query(conn, path);
    res = mysql_use_result(conn);
    sprintf(time_h, "%d-%02d-%02d %02d时%02d分%02d秒", chat_time->tm_year + 1900, chat_time->tm_mon + 1, chat_time->tm_mday, chat_time->tm_hour, chat_time->tm_min, chat_time->tm_sec);
    while ((row = mysql_fetch_row(res)) != NULL)
    {
        int a = atoi(row[3]);
        write(a, &buf1, sizeof(buf1));
    }
    char query[1000];
    sprintf(query, "insert into message_1 (send,mes,time) values (%d,'%s','%s');", buf->acc_user, buf->mes, time_h);
    // 存入数据
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "信息存入失败: %s\n", mysql_error(conn));
        return;
    }
    return;
    // mysql_free_result(res);
}

/************************************************/
////      私聊信息        /////////////
/************************************************/

void personal(MESSAGE *buf, int addr)
{
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = {0};
    buf1 = *buf;
    char path[200] = {0};
    // printf("222\n");
    sprintf(path, "SELECT * FROM message where send = %d or rec = %d;", buf->acc_user, buf->acc_user);

    if (mysql_query(conn, path) != 0)
    {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(conn));

        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        fprintf(stderr, "Error storing query result: %s\n", mysql_error(conn));
        return;
    }
    // printf("222\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {

        strcpy(buf1.mes_time, row[3]);
        //  printf("%s\n",buf1.mes_time);
        strcpy(buf1.mes, row[2]);
        //   printf("%s\n",buf1.mes);
        buf1.acc_user = atoi(row[0]);
        //  printf("%d\n",buf1.acc_user);
        buf1.acc_user1 = atoi(row[1]);
        //  printf("%d\n",buf1.acc_user1);

        write(addr, &buf1, sizeof(buf1));
        //  printf("123\n");
    }

    // printf("111\n");
    return;
}

/************************************************/
////      群聊信息        /////////////
/************************************************/

void group(MESSAGE *buf, int addr)
{
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = {0};
    buf1 = *buf;
    char path[200] = {0};
    // printf("222\n");
    sprintf(path, "SELECT * FROM message_1 ;");

    if (mysql_query(conn, path) != 0)
    {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(conn));

        return;
    }

    res = mysql_store_result(conn);
    if (res == NULL)
    {
        fprintf(stderr, "Error storing query result: %s\n", mysql_error(conn));
        return;
    }
    // printf("222\n");
    while ((row = mysql_fetch_row(res)) != NULL)
    {

        strcpy(buf1.mes_time, row[2]);
        //  printf("%s\n",buf1.mes_time);
        strcpy(buf1.mes, row[1]);
        //   printf("%s\n",buf1.mes);
        buf1.acc_user = atoi(row[0]);
        //  printf("%d\n",buf1.acc_user);

        write(addr, &buf1, sizeof(buf1));
        //  printf("123\n");
    }

    // printf("111\n");
    return;
}

/************************************************/
////      设置禁言        /////////////
/************************************************/

void set_ban(MESSAGE *buf, int addr)
{
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = {0};
    buf1 = *buf;
    char path[200] = {0};
     printf("222\n");
    sprintf(path, "SELECT * FROM administrator where acc = %d;",buf->acc_user);

    if (mysql_query(conn, path) != 0)
    {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(conn));

        return;
    }
    res = mysql_store_result(conn);
    if (res == NULL)
    {
        //printf("你没有权限\n");
        // strcpy(buf1.mes,"你没有权限\n");
        // write(addr, &buf1, sizeof(buf1));
        return;
    }
    if ((row = mysql_fetch_row(res)) == NULL){
        mysql_free_result(res);
        strcpy(buf1.mes,"你没有权限\n");
        write(addr, &buf1, sizeof(buf1));
        
        return;

     }

    char query[1000];
    sprintf(query, "insert into silence(acc) values (%d);", buf->acc_user1);
    // 存入数据
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "信息存入失败: %s\n", mysql_error(conn));
        return;
    }
    

     strcpy(buf1.mes,"已禁言\n");

    write(addr, &buf1, sizeof(buf1));
    printf("222\n");

    return;
}

/************************************************/
////      删除禁言        /////////////
/************************************************/

void Unmute(MESSAGE *buf, int addr)
{
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = {0};
    buf1 = *buf;
    char path[200] = {0};
     printf("222\n");
    sprintf(path, "SELECT * FROM administrator where acc = %d;",buf->acc_user);

    if (mysql_query(conn, path) != 0)
    {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(conn));

        return;
    }
    res = mysql_store_result(conn);
    if (res == NULL)
    {
        //printf("你没有权限\n");
        // strcpy(buf1.mes,"你没有权限\n");
        // write(addr, &buf1, sizeof(buf1));
        return;
    }
    if ((row = mysql_fetch_row(res)) == NULL){
        mysql_free_result(res);
        strcpy(buf1.mes,"你没有权限\n");
        write(addr, &buf1, sizeof(buf1));
        
        return;

     }
    char query[1000];
    sprintf(query, "delete from silence where acc = %d;", buf->acc_user1);
    // 存入数据
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "信息删除失败: %s\n", mysql_error(conn));
        return;
    }
    

     strcpy(buf1.mes,"已解除禁言\n");

    write(addr, &buf1, sizeof(buf1));


    return;

}

/************************************************/
////      设置管理员        /////////////
/************************************************/

void set_administrator(MESSAGE *buf, int addr)
{
    MYSQL_RES *res;     // MySQL查询结果对象
    MYSQL_ROW row;      // MySQL行数据对象
    MYSQL_FIELD *field; // MySQL列信息对象
    MESSAGE buf1 = {0};
    buf1 = *buf;
    char path[200] = {0};
    // printf("222\n");
    sprintf(path, "SELECT * FROM lord where acc = %d;",buf->acc_user);

    if (mysql_query(conn, path) != 0)
    {
        fprintf(stderr, "Error executing query: %s\n", mysql_error(conn));

        return;
    }
    res = mysql_store_result(conn);
    if (res == NULL)
    {
        //printf("你没有权限\n");
        // strcpy(buf1.mes,"你没有权限\n");
        // write(addr, &buf1, sizeof(buf1));
        return;
    }
    if ((row = mysql_fetch_row(res)) == NULL){
        mysql_free_result(res);
        strcpy(buf1.mes,"你没有权限\n");
        write(addr, &buf1, sizeof(buf1));
        
        return;

     }

    char query[1000];
    sprintf(query, "insert into administrator(acc) values (%d);", buf->acc_user1);
    // 存入数据
    if (mysql_query(conn, query))
    {
        fprintf(stderr, "信息存入失败: %s\n", mysql_error(conn));
        return;
    }
    

     strcpy(buf1.mes,"已设置为管理员\n");

    write(addr, &buf1, sizeof(buf1));


    return;


}