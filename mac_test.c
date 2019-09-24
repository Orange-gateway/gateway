#include "main.h"
#include <string.h>
int mac_and_port_judge(RSD *p,uint8_t *cmd)
{
	int i=0,flag_success=0;
	for(i=0;i<2;i++)
	{
		if(p->cmd[i+8]==cmd[i+8])
		{
			flag_success+=1;
		}
	}
	if(p->cmd[11]==cmd[11])
	{
		flag_success+=1;
	}
	if(flag_success==3)
	{
		return 0;
	}
	else
	{
		return -1;
	}
}

int mac_and_port_judge_human(HB *p,char *mac,char *port)
{
	if(strcmp(p->mac,mac)==0 && strcmp(p->port,port)==0)
		return 0;
	else
		return -1;
}


