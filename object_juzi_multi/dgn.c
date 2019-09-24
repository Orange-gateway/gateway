#include "include.h"
int open_wifi_file()
{
	int wifi_file_fd;
	wifi_file_fd = open("/root/wifi_file.txt",O_RDWR,0777);
	if( wifi_file_fd==-1 )
	{
		printf("open wifi_file failed\n");

		close(wifi_file_fd);
		return -1;
	}
	else
	{
		printf("open wifi_file success\n");
		close(wifi_file_fd);
		return 0;
	}
}
void create_wifi_file()
{
	open("/root/wifi_file.txt",O_RDWR | O_CREAT,0777);
	printf("create wifi_file.txt success\n");
}

/*led*/
void gpio_led_kill(int num)
{
	system("echo out > /sys/class/gpio/gpio12/direction");
	pthread_exit(0);
}
void pthread_gpio_led(void)
{
	pthread_detach(pthread_self());
	signal(4,gpio_led_kill);
	while(1)
	{
		system("echo in > /sys/class/gpio/gpio12/direction");
		usleep(500000);
		system("echo out > /sys/class/gpio/gpio12/direction");
		usleep(500000);
	}
}

int server_wifi(void)
{
	int cl_num=0;
	int slen=0;
	char passwd[100];
	memset(passwd,0,100);
	char buf[1024];
	memset(buf,0,1024);
	int sockfd, num_d=0,sin_size,ret_recv,client_fd,num=0;
	fd_set inset,tmp_inset;
	struct sockaddr_in server_sockaddr, client_sockaddr;
	memset((void *)&server_sockaddr,0,sizeof(server_sockaddr));
	memset((void *)&client_sockaddr,0,sizeof(client_sockaddr));
	gw_add_user_flag=0;
	if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1) 
	{ 
		perror("socket"); 
		exit(1); 
	} 
	server_sockaddr.sin_family = AF_INET; 
	server_sockaddr.sin_port = htons(8888); 
	server_sockaddr.sin_addr.s_addr = inet_addr("192.168.77.1"); 
	bzero(&(server_sockaddr.sin_zero), 8);
	int i = 1;/* 允许重复使用本地地址与套接字进行绑定*/ 
	setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &i, sizeof(i));
	if (bind(sockfd, (struct sockaddr *)&server_sockaddr, sizeof(struct sockaddr)) == -1) 
	{ 
		perror("bind"); 
		exit(1); 
	}
	if(listen(sockfd,10)== -1) 
	{ 
		perror("listen"); 
		exit(1);
	}

	printf("listening.........%d\n",sockfd);

	FD_ZERO(&inset); 
	FD_SET(sockfd, &inset);
	while(1)
	{
		tmp_inset = inset;
		sin_size=sizeof(struct sockaddr_in); 
		if (select(1024, &tmp_inset,NULL, NULL, NULL)>0) //选择文件描述符的动作，如果有动作就不阻塞 否则一直阻塞在那.
		{
			for (num_d = 3; num_d < 13; num_d++)
			{
				if (FD_ISSET(num_d, &tmp_inset) > 0)
				{
					if (num_d == sockfd) 
					{
						if ((client_fd = accept(sockfd,(struct sockaddr *)&client_sockaddr,(socklen_t *)&sin_size))== -1) 
						 { 
	 	 				       perror("accept"); 
						       exit(1); 
						 } 
						 FD_SET(client_fd, &inset);
						 num+=1;
					}
					else if(num_d == client_fd)
					{
						ret_recv=recv(num_d,buf,1024,0);//接收
						write(num_d,"recv\n",strlen("recv\n"));
						if(ret_recv>0)
						{	
							shutdown(sockfd,SHUT_RDWR);
							close(sockfd);

							slen = strlen(buf);
							buf[slen]='\0';
							cJSON *wifi_root = cJSON_Parse(buf);
							cJSON *wifi_uid = cJSON_GetObjectItem(wifi_root,"uid");
							cJSON *wifi_ssid = cJSON_GetObjectItem(wifi_root,"ssid");
							cJSON *wifi_passwd = cJSON_GetObjectItem(wifi_root,"passwd");

							memset(u_id,0,50);
							memcpy(u_id,wifi_uid->valuestring,strlen(wifi_uid->valuestring));
							printf("uid:%s\n",wifi_uid->valuestring);
							printf("ssid:%s\n",wifi_ssid->valuestring);
							printf("passwd:%s\n",wifi_passwd->valuestring);
							strcat(passwd,"/bin/set_sta.sh ");
							strcat(passwd,wifi_ssid->valuestring);
							strcat(passwd," ");
							strcat(passwd,wifi_passwd->valuestring);
							system(passwd);
							while(1)
							{
								if(system("ping -c 1 www.baidu.com") == 0)
								{
									uint8_t voice_notice[23] = {0x5a,0xa5,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x45,0x01,0x00,0x00,0x08,0xa5,0xfa,0x00,0x03,0xbe,0x00,0x5f,0xfb};//网络连接成功
									usart_send(fd,voice_notice,23);
									cJSON_Delete(wifi_root);
									wifi_root=NULL;
									pthread_kill(id_gpio,4);
									close(sockfd);
									close(num_d);
									return 1;
								}
								else
								{
									if(cl_num > 5)
									{
										uint8_t voice_notice[23] = {0x5a,0xa5,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x88,0x45,0x01,0x00,0x00,0x08,0xa5,0xfa,0x00,0x03,0xbd,0x00,0x60,0xfb};//网络连接不成功
										usart_send(fd,voice_notice,23);
										system("/bin/net_reset.sh");
										close(sockfd);
										close(num_d);
										cJSON_Delete(wifi_root);
										wifi_root=NULL;
										return 0;
									}
									sleep(5);
									cl_num+=1;
								}
							}
						}
						else
						{
							close(num_d);//diaoxian
							FD_CLR(num_d, &inset);
						}
					}
				}
			}
		}
	}
}




