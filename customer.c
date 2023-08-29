#include "service.h"


volatile int the_sign;
int sign1;
int sfd_copy;
MESSAGE exit_1 = {0};
 //MESSAGE buf1 = {0};

/************************************************/
////      钩子函数        /////////////
/************************************************/

void cleanup(void)
{
    
    exit_1.mes_type2 = 3;
    
    write(sfd_copy,&exit_1,sizeof(MESSAGE));
    sleep(2);
    close(sfd_copy);
    
    printf("程序退出\n");

}

/************************************************/
////      指令函数        /////////////
/************************************************/

void sigint_handler(int signal)
{
    printf("Received SIGINT signal\n");
    the_sign = 1;
    exit(0);
}

/************************************************/
////      进程函数        /////////////
/************************************************/

void *data(void *argv)
{
    int sfd = * (int *)argv; 
    MESSAGE buf;
    socklen_t len = sizeof(MESSAGE);
    while(1){
        
        if(the_sign == 1){         
            break;
        }
        else
        {
           
                 read(sfd,&buf,len);
    
                 if(buf.mes_type2 == 1 && sign1 == 1){
                    printf("\n\t\t私聊消息: %s: %s\n",buf.name,buf.mes);
                    printf("\n");
                    printf("--------------------------------------------------\n");
                  }
                  else if(buf.mes_type2 == 2 && sign1 ==2){
                    printf("\n\t\t群聊消息: %s: %s\n",buf.name,buf.mes);
                    printf("\n");
                    printf("--------------------------------------------------\n");
                  }
                  else if(buf.mes_type2 == 4){
                        printf("%d向%d发送了%s       \t%s\n",buf.acc_user,buf.acc_user1,buf.mes,buf.mes_time);
                        //printf("--------------------------------------------------\n");    

                  }
                  else if(buf.mes_type2 == 5 ){

                        printf("%d发送了%s       \t%s\n",buf.acc_user,buf.mes,buf.mes_time);
                        //printf("--------------------------------------------------\n");  
                  }

        }

    }
    printf("%s\n",buf.mes);
    
    pthread_exit(NULL);
}


/************************************************/
////      注册函数        /////////////
/************************************************/

void login1(int sfd,MESSAGE *buf)
{
    printf("请输入昵称\n");
    char name_1[32];
loop1:   
    scanf("%s",name_1);
    getchar();
    if(strlen(name_1)>31){
        printf("名字过长请重新输入\n");
        goto loop1;
    }
    strcpy(buf->name,name_1);
    printf("请输入密码\n");
    char password_1[10];
loop2:  
    scanf("%s",password_1);
    getchar();
    if(strlen(password_1)>10){
        printf("密码过长请重新输入\n");
        goto loop2;        
    }
    strcpy(buf->password,password_1);
    
    printf("请输入你自己想要创建的账户号码\n");
    int user_num;
loop3:
   
    
    if (scanf("%d", &user_num) != 1 || user_num<100) {
        printf("你的输入有误，请重新输入\n");
        while (getchar() != '\n');
        goto loop3;
    }
    
    if(user_num>999999999){
         printf("账户过长请重新输入\n");
         goto loop3;
    }
    buf->acc_user = user_num;
    socklen_t len = sizeof(MESSAGE);
    write(sfd,buf,len);
    read(sfd,buf,len);
    printf("%s\n",buf->mes);
}

/************************************************/
////      登录函数        /////////////
/************************************************/

