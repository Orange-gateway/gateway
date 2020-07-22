#include "include.h"

int reboot_restart()
{
	sync(); // 同步磁盘数据,将缓存数据回写到硬盘,以防数据丢失
    return reboot(RB_AUTOBOOT);
}
void kill_gateway(void)
{
	close(fd);
	shutdown(cd,SHUT_RDWR);
	close(cd);
	pthread_mutex_lock(&mutex_down_file);
	sync();
	system("killall gateway");
	pthread_mutex_unlock(&mutex_down_file);
}
void delete_len_from_arr(uint8_t *arr,int n,int *len_of_arr)///操作的数组，删除的长度，本来的长度
{
    int i;
    
    for(i=n; i<(*len_of_arr); i++)
    {
        arr[i-n] =arr[i];
    }
    
    *len_of_arr -=n;
}


void delete_len_from_str(char *str,int n,int *len_of_str)///操作的字符串，删除的长度，本来的长度
{
    int i;
    
    for(i=n; i<(*len_of_str); i++)
    {
        str[i-n] =str[i];
    }
    
    *len_of_str -=n;
}

// 将中文字符替换为'*' 用于json字符串合法性检查
int replace_character(char *string)
{
    if (string == NULL)
    {
        return -1;
    }
    while(*string++ != '\0')
    {
        if (((*string) < 0x00) || ((*string) > 0x7F))
        {
            *string = '*';
        }
    }

    return 0;
}

void int_to_str(int num,char *str)        
{
    int i,j;
    char string[50];
    memset(string,0,sizeof(string));
    if( num < 0 )
        num=-num;
    i=0;
    do
    {
        string[i++] = num%10+'0';
    }
    while ( (num/=10) >0 );

    for(j=0; j<i; j++) 
    {
        str[j] = string[i-j-1];
    }
    str[i] = '\0';
}
/*******************整形转化为字符*****************/
char int_to_char(int i)
{
	if((-1<i)&&(i<10))
		return i+48;
	else if((i>9)&&(i<16))
		return  i+87;
	else
		return '*';
}
/**************字符转化为整形**************/
int char_to_int(char b)
{
	if((47<b)&&(b<58))
		return b-48;
	else if((64<b)&&(b<71))
		return b-55;
	else if((96<b)&&(b<103))
		return b-87;
	else
		return -1;
}

/*16进制转字符串:参数：16进制，字符串，16进制长度*/
void hex_to_str(uint8_t *str_unchar,char *str_char,int len_of_hex)
{
    int i,H1=0,H2=0;
    for(i=0; i<len_of_hex; i++)
    {
        H1 = str_unchar[i]/16;
        H2 = str_unchar[i]%16;
        if((H1>=0)&&(H1<=9))
        {
            str_char[i+i] = (char)(48 + H1);
        }
        else if((H1>=10)&&(H1<=15))
        {
            str_char[i+i] = (char)(87 + H1);
        }
        if((H2>=0)&&(H2<=9))
        {
            str_char[i+i+1] = (char)(48 + H2);
        }
        else if((H2>=10)&&(H2<=15))
        {
            str_char[i+i+1] = (char)(87 + H2);
        }
    }
}

