#ifndef _MAC_TEST_H_
#define _MAC_TEST_H_
#include "main.h"
#include <stdint.h>
int mac_and_port_judge(RSD *p,uint8_t *cmd);
int mac_and_port_judge_human(HB *p,char *mac,char *port);
int mac_and_port_judge_delay(DELAY *p,char *mac,char *port);
#endif
