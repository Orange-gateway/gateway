#ifndef _DGN_H_
#define _DGN_H_
#include "include.h"

int server_wifi(void);
int open_wifi_file();
void create_wifi_file();
void gpio_led_kill(int num);
void pthread_gpio_led(void);

 pthread_t id_gpio;
 int gw_add_user_flag;//uid成功标志
 char u_id[50];

#endif
