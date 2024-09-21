#include <string.h>
#include <stdlib.h>

#include "../lib/print.h"
#include "../lib/file.h"
#include "../lib/report.h"
#include "../lib/state.h"

/*
 * Build a string for lp command to print to specified printer
 * or use defaults if config is empty.
 */
void print_reciept(char path[])
{
	if(get_state("PRINTER_1") == 0)
	{
		return;
	}
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
	strncat(command," &> /dev/null",14);
	system(command);

}