void enter1(int sfd,MESSAGE *buf)
{
    printf("请输入你的账户号码\n");
    int user_num;
loop4:
    if (scanf("%d", &user_num) != 1 ) {
        printf("你的输入有误，请重新输入\n");
        while (getchar() != '\n');
        goto loop4;
    }
    if(user_num>999999999){
        printf("账户过长请重新输入\n");
        goto loop4;
    }
    buf->acc_user = user_num;
     
    printf("请输入密码\n");
    char password_1[10];
loop5:  
    scanf("%s",password_1);
    getchar();
    if(strlen(password_1)>10){
        printf("密码过长请重新输入\n");
        goto loop5;        
    }
    strcpy(buf->password,password_1);
    socklen_t len = sizeof(MESSAGE);

    buf->mes_type2 = 0;
    write(sfd,buf,len);
    read(sfd,buf,len);
    printf("%s\n",buf->mes);
    if(strcmp(buf->mes,"登陆成功\n")!=0){
        return;
    }
    exit_1 = *buf;
    sfd_copy = sfd;
    while (1) {
        printf("私聊        \t[1]\n");
        printf("加入群聊    \t[2]\n");
        printf("退出登录   \t[3]\n");
        printf("查看私聊记录 \t[4]\n");
        printf("查看群聊登录 \t[5]\n");
        printf("设置禁言     \t[6]\n");
        printf("解除禁言     \t[7]\n");
        printf("设置管理员  \t[8]\n");
        int type_num;
     loop9:  
        if (scanf("%d", &type_num) != 1 || type_num>8 || type_num<1) {
            printf("你的输入有误，请重新输入\n");
            while (getchar() != '\n');
            goto loop9;
        }
        buf->mes_type2 = type_num;
        switch (buf->mes_type2) {
            case 1: 
                    {
                                    sign1 = 1;
                                    pthread_t tid;
                                    the_sign = 0;
                                    pthread_create(&tid, NULL, data, &sfd);
                                    pthread_detach(tid);
                                    sleep(1);
                                    printf("--------------------------------------------------\n");
                                    printf("以上是未读消息\n");
                                    printf("你可以随时输入任意建开始回复消息\n");
                                    gets();
                                    while (getchar() != '\n');
                                    printf("请输入你要私聊的账号\n");
                                    int num;
                                    //scanf("%d", &(buf->acc_user1));
                                lll:
                                    
                                    if (scanf("%d", &num) != 1 ) {
                                            printf("你的输入有误，请重新输入\n");
                                            while (getchar() != '\n');
                                            goto lll;
                                    }
                                    buf->acc_user1 = num;
                                    while (1) {
                                        printf("--------------------------------------------------\n");
                                        scanf("%s",buf->mes);
                                        getchar();
                                        write(sfd, buf, len);
                                        if (strcmp(buf->mes, "quit") == 0) {
                                            the_sign = 1;
                                            //buf->mes_type2 = 0;
                                            sign1 = 0;
                                            sleep(1); 
                                            // int ret = pthread_join(tid, NULL);
                                            // if (ret != 0) {
                                            //         printf("Failed to join thread\n");
                                            //         break;
                                            // }
                                            break;
                                        }
                                        
                                    }
                                    break;
                    }
            case 2: 
                    {
                        
                                sign1 = 2;
                                the_sign = 0;
                                pthread_t tid2;
                                pthread_create(&tid2, NULL, data, &sfd);
                                pthread_detach(tid2);
                                sleep(1);
                                printf("-------------------------------------------------------\n");
                                printf("以上是未读消息\n");     
                                printf("欢迎加入群聊\n");
                                printf("你可以随时输入任意建开始回复消息\n");
                                gets();
                                while (getchar() != '\n');
                                printf("-------------------------------------------------------\n");
                                while (1) {
                                        scanf("%s", buf->mes);
                                        write(sfd, buf, len);
                                        if (strcmp(buf->mes, "quit") == 0) {
                                            the_sign = 1;
                                            sign1 = 0;
                                        // buf->mes_type2 = 0;
                                            sleep(2);
                                            break;
                                        }
                                }
                                break;
                    }
            case 3: 
                    {

                                printf("请问你要退出吗\n");
                                char acc[6];
                                printf("如果是请输入quit\n");
                                scanf("%s",acc);
                                getchar();
                                if(strcmp(acc,"quit")==0){
                                        write(sfd, buf, len);
                                        sleep(1);
                                        return ;
                                }
                                break;

                    }
            case 4:
                    {
                                
                                the_sign = 0;
                                pthread_t tid2;
                                pthread_create(&tid2, NULL, data, &sfd);
                                pthread_detach(tid2);
                                strcpy(buf->mes,"0");
                                write(sfd, buf, len);
                                sleep(3);
                                printf("如果退出请输入quit\n");
                                char acc[6];
                            loo:   
                                scanf("%s",acc);
                                getchar();
                                
                                if(strcmp(acc,"quit")==0){
                                        strcpy(buf->mes,acc);
                                        write(sfd, buf, len);
                                        the_sign = 1;
                                        sleep(1);
                                        
                                }
                                else{
                                    printf("请重新输入\n");
                                    goto loo;
                                }
                                
                                
                        break;
                    }
            case 5:
                    {
                        the_sign = 0;
                                pthread_t tid2;
                                pthread_create(&tid2, NULL, data, &sfd);
                                pthread_detach(tid2);
                                strcpy(buf->mes,"0");
                                write(sfd, buf, len);
                                sleep(3);
                                printf("如果退出请输入quit\n");
                                char acc[6];
                            lo:   
                                scanf("%s",acc);
                                getchar();
                                
                                if(strcmp(acc,"quit")==0){
                                        strcpy(buf->mes,acc);
                                        write(sfd, buf, len);
                                        the_sign = 1;
                                        sleep(1);
                                        
                                }
                                else{
                                    printf("请重新输入\n");
                                    goto lo;
                                }
                                break;
                    }
                    
            case 6:
                    {
                        printf("请输入你要禁言的账号\n");
                    III:
                        if (scanf("%d", &user_num) != 1 ) {
                            printf("你的输入有误，请重新输入\n");
                            while (getchar() != '\n');
                            goto III;
                        }
                        buf->acc_user1 = user_num;
                        write(sfd, buf, len);
                        read(sfd,buf,len);
                        printf("%s",buf->mes);



                        break;
                    }
            case 7:
                    {
                        printf("请输入你要解除禁言的账号\n");
                    IIII:
                        if (scanf("%d", &user_num) != 1 ) {
                            printf("你的输入有误，请重新输入\n");
                            while (getchar() != '\n');
                            goto IIII;
                        }
                        buf->acc_user1 = user_num;
                        write(sfd, buf, len);
                        read(sfd,buf,len);
                        printf("%s",buf->mes);


                        break;

                    }
            case 8:
                    {
                        printf("请输入你要设置为管理员的账号\n");
                    IIIII:
                        if (scanf("%d", &user_num) != 1 ) {
                            printf("你的输入有误，请重新输入\n");
                            while (getchar() != '\n');
                            goto IIIII;
                        }
                        buf->acc_user1 = user_num;
                        write(sfd, buf, len);
                        read(sfd,buf,len);
                        printf("%s",buf->mes);


                        break;

                    }
        }
    }

}

