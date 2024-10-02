#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <panel.h>

#include "../lib/print.h"
#include "../lib/file.h"
#include "../lib/report.h"
#include "../lib/state.h"
#include "../lib/err.h"
#include "../lib/draw.h"
#include "../lib/scroll.h"
#include "../lib/draw_state_6.h"

struct devices{
	char device[100];
	struct devices *next;
}devices;

struct devices *head1  = NULL;

/*
 * Get installed printers (According to CUPS) from installed_printers.dat
 * and populate a linked list with their device names
 */
void find_printers(void)
{
	/*
	 * Create file pointer called printers and initialize it to point to
	 * dat/installed_printers.dat
	 */
	FILE *printers;
	printers = fopen("dat/installed_printers.dat","r");
	/*
	 * Return with error if file does not exist
	 */
	if(!printers)
	{
		err_dialog("installed_printers.dat not found");
		return;
	}
	/*
	 * Line 1 and 2 hold fragments of data from lines in installed_printers.dat
	 */
	char line1[20];
	char line2[100];
	/*
	 * Loop through installed_printers.dat until end of file
	 */
	while(!feof(printers))
	{
		/*
		 * Initialize line1 to hold first 12 chars from installed_printers line
		 */
		fgets(line1,12,printers);
		/*
		 * If line1 is equal to "device for " then get line2
		 */
		if(strncmp(line1,"device for ",11) == 0)
		{
			/*
			 * Temp will be the final device name
			 */
			char temp[100];
			/*
			 * Initialize line2 to hold rest of line from installed_printers
			 * after first 12 chars -- if first 12 chars are "device for "
			 */
			fgets(line2,100,printers);
			line2[strcspn(line2,"\n")] = 0;
			/*
			 * Scrape through line2 and copy each character to temp until
			 * ':' is read
			 */
			for(int i = 0;i < 100; i++)
			{
				if(line2[i] == ':')
				{
					break;
				}
				temp[i] = line2[i];
				temp[i + 1] = '\0';
			}
			/*
			 * Temp now holds device name, add device name to linked list
			 * (If it is not a duplicate)
			 */
			if(check_duplicate_printer(temp) == 0)
			{
				populate_printer_list(temp);
			}
		}
	}
	write_printers();
	/*
	 * Close file pointer printers
	 */
	fclose(printers);
}

/*
 * Create linked list to hold names of installed printers
 */
void populate_printer_list(char device[])
{
	/*
	 * Allocate memory for new node using devices struct 
	 */
	struct devices *new_device = (struct devices*)malloc(sizeof(struct devices));
	/*
	 * Initialize new_device->device to device name of installed printer
	 */
	strncpy(new_device->device,device,strlen(device) + 1);
	/*
	 * Signify that this node is the last node in the list
	 */
	new_device->next = NULL;
	
	/*
	 * Make pointer to head of list
	 */
	struct devices *list = head1;
	/*
	 * If list is empty, insert node to head
	 */
	if(list == NULL)
	{
		list = new_device;
		list->next = head1;
		head1 = list;
	}
	/*
	 * If list is not empty, iterate until the end and add to tail
	 */
	else if(list != NULL)
	{
		while(list->next != NULL)
		{
			list = list->next;
		}
		list->next = new_device;
	}
	set_printerdex("MAX_LINE",get_printerdex("MAX_LINE") + 1);
}

/*
 * Check if a device name is already in the detected printers list
 * while(!feof(detected_printers.dat)) gets the last device twice
 */
int check_duplicate_printer(char device[])
{
	int val = 0;
	/*
	 * Point to head of list
	 */
	struct devices *printers = head1;
	/*
	 * Return if list is empty
	 */
	if(!printers)
	{
		return false;
	}
	/*
	 * If list head is not NULL, loop through list
	 */
	while(printers)
	{
		/*
		 * Check if device name to be inserted into list is the same
		 * as current node
		 */
		if(strcmp(printers->device,device) == 0)
		{
			/*
			 * If entry is duplicate, return 1. Otherwise return 0
			 */
			val = 1;
		}
		printers = printers->next;
	}
	return val;
}


/*
 * 
 */
void write_printers(void)
{
	clear_printer_win();
	set_printerdex("CURRENT",0);
	struct devices *lk = head1;
	char temp[100];
	if(lk == NULL)
	{
		err_dialog("No printers found");
		return;
	}
	else if(lk != NULL)
	{
		while(lk != NULL)
		{
			strncpy(temp,lk->device,strlen(lk->device) + 1);
			concat_blanks(48-strlen(lk->device),temp);
			write_to_printer_win(temp,1);
			lk = lk->next;
		}
	}
}

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