/*字符串转16进制：参数：16进制数组，字符串，16进制数组长度*/
void str_to_hex(uint8_t *str_unchar,char *str_char,int len_of_hex)
{
    int i,i1=0,i2=0;
    for(i=0; i<len_of_hex; i++)
    {
        if(str_char[2*i]>='a'&&str_char[2*i]<='f')
        {
            i1 = (str_char[2*i]-'a'+10)*16;
        }
        else if(str_char[2*i]>='A'&&str_char[2*i]<='F')
        {
            i1 = (str_char[2*i]-'A'+10)*16;
        }
        else if(str_char[2*i]>='0'&&str_char[2*i]<='9')
        {
            i1 = (str_char[2*i] - '0')*16;
        }

        if(str_char[2*i+1]>='a'&&str_char[2*i+1]<='f')
        {
            i2 = str_char[2*i+1]-'a'+10;
        }
        else if(str_char[2*i+1]>='A'&&str_char[2*i+1]<='F')
        {
            i2 = str_char[2*i+1]-'A'+10;
        }
        else if(str_char[2*i+1]>='0'&&str_char[2*i+1]<='9')
        {
            i2 = str_char[2*i+1] - '0';
        }
        
        str_unchar[i] = i1+i2;
    
    }
  
}
void get_sensor_value(uint8_t *u_data,char *value,char *level)
{
	//char *value_z = (char *)malloc(6);
	char value_z[6];
	memset(value_z,0,6);
	//char *value_x = (char *)malloc(4);
	char value_x[4];
	memset(value_x,0,4);
	uint16_t num1 ;
	num1 = u_data[16]<<8 | u_data[17];
	int num_z = (int)num1;
	int num_x = (int)u_data[18];

	if(u_data[15]!=0)
		value[0]='-';
	int_to_str(num_z,value_z);
	strcat(value,value_z);
	int_to_str(num_x,value_x);
	if(num_x !=0)
	{
		strcat(value,".");
		strcat(value,value_x);
	}
	int my_level;
	my_level = (int)u_data[19];
	int_to_str(my_level,level);
	//free(value_z);
	//value_z = NULL;
	//free(value_x);
	//value_x = NULL;
}
void get_sensor_power(uint8_t *u_data,char *level)
{
	int num_level = (int)u_data[16];
	int_to_str(num_level,level);
}
void rgb_judge(uint8_t *num,char *rgb_char)
{
	int i;
	for(i=0;i<4;i++)
	{
		if(num[i]>0xfa)
		num[i]=0xfa;
	}
	hex_to_str(num,rgb_char,4);
	rgb_char[8]='\0';
}

int mac_mac(char *mac1,char *mac2)
{
	if(mac1[12]==mac2[12]&&mac1[13]==mac2[13]&&mac1[14]==mac2[14]&&mac1[15]==mac2[15])
	return 1;
	else
	return 0;
}
/************线程创建**************/
pthread_t pth_creat_my(void *func,void *s_data)
{

 pthread_t pid;
 pthread_attr_t attr;
///设置线程的分离属性
 int res = pthread_attr_init(&attr);        //初始化
 if(res!=0)
    printf("attr init fault\n");
	
	pthread_attr_setscope(&attr, PTHREAD_SCOPE_SYSTEM);//all
	
 res = pthread_attr_setdetachstate(&attr,PTHREAD_CREATE_DETACHED);
 
 if(res!=0)
    printf("set_detachstate fault\n");

 if(pthread_create(&pid,&attr,func,s_data)!=0)
    {
        printf("create pthread  fault\n");
    }
    return pid;
}
void get_mac(char *MyMac)
{
    unsigned char MyMac0[6];
    memset(MyMac0,0,6);
    struct ifreq req;
    int sock;
    sock = socket (AF_INET, SOCK_STREAM, 0);
    if(sock < 0)
    {
        perror("socket error:");
    }
    strcpy(req.ifr_name, "ath0");    ///Currently, only get ath0
    if(ioctl(sock, SIOCGIFHWADDR, &req) < 0)
    {
        perror("ioctl error:");
    }
    int i=0;
    for(i=0; i<6; i++)
    {
        MyMac0[i] = (unsigned char)req.ifr_hwaddr.sa_data[i];
 
    }
    close(sock);
    hex_to_str(MyMac0,MyMac,6);
    MyMac[12] = '\0';
	//printf("mac:%s\n",MyMac);
}

/***************************************************************************/

