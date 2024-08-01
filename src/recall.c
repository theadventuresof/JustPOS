#include "../lib/justpos.h"
#include <dirent.h>

DIR *ordp;
struct dirent *dirord;

/*
 * This structure holds the names of orders from a specified day.
 */
struct recall_orders{
	char order[50];
	struct recall_orders *next;
}recall_orders;

struct recall_orders *top = NULL;
struct recall_orders *next = NULL;

/*
 * Insert file name string 'order-*' into list.
 */
void populate_recall_list(char order[])
{
	/*
	 * Allocate and initialize new node
	 */
	struct recall_orders *new_ord = (struct recall_orders*)malloc(sizeof(struct recall_orders));
	strncpy(new_ord->order,order,strlen(order)+1);
	new_ord->next = NULL;
	
	struct recall_orders *lk = top;
	/*
	 * If top of list is NULL, insert to top/head
	 */
	if(!lk)
	{
		lk = new_ord;
		lk->next = top;
		top = lk;
	}
	/*
	 * If top of list is initialized, insert to tail
	 */
	else if(lk != NULL)
	{
		/*
		 * Traverse list until lk->next = NULL
		 */
		while(lk->next)
		{
			lk = lk->next;
		}
		lk->next = new_ord;
	}
}

/*
 * Look through the given date folder for files beginning with 'order-'
 */
void find_recall_list(char date[],int func)
{
	int i=1;
	/*
	 * If folder exists...
	 */
	if((ordp = opendir(date)) != NULL)
	{
		/*
		 * Attempt to read...
		 */
		while((dirord = readdir(ordp)) != NULL)
		{
			/*
			 * If read was successfull...
			 */
			 
			/*
			 * If func = 1, orders are being sent to the list
			 */
			if(func == 1)
			{
				/*
				 * If filename starts with "order-"
				 */
				if(strncmp(dirord->d_name,"order-",6) == 0)
				{
					/*
					 * Send file name string to be inserted into linked list
					 */
						populate_recall_list(dirord->d_name);
						i++;
				}
			}
			/*
			 * If func = 2, directories are being send to the list
			 */
			else if(func == 2)
			{
				/*
				 * If entry is a directory
				 */
				if(dirord->d_type == DT_DIR)
				{
					/*
					 * Skip adding . and ..
					 */
					if((strcmp(dirord->d_name,".") == 0) | (strcmp(dirord->d_name,"..") == 0))
					{
						continue;
					} 
					/*
					 * Send entries to list
					 */
					populate_recall_list(dirord->d_name);
					i++;
				}
			}
			set_recalldex("MAX_LINE",i);
		}
		/*
		 * Close directory stream
		 */
		closedir(ordp);
	}
	/*
	 * Print error on failure
	 */ 
	else{
		err_dialog("COULD NOT OPEN ORDER DIRECTORY");
	}
}

/*
 * Set recall_orders lsit to NULL
 */
void del_recall_list(void)
{
	struct recall_orders *ord = top;
	if(!ord)
	{
		return;
	}
	if(ord)
	{
		top = NULL;
		ord->next = NULL;
	}
}

/*
 * Iterate through list, and pass orders to write_to_recall_win for display
 */
void write_recall(void)
{
	char line[75];
	/*
	 * Copy blanks to every position in recall_win and set the current line to 0
	 */
	clear_recall_win();
	set_recalldex("CURRENT",0);
	struct recall_orders *ord = top;
	/*
	 * Return if list is NULL
	 */
	if(!ord)
	{
		return;
	}
	while(ord)
	{
		/*
		 * Copy blanks to fill line -- for highlighting
		 */
		strncpy(line,ord->order,strlen(ord->order) + 1);
		concat_blanks(34-strlen(ord->order),line);
		/*
		 * Send lines to be written -- 2 is highlight
		 */
		if(get_recalldex("CURRENT") == get_recalldex("LINE"))
		{
			write_to_recall(line,2);
		}
		else{
			write_to_recall(line,1);
		}
		ord = ord->next;
	}
}

/*
 * Find which line was touched in recall_win
 */
int find_reacall_lines(int y)
{
	int i=0;
	/*
	 * recall_win begins on line 5, but does not write to line 0,
	 * so, by finding the minimum position of recall_win and subtracting
	 * 6, the touched line is also found
	 */
	y = (y + get_recalldex("MIN")) - 6;
	struct recall_orders *ord = top;
	/*
	 * Return -1 if list is NULL
	 */
	if(!ord)
	{
		return -1;
	}
	while(ord)
	{
		/*
		 * Return touched line if a match is found
		 */
		if(y == i)
		{
			return i;
		}
		i++;
		ord = ord->next;
	}
	/*
	 * Return -1 if no match is found
	 */
	return -1;
}

/*
 * Get date viewing from recalldex, and append a selected order
 */
void append_order_recall(char *order)
{
	struct recall_orders *ord = top;
	/*
	 * Return if list is NULL
	 */
	if(!ord)
	{
		return;
	}
	int i = 0;
	/*
	 * Iterate through list until line is matched
	 */
	while(ord)
	{
		if(i == get_recalldex("LINE"))
		{
			/*
			 * Concat list item to external string
			 */
			strncat(order,ord->order,strlen(ord->order) + 1);
			return;
		}
		i++;
		ord = ord->next;
	}
	
}
