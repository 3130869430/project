#include "service.h"





int main()
{
    time_t mytime;
    time(&mytime);
    chat_time = localtime(&mytime);
    
    

    conn = mysql_init(NULL); // 初始化连接对象
    

    if (!mysql_real_connect(conn, "localhost", "root", "1", "personal_information", 0, 0, 0)) {
        fprintf(stderr, "连接到数据库失败: %s\n", mysql_error(conn));
        return -1;
    }
  




    struct sockaddr_in seraddr[2]; 
    pthread_t tid;
    socklen_t len = sizeof(MESSAGE);
    //创建套接字
    int sfd = socket(AF_INET,SOCK_STREAM,0);
    if(sfd<0){
        perror("socket");
        return -1;
    }
    //设置io复用
    int flag = 1;      
    setsockopt(sfd,SOL_SOCKET,SO_REUSEADDR,&flag,sizeof(int));
    //绑定端口
    seraddr[0].sin_family = AF_INET;
    seraddr[0].sin_port = htons(9999);
    seraddr[0].sin_addr.s_addr = inet_addr("192.168.43.28"); 
    int ret = bind(sfd,(struct sockaddr *)&seraddr[0],sizeof(seraddr[0]));
    if(ret <0){
        perror("bind");
        return -1;
    }
    //监听
    ret = listen(sfd,30);
    if(ret<0){
        perror("listen");
        return -1;
    }
    
    int epoll_id = epoll_create(1);
    if (epoll_id == -1) {
        perror("epoll");
        return -1;

        // 创建 epoll 实例失败，处理错误
    }

    struct epoll_event event;
    event.events = EPOLLIN;  // 监视可读事件
    event.data.fd = sfd;  // 要监视的文件描述符

    int result = epoll_ctl(epoll_id, EPOLL_CTL_ADD, sfd, &event);
    if (result == -1) {
        // 添加文件描述符到 epoll 实例失败，处理错误
        printf("111\n");
        perror("epoll_ctl");
        return -1;
    }

    struct epoll_event events[MAX_EVENTS];
    
   
    while (1) {
        MESSAGE buf = {0};
        
        // 等待事件发生
        int num_events = epoll_wait(epoll_id, events, MAX_EVENTS, -1);
        if (num_events == -1) {
            // epoll_wait 出错，处理错误
        }

        // 处理已发生的事件
        for (int i = 0; i < num_events; i++) {
            if((events[i].data.fd == sfd)&&(events[i].events == EPOLLIN)){
                socklen_t len = sizeof(seraddr[1]);
                    int cfd = accept(sfd,(struct sockaddr *)&seraddr[1],&len);
                    if(cfd <0){
                        perror("accept");
                        continue;
                    }
                    event.events = EPOLLIN;  // 监视可读事件
                    event.data.fd = cfd;  // 要监视的文件描述符
                    result = epoll_ctl(epoll_id, EPOLL_CTL_ADD, cfd, &event);
                    if (result == -1) {
                        perror("epoll_ctl");
                        return -1;
                    }
                   
            }
            else if((events[i].data.fd != sfd)&&(events[i].events == EPOLLIN)){
                
                if(read(events[i].data.fd,&buf,sizeof(buf))==0){
                    // if(out(&buff,events[i].data.fd)<0){
                    //     printf("失败\n");
                    // }
                    
                   // printf("%d已下线\n",events[i].data.fd);
                  
                                                
                    write(events[i].data.fd,&buf,len);
                                                
                    close(events[i].data.fd);
                    epoll_ctl(epoll_id,EPOLL_CTL_DEL,events[i].data.fd,NULL);
                }
                else{
                    
                    switch(buf.mes_type)
                    {
                        case 1:    if(login(&buf)<0){
                                        strcpy(buf.mes,"注册失败,你要注册的账户已存在\n");
                                        write(events[i].data.fd,&buf,len);
                                        continue;
                                    }
                                    else{
                                        strcpy(buf.mes,"注册成功\n");
                                        write(events[i].data.fd,&buf,len);
                                    }
                                    
                                    break;  //注册
                        
                        case 2: 
                                if(enter(&buf,events[i].data.fd)<0){
                                        strcpy(buf.mes,"登陆失败,你要登陆的账户不存在或输入有误\n");
                                        write(events[i].data.fd,&buf,len);
                                        continue;
                                }
                                else{
                                        if(buf.mes_type2 == 0){
                                            strcpy(buf.mes,"登陆成功\n");
                                            PRINTF_DATA();
                                            write(events[i].data.fd,&buf,len);
                                        }
                                        else if(buf.mes_type2 == 1){
                                                if(strcmp(buf.mes,"quit")==0){
                                                   buf.mes_type2 = 0;
                                                    strcpy(buf.mes,"-------");
                                                    write(events[i].data.fd,&buf,len);
                                                    break;
                                                }
                                                
                                                SEND(&buf,events[i].data.fd);
                                        }
                                        else if(buf.mes_type2 == 2){
                                            if(strcmp(buf.mes,"quit")==0){
                                                buf.mes_type2 = 0;
                                                strcpy(buf.mes,"-------");
                                                
                                                write(events[i].data.fd,&buf,len);
                                                break;
                                            }
                                            send_everyone(&buf,events[i].data.fd);
                                        }
                                        else if(buf.mes_type2 == 3){
                                            
                                            if(out(&buf,events[i].data.fd)<0){
                                                printf("失败\n");
                                            }
                                            PRINTF_DATA();   
                                        }
                                        else if(buf.mes_type2 == 4){
                                                    if(strcmp(buf.mes,"quit")==0){
                                                        buf.mes_type2 = 0;
                                                        strcpy(buf.mes,"已退出\n");
                                                        
                                                        write(events[i].data.fd,&buf,len);
                                                        break;
                                                    }
                                                    //printf("333\n");
                                                    personal(&buf,events[i].data.fd);     
                                        }
                                        else if(buf.mes_type2 == 5){
                                                    if(strcmp(buf.mes,"quit")==0){
                                                        buf.mes_type2 = 0;
                                                        strcpy(buf.mes,"已退出\n");
                                                        
                                                        write(events[i].data.fd,&buf,len);
                                                        break;
                                                    }
                                                    group(&buf,events[i].data.fd);
                                        }
                                        else if(buf.mes_type2 == 6){

                                                set_ban(&buf,events[i].data.fd); 

                                        }
                                        else if(buf.mes_type2 == 7){

                                                Unmute(&buf,events[i].data.fd);

                                        }
                                        else if(buf.mes_type2 == 8){


                                                set_administrator(&buf,events[i].data.fd);
                                        }





                                }

                                     break;  //登陆
                        case 3: 
                        {
                                 if(logout(&buf)<0){
                                        strcpy(buf.mes,"注销失败,你要注销的账户不存在或输入有误\n");
                                        write(events[i].data.fd,&buf,len);
                                        continue;
                                    }
                                    else{
                                        strcpy(buf.mes,"注销成功\n");
                                        write(events[i].data.fd,&buf,len);
                                    }
                        
                                     break;  //注销

                        }
                       
                      
                               


                    }
                    
                    
                }

            }
        }
    }

    // 关闭 epoll 实例和相关的文件描述符
    close(epoll_id);
    close(sfd);



    return 0;
}


// void * data(void *argv)
// {
    


// }