void create_secret_str(char *str,char *sn,char *mac)
{
    time_t time_now;
    time(&time_now);
    int time_now_int = (int)time_now;
    char *times_tamp = (char *)malloc(11);
    memset(times_tamp,0,11);
    int_to_str(time_now_int,times_tamp);

    char *encrypted_data = (char *)malloc(33);
    
    memset(encrypted_data,0,33);
    
    get_pwd(encrypted_data,sn,times_tamp,mac);


    cJSON *da=cJSON_CreateObject();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_identify");
    cJSON_AddItemToObject(root,"data",da);

    /**动态获取mac，sn**/
    cJSON_AddStringToObject(da,"mac",mac);
    cJSON_AddStringToObject(da,"sn",sn);
    cJSON_AddStringToObject(da,"pwd",encrypted_data);
    cJSON_AddStringToObject(da,"timestamp",times_tamp);
	cJSON_AddStringToObject(da,"version",gw_version);
	struct timeval now;
	struct timespec outtime;
	gettimeofday(&now, NULL);
	outtime.tv_sec = now.tv_sec + 1;
	pthread_cond_timedwait(&cond,&mutex_waite,&outtime);
	cJSON_AddStringToObject(da,"pan_id",zigbee_id);
	cJSON_AddStringToObject(da,"channel",zigbee_channel);
    char *char_root=cJSON_PrintUnformatted(root);
    
    memcpy(str,char_root,strlen(char_root));
    
    strcat(str,"\n\0");

    free(times_tamp);
    times_tamp = NULL;

    free(encrypted_data);
    encrypted_data = NULL;

    free(char_root);
    char_root = NULL;

    cJSON_Delete(root);
    root=NULL;
}
/***************************************************************************/
void sn_pwd_str(char *str,char *mac)
{
    time_t time_now;
    time(&time_now);
    int time_now_int = (int)time_now;
    char *times_tamp = (char *)malloc(11);
    memset(times_tamp,0,11);
    int_to_str(time_now_int,times_tamp);

    char *encrypted_data = (char *)malloc(33);
    
    memset(encrypted_data,0,33);
    
    get_sn_pwd(encrypted_data,times_tamp,mac);


    cJSON *da=cJSON_CreateObject();
    cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_get_sn");
    cJSON_AddItemToObject(root,"data",da);

    /**动态获取mac，sn**/
    cJSON_AddStringToObject(da,"mac",mac);
    cJSON_AddStringToObject(da,"pwd",encrypted_data);
    cJSON_AddStringToObject(da,"timestamp",times_tamp);
    
    char *char_root=cJSON_PrintUnformatted(root);
    
    memcpy(str,char_root,strlen(char_root));
    
    strcat(str,"\n\0");

    free(times_tamp);
    times_tamp = NULL;

    free(encrypted_data);
    encrypted_data = NULL;

    free(char_root);
    char_root = NULL;

    cJSON_Delete(root);
    root=NULL;
}
/***************************************************************************/
void get_pwd(char *pwd,char *sn,char *timestamp,char *mac)//加密函数
{
    MD5_CTX md5;
    MD5Init(&md5); ///initialize md5
    char code[21] = "zgjzzgjzzgjzzgjzzgjz\0";
    char *long_code = (char *)malloc(128);
    memset(long_code,0,128);
    ///拼接加入
    strcpy(long_code,mac);
    strcat(long_code,sn);
    strcat(long_code,timestamp);
    strcat(long_code,code);

    ///需要加密的字符串
    unsigned char *encrypt = (unsigned char *)malloc(256);
    memset(encrypt,0,256);
    strcpy((char *)encrypt,long_code);
    ///decrypt产生的目标字符串
    unsigned char *decrypt = (unsigned char *)malloc(16);
    memset(decrypt,0,16);
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(decrypt,&md5);
    ///转化为整数，
    hex_to_str(decrypt,pwd,16);
 
    
    free(long_code);
    long_code = NULL;
    
    free(encrypt);
    encrypt = NULL;
    
    free(decrypt);
    decrypt = NULL;
}
/***************************************************************************/
void get_sn_pwd(char *pwd,char *timestamp,char *mac)//加密函数
{
    MD5_CTX md5;
    MD5Init(&md5); ///initialize md5
    char code[21] = "zgjzzgjzzgjzzgjzzgjz\0";
    char *long_code = (char *)malloc(128);
    memset(long_code,0,128);
    ///拼接加入
    strcpy(long_code,mac);
   
    strcat(long_code,timestamp);
    strcat(long_code,code);

    ///需要加密的字符串
    unsigned char *encrypt = (unsigned char *)malloc(256);
    memset(encrypt,0,256);
    strcpy((char *)encrypt,long_code);
    ///decrypt产生的目标字符串
    unsigned char *decrypt = (unsigned char *)malloc(16);
    memset(decrypt,0,16);
    MD5Update(&md5,encrypt,strlen((char *)encrypt));
    MD5Final(decrypt,&md5);
    ///转化为整数，
    hex_to_str(decrypt,pwd,16);

    
    free(long_code);
    long_code = NULL;
    
    free(encrypt);
    encrypt = NULL;
    
    free(decrypt);
    decrypt = NULL;
}
/*******************************************************************/
//请求情景列表
void scene_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_getScenBas");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
	send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/*******************************************************************/
