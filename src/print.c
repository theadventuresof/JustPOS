#include "../lib/justpos.h"

/*
 * Build a string for lp command to print to specified printer
 * or use defaults if config is empty.
 */
void print_reciept(char path[])
{
	char temp[100],command[100];
	strncpy(command,"lp ",4);
	if(check_conf_val("printer1=") == 1)
	{
		get_file_data(".conf","printer1=",temp);
		strncat(command,"-d ",4);
		strncat(command,temp,strlen(temp) + 1);
		strncat(command," ",2);
	}
	if(check_conf_val("printer1-copies=") == 1)
	{
		get_file_data(".conf","printer1-copies=",temp);
		strncat(command,"-n ",4);
		strncat(command,temp,strlen(temp) + 1);
		strncat(command," ",2);
	}
	strncat(command,path,strlen(path) + 1);
	//printw("\n\n%s\n",command);
	//system(command);

}
