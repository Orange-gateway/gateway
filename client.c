#include "include.h"
#include <netinet/tcp.h>
/*  tcp初始化函数   */
int client_init(void)
{
	int ret = 1;
	cd = -1;
	struct sockaddr_in ser_addr;
	
	memset((void*)&ser_addr,0,sizeof(ser_addr));
	ser_addr.sin_family = AF_INET;
	ser_addr.sin_port = htons(PORT);
	
	//inet_aton("47.106.229.214",&ser_addr.sin_addr);//将点分式ip地址 转换为2进制 
	//inet_aton("192.168.77.131",&ser_addr.sin_addr);//将点分式ip地址 转换为2进制 
	
	struct hostent *tmp;
	tmp = gethostbyname("smart.zgjuzi.com");
	
	if(tmp == NULL)
	{
		printf("NULL NULL NULL\n");
		return -1;
	}
	
	memcpy(&ser_addr.sin_addr,(struct in_addr *)tmp->h_addr,4);

	cd = socket(AF_INET,SOCK_STREAM,0);
	if(cd == -1)
		return -1;
	ret = connect(cd,(struct sockaddr*)&ser_addr,sizeof(ser_addr));
	if( ret == 0)
	{	
		printf("connect success  cd = %d\n",cd);
		struct timeval timeout;
		socklen_t time_len = sizeof(timeout);
		getsockopt(cd,SOL_SOCKET, SO_SNDTIMEO,&timeout,&time_len);
		printf("%d %d\n",(int)timeout.tv_sec,(int)timeout.tv_usec);
		#if 0
			int keepAlive = 1;
			int keepIdle = 10;
			int keepInterval = 1;
			int keepCount = 5;
			setsockopt(cd, SOL_SOCKET,SO_KEEPALIVE, &keepAlive, sizeof(keepAlive));
			setsockopt(cd, IPPROTO_TCP, TCP_KEEPIDLE, &keepIdle, sizeof(keepIdle));
			setsockopt(cd, IPPROTO_TCP, TCP_KEEPINTVL, &keepInterval, sizeof(keepInterval));
			setsockopt(cd, IPPROTO_TCP, TCP_KEEPCNT, &keepCount, sizeof(keepCount));
		#endif
		
		return 0;
	}
	else
	{
		printf("connet fault  cd = %d\n",cd);
		NET_FLAG = 0;
		return -1;
	}
}