/************************************************/
////      注销函数        /////////////
/************************************************/

void logout1(int sfd,MESSAGE *buf)
{
    
    printf("请输入你要注销的账户\n"); 
    int user_num;
loop6:
    if (scanf("%d", &user_num) != 1 ) {
        printf("你的输入有误，请重新输入\n");
        while (getchar() != '\n');
        goto loop6;
    }
    if(user_num>999999999){
        printf("账户过长请重新输入\n");
        goto loop6;
    }
    buf->acc_user = user_num;

    printf("请输入昵称\n");
    char name_1[32];
loop7:   
    scanf("%s",name_1);
    getchar();
    if(strlen(name_1)>31){
        printf("名字过长请重新输入\n");
        goto loop7;
    }
    strcpy(buf->name,name_1);
    printf("请输入密码\n");
    char password_1[10];
loop8:  
    scanf("%s",password_1);
    getchar();
    if(strlen(password_1)>10){
        printf("密码过长请重新输入\n");
        goto loop8;        
    }
    strcpy(buf->password,password_1);
    socklen_t len = sizeof(MESSAGE);
    write(sfd,buf,len);
    read(sfd,buf,len);
    printf("%s\n",buf->mes);
}



int main()
{
     if (atexit(cleanup) != 0) {
        fprintf(stderr, "Failed to register cleanup function.\n");
        return 1;
    }
    signal(2, sigint_handler);
    struct sockaddr_in service_data;
    
    //创建套接字
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd<0){
        perror("socket");
        return -1;
    }
    //设置io复用
    int flag = 1;      
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int));

    service_data.sin_family = AF_INET;
    service_data.sin_port = htons(9999);
    service_data.sin_addr.s_addr = inet_addr("192.168.43.28"); 
    int ret = connect(sfd,(struct sockaddr*)&service_data,sizeof(service_data));
    if(ret<0){
        perror("connect");
        return -1;
    }
    while(1){
        MESSAGE buf1 = {0};
        printf("请问你要做什么\n");
        printf("1\t注册\n");
        printf("2\t登陆\n");
        printf("3\t注销\n");
        
     
        int a;
     l:
        if (scanf("%d", &a) != 1 || a>3 || a<1) {
            printf("你的输入有误，请重新输入\n");
            while (getchar() != '\n');
            goto l;
        }
       
        buf1.mes_type = a;
        switch(a)
        {
            case 1:login1(sfd,&buf1);break;
            case 2:enter1(sfd,&buf1);break;
            case 3:logout1(sfd,&buf1);break;

        }
    }
    


    return 0;
}