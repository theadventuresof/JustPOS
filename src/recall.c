#include <panel.h>
#include <string.h>
#include <stdlib.h>
#include <dirent.h>

#include "../lib/recall.h"
#include "../lib/err.h"
#include "../lib/draw.h"
#include "../lib/draw_state_4.h"
#include "../lib/scroll.h"

DIR *ordp;
struct dirent *dirord;

struct recall_ord *top = NULL;
struct recall_ord *next = NULL;

/*
 * 
 */
struct recall_ord *sort_recall(void)
{
	int len = get_length(top);
	int itr = 0;
	int swapped = 0;
	
	while(itr < len)
	{
		struct recall_ord *traverse_node = top;
		struct recall_ord *prev_node = top;
		swapped = 0;
		
		while(traverse_node->next != NULL)
		{
			struct recall_ord *temp = traverse_node->next;
			int temp1 = get_orderno(traverse_node->order);
			int temp2 = get_orderno(temp->order);
			if(temp1 > temp2)
			{
				swapped = 1;
				if(traverse_node == top)
				{
					traverse_node->next = temp->next;
					temp->next = traverse_node;
					prev_node = temp;
					top = prev_node;
				}
				else{
					traverse_node->next = temp->next;
					temp->next = traverse_node;
					prev_node->next = temp;
					prev_node = temp;
				}
				continue;
			}
			prev_node = traverse_node;
			traverse_node = traverse_node->next;
		}
		if(!swapped)
		{
			break;
		}
		itr++;
	}
	doupdate();
	return top;
}

/*
 * 
 */
int get_orderno(char order[])
{
	int val;
	if((get_recalldex("STATE") == 4) | (get_recalldex("STATE") == 2))
	{
		val = strtof(order,NULL);
	}
	else if(get_recalldex("STATE") == 1)
	{
		char *temp = order + 5;
		val = strtof(temp,NULL);
	}
	return val;
}

/*
 * 
 */
int get_length(struct recall_ord *top)
{
	int i=0;
	if(top == NULL)
	{
		return 0;
	}
	while(top->next)
	{
		i++;
		top = top->next;
	}
	return i;
}

/*
 * Insert file name string 'order-*' into list.
 */
void populate_recall_list(char order[])
{
	/*
	 * Allocate and initialize new node
	 */
	struct recall_ord *new_ord = (struct recall_ord*)malloc(sizeof(struct recall_ord));
	strncpy(new_ord->order,order,strlen(order)+1);
	new_ord->next = NULL;
	
	struct recall_ord *lk = top;
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
						sort_recall();	
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
					sort_recall();
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
	struct recall_ord *ord = top;
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
	struct recall_ord *ord = top;
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
	struct recall_ord *ord = top;
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
	struct recall_ord *ord = top;
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

/*
 * Check for specified order number in "order-n" context in linked list
 */
int find_recall_order(char val[])
{
	int line = -1,i=0;
	
	struct recall_ord *ord = top;
	if(!ord)
	{
		return -1;
	}
	while(ord)
	{
		if(strcmp(val,ord->order) == 0)
		{
			line = i;
		}
		i++;
		ord = ord->next;
	}
	
	return line;
}

/*
 * Count list nodes in recall_orders list
 */
int count_recall(void)
{
	int count = 0;
	
	struct recall_ord *ord = top;
	
	if(!ord)
	{
		return -1;
	}
	while(ord)
	{
		count++;
		ord = ord->next;
	}
	
	return count;
}
