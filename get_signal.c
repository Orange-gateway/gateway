#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <linux/wireless.h>
#include "cjson.h"
#include <malloc.h>
#include "main.h"
#include <sys/ioctl.h>
#include <unistd.h>
void get_wireless_link_siganl(char *ath)
{
    int sock_fd;
    struct iwreq iwr;
    struct iw_statistics stats;

    /* make socket fd */
    if ((sock_fd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        printf("socket err/n");
        return ;
    }

    /* make iwreq */
    memset(&iwr, 0, sizeof(iwr));
    memset(&stats, 0, sizeof(stats));
    iwr.u.data.pointer = (caddr_t) &stats; /* result value */
    iwr.u.data.length = sizeof(stats);
    iwr.u.data.flags = 1; /* clear updated flag */

    /* ifname is reqired */
    strncpy(iwr.ifr_ifrn.ifrn_name, ath, IFNAMSIZ - 1);

    /* get SIOCGIWSTATS */
    if (ioctl(sock_fd, SIOCGIWSTATS, &iwr) < 0)
    {
        printf("No Such Device %s/n",ath);
        close(sock_fd);
        return ;
    }
    
	cJSON *wifi_signal_root = cJSON_CreateObject();
    cJSON_AddNumberToObject(wifi_signal_root,"retcode",0);
    cJSON_AddStringToObject(wifi_signal_root,"message","success");
    cJSON_AddStringToObject(wifi_signal_root,"api","app_gw_get_signal");
    cJSON *wifi_signal_data = cJSON_CreateObject();
    cJSON_AddNumberToObject(wifi_signal_data,"value",stats.qual.qual);
    cJSON_AddItemToObject(wifi_signal_root,"data",wifi_signal_data);
    char *send_char = cJSON_PrintUnformatted(wifi_signal_root);
    int my_len = strlen(send_char);
    char *my_send_char = (char *)malloc(my_len+2);
    memset(my_send_char,0,my_len+2);
    memcpy(my_send_char,send_char,my_len);
    strcat(my_send_char,"\n\0");
	//printf("my_send_char:%s",my_send_char);
    send(cd,my_send_char,my_len+1,0);

    cJSON_Delete(wifi_signal_root);
    wifi_signal_root = NULL;
    free(send_char);
    send_char = NULL;
    free(my_send_char);
    my_send_char = NULL;
    close(sock_fd);
    return ;
}