//请求情景明细列表
void scene_detail_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_getScenList_mx");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/*********************************************************************/
//请求设备列表
void dev_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_get_devlist");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}

/***********************************************************************/
//请求多联绑定列表
void multi_bind_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_getBindList_mx");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/***************************************************************************/
//请求定时器列表
void timer_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_getTimerList");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/***************************************************************************/
//请求安防列表
void secure_set_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_getSecuModelSet");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
 	int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/*****************************************************************************/
//请求离线语言表
void voice_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_get_voi_conf");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/*****************************************************************************/
//请求房间表
void room_list_rq(void)
{
	cJSON *data=cJSON_CreateObject();
	cJSON *root = cJSON_CreateObject();
    cJSON_AddStringToObject(root,"co","orange");
    cJSON_AddStringToObject(root,"api","gw_get_room");
    cJSON_AddItemToObject(root,"data",data);
    char *str=cJSON_PrintUnformatted(root);
    int my_len = strlen(str);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,str,my_len);
    strcat(my_send_char,"\n\0");
    send(cd,my_send_char,my_len+1,0);
    cJSON_Delete(root);
    root = NULL;
    free(str);
    str=NULL;
    free(my_send_char);
    my_send_char=NULL;
}
/*******************************************************************************************/
void pthread_mutex_init_func(void)
{
	pthread_mutex_init(&mutex_v,NULL);
	pthread_mutex_init(&mutex_sl,NULL);
	pthread_mutex_init(&mutex_bl,NULL);
	pthread_mutex_init(&mutex_qj,NULL);
	pthread_mutex_init(&mutex_zl,NULL);
	pthread_mutex_init(&mutex_af,NULL);
	pthread_mutex_init(&mutex_scene,NULL);
	pthread_mutex_init(&mutex_time,NULL);
	pthread_mutex_init(&mutex_voice,NULL);
	pthread_mutex_init(&mutex_code,NULL);
	pthread_mutex_init(&mutex_down_file,NULL);
	pthread_mutex_init(&mutex_waite,NULL);
	pthread_mutex_init(&mutex_room,NULL);
	pthread_mutex_init(&mutex_human,NULL);
	pthread_mutex_init(&mutex_delay,NULL);
	pthread_mutex_init(&mutex_resend,NULL);
}

