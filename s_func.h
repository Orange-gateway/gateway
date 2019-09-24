#ifndef _S_FUNC_H_
#define _S_FUNC_H_
#include "include.h"
void delete_len_from_arr(uint8_t *arr,int n,int *len_of_arr);
void delete_len_from_str(char *str,int n,int *len_of_str);
int replace_character(char *string);
pthread_t pth_creat_my(void *func,void *s_data);
void int_to_str(int num,char *str);
char int_to_char(int i);
int char_to_int(char b);
void hex_to_str(uint8_t  *str_unchar,char *str_char,int len_of_hex);
void str_to_hex(uint8_t *str_unchar,char *str_char,int len_of_hex);
int reboot_restart();
void kill_gateway(void);
void get_mac(char *MyMac);
void create_secret_str(char *str,char *mysn,char *mac);//身份验证，加密函数
void sn_pwd_str(char *str,char *mac);//sn身份验证，加密函数
void get_pwd(char *pwd,char *sn,char *timestamp,char *mac);//加密函数
void get_sn_pwd(char *pwd,char *timestamp,char *mac);//加密函数
void scene_list_rq(void);
void scene_detail_rq(void);
void dev_list_rq(void);
void multi_bind_rq(void);
void voice_list_rq(void);
void timer_list_rq(void);
void secure_set_list_rq(void);
void room_list_rq(void);
void get_sensor_value(uint8_t *u_data,char *value,char *level);
void get_sensor_power(uint8_t *u_data,char *level);
void pthread_mutex_init_func(void);
void rgb_judge(uint8_t *num,char *rgb_char);
int mac_mac(char *mac1,char *mac2);
void get_sn_from_server(cJSON *root);
void get_sn_feedback(cJSON *root);
int trigger_source_time_judge(char *time_judge);
void read_channel(void);
int traversing_room_list(char *voice_str,char *ret_str);
#endif