/*********************************************************************************************/
void get_sn_from_server(cJSON *root)
{
	cJSON *ret = cJSON_GetObjectItem(root,"retcode");
	if(ret->valueint==0)
	{
		char *mac = malloc(13);
		memset(mac,0,13);
		get_mac(mac);
		cJSON *get_data = cJSON_GetObjectItem(root,"data");
		cJSON *get_sn_num = cJSON_GetObjectItem(get_data,"sn");
		cJSON *sn_old_or_new = cJSON_GetObjectItem(get_data,"is_old");
		if(sn_old_or_new->valueint==0)//new sn
		{
			cJSON *notice_sn = cJSON_CreateObject();
			cJSON_AddStringToObject(notice_sn,"co","orange");
			cJSON_AddStringToObject(notice_sn,"api","gw_notice_sn");
			cJSON *notice_data = cJSON_CreateObject();
			cJSON_AddItemToObject(notice_sn,"data",notice_data);
			cJSON_AddStringToObject(notice_data,"mac",mac);
			cJSON_AddStringToObject(notice_data,"program_ver",gw_version);
			cJSON_AddStringToObject(notice_data,"sn",get_sn_num->valuestring);
			cJSON_AddStringToObject(notice_data,"type","2");
			char *send_char = cJSON_PrintUnformatted(notice_sn);
			int my_len = strlen(send_char);
			char *my_send_char = (char *)malloc(my_len+2);
			memset(my_send_char,0,my_len+2);
			memcpy(my_send_char,send_char,my_len);
			strcat(my_send_char,"\n\0");
			send(cd,my_send_char,my_len+1,0);
			cJSON_Delete(notice_sn);
			notice_sn=NULL;
			free(send_char);
			send_char= NULL;
			free(my_send_char);
			my_send_char=NULL;
		}
		else if(sn_old_or_new->valueint==1)//old sn
		{
			int sn_fd = open("/root/sn.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
			cJSON *sn_object = cJSON_CreateObject();
			cJSON_AddStringToObject(sn_object,"sn",get_sn_num->valuestring);
			char *sn_char = cJSON_PrintUnformatted(sn_object);
			write(sn_fd,sn_char,(int)strlen(sn_char));
			fsync(sn_fd);
			close(sn_fd);
			cJSON_Delete(sn_object);
			sn_object=NULL;
			free(sn_char);
			sn_char=NULL;
			
			char *str_json = malloc(256);
			memset(str_json,0,256);
			create_secret_str(str_json,get_sn_num->valuestring,mac);
			send(cd,str_json,(int)strlen(str_json),0);
			free(str_json);
			str_json=NULL;
		}
		free(mac);
		mac = NULL;
	}
}
/*********************************************************************************************/
void get_sn_feedback(cJSON *root)
{
	cJSON *ret = cJSON_GetObjectItem(root,"retcode");
	if(ret->valueint==0)
	{
		char *str_json = malloc(256);
		memset(str_json,0,256);
		char *mac = malloc(13);
		memset(mac,0,13);
		get_mac(mac);
		cJSON *get_data = cJSON_GetObjectItem(root,"data");
		cJSON *get_sn_num = cJSON_GetObjectItem(get_data,"sn");
		int sn_fd = open("/root/sn.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
		cJSON *sn_object = cJSON_CreateObject();
		cJSON_AddStringToObject(sn_object,"sn",get_sn_num->valuestring);
		char *sn_char = cJSON_PrintUnformatted(sn_object);
		write(sn_fd,sn_char,(int)strlen(sn_char));
		fsync(sn_fd);
		close(sn_fd);
		cJSON_Delete(sn_object);
		sn_object=NULL;
		free(sn_char);
		sn_char=NULL;
		
		create_secret_str(str_json,get_sn_num->valuestring,mac);
		send(cd,str_json,(int)strlen(str_json),0);
		free(str_json);
		str_json=NULL;
		free(mac);
		mac=NULL;
	}
}


/*******************************************************************************************/
int trigger_source_time_judge(char *time_judge)
{
	int flag_kt=0;
	char *str1 = NULL;
	char *str2 = NULL;
	str1 = strtok(time_judge, "-");
	str2 = strtok(NULL, "-");
	if( strcmp(str1,str2) > 0)//跨天执行
	{
		time_t nowtime;
		struct tm* nt;
		time(&nowtime);
		nt = localtime(&nowtime);
		char nowtime_str[10];
		memset(nowtime_str,0,10);
		sprintf(nowtime_str,"%02d:%02d:%02d",nt->tm_hour,nt->tm_min,nt->tm_sec);
		if(strcmp(nowtime_str,str2) > 0 && strcmp(nowtime_str,str1) < 0 )
			flag_kt = 0;
		else
			flag_kt = 1;
	}
	else if( strcmp(str1,str2) <= 0)//无需跨天执行
	{
		time_t nowtime;
		struct tm* nt;
		time(&nowtime);
		nt = localtime(&nowtime);
		char nowtime_str[10];
		memset(nowtime_str,0,10);
		sprintf(nowtime_str,"%02d:%02d:%02d",nt->tm_hour,nt->tm_min,nt->tm_sec);
		if( strcmp(nowtime_str,str1) >= 0 && strcmp(nowtime_str,str2) <= 0)
			flag_kt = 1;
	}
	if( flag_kt )//执行
		return 0;
	else
		return 1;
}
void read_channel(void)
{
	uint8_t read_channel_cmd[16] = {0x5a,0xa5,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x30,0x00,0x00,0x00,0x01,0x00};
	usart_send(fd,read_channel_cmd,16);
}

int traversing_room_list(char *voice_str,char *ret_str)
{
	int ret = 0;
	pthread_mutex_lock(&mutex_room);
	cJSON *room_list_pase = cJSON_Parse(room_list);
	pthread_mutex_unlock(&mutex_room);
	if(room_list_pase != NULL)
	{
		if(room_list_pase->child != NULL)
		{
			cJSON *room_list_pase_data = cJSON_GetObjectItem(room_list_pase,"data");
			cJSON *room_data_list = cJSON_GetObjectItem(room_list_pase_data,"room_list");
			int room_num = cJSON_GetArraySize(room_data_list);
			int i=0;
			cJSON *room_arr = NULL;
			cJSON *room_name_list = NULL;
			for(i=0;i<room_num;i++)
			{
				room_arr = cJSON_GetArrayItem(room_data_list,i);
				room_name_list = cJSON_GetObjectItem(room_arr,"room_name");
				if(strstr(voice_str,room_name_list->valuestring))
				{
					cJSON *room_id_list = cJSON_GetObjectItem(room_arr,"room_id");
					int room_name_len = strlen(room_id_list->valuestring)+1;
					memcpy(ret_str,room_id_list->valuestring,room_name_len);
					ret = 1;
					break;
				}
			}
		}
	}
	cJSON_Delete(room_list_pase);
	room_list_pase = NULL;
	return ret;
}
void my_delay_file(void)
{
	DELAY *p_delay = NULL;
	p_delay = delay_head;
	cJSON *delay_root = cJSON_CreateObject();
	cJSON *delay_arr = cJSON_CreateArray();
	cJSON_AddItemToObject(delay_root,"delay_list",delay_arr);
	while( p_delay )
	{
		if( p_delay->flag )
		{
			cJSON *delay_add = cJSON_CreateObject();
			cJSON_AddStringToObject(delay_add,"dev_id",p_delay->dev_id);
			cJSON_AddStringToObject(delay_add,"dev_mac",p_delay->dev_mac);
			cJSON_AddStringToObject(delay_add,"dev_type",p_delay->dev_type);
			cJSON_AddStringToObject(delay_add,"dev_port",p_delay->dev_port);
			cJSON_AddStringToObject(delay_add,"dev_cmd",p_delay->cmd);
			cJSON_AddNumberToObject(delay_add,"dev_hour",p_delay->delay_time[0]);
			cJSON_AddNumberToObject(delay_add,"dev_min",p_delay->delay_time[1]);
			cJSON_AddNumberToObject(delay_add,"dev_sec",p_delay->delay_time[2]);
			cJSON_AddItemToArray(delay_arr,delay_add);
		}
		p_delay = p_delay->next;
	}
	char *my_char = cJSON_PrintUnformatted(delay_root);
	int delay_file_fd = open("/root/delay_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
	write(delay_file_fd,my_char,strlen(my_char));
	fsync(delay_file_fd);
	close(delay_file_fd);
	free(my_char);
	my_char = NULL;
	cJSON_Delete(delay_root);
	delay_root = NULL;
}
void my_human_file(void)
{
	HB *p_human = NULL;
	p_human = human_head;
	cJSON *human_root = cJSON_CreateObject();
	cJSON *human_arr = cJSON_CreateArray();
	cJSON_AddItemToObject(human_root,"human_list",human_arr);
	while( p_human )
	{
		if( p_human->flag )
		{
			cJSON *human_add = cJSON_CreateObject();
			cJSON_AddStringToObject(human_add,"dev_mac",p_human->mac);
			cJSON_AddStringToObject(human_add,"dev_port",p_human->port);
			cJSON_AddStringToObject(human_add,"dev_id",p_human->id);
			cJSON_AddStringToObject(human_add,"dev_type",p_human->type);
			cJSON_AddNumberToObject(human_add,"dev_time",p_human->now_time);
			cJSON_AddItemToArray(human_arr,human_add);
		}
		p_human = p_human->next;
	}
	char *my_char = cJSON_PrintUnformatted(human_root);
	int human_file_fd = open("/root/human_list.txt",O_RDWR|O_CREAT|O_TRUNC,0777);
	write(human_file_fd,my_char,strlen(my_char));
	fsync(human_file_fd);
	close(human_file_fd);
	free(my_char);
	my_char = NULL;
	cJSON_Delete(human_root);
	human_root = NULL;
}
void delete_delay_or_human_file(char *delete_mac)
{
	HB *p1 = NULL;
	p1 = human_head;
	pthread_mutex_lock(&mutex_human);
	while( p1 )
	{
		if(!strcmp(p1->mac,delete_mac))
		{
			p1->flag = 0;
			my_human_file();
			break;
		}
		p1 = p1->next;
	}
	pthread_mutex_unlock(&mutex_human);
	
	DELAY *p = NULL;
	p = delay_head;
	pthread_mutex_lock(&mutex_delay);
	while( p )
	{
		if(!strcmp(p->dev_mac,delete_mac))
		{
			p->flag = 0;
			my_delay_file();
			break;
		}
		p = p->next;
	}
	pthread_mutex_unlock(&mutex_delay);
}
