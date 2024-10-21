#include <panel.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>

#include "../lib/order.h"
#include "../lib/err.h"
#include "../lib/print.h"
#include "../lib/report.h"
#include "../lib/scroll.h"
#include "../lib/file.h"
#include "../lib/draw.h"
#include "../lib/state.h"
#include "../lib/item.h"
#include "../lib/misc.h"

struct mod_t{
	char name[50];
	char msg[60];
	float charge;
	int mod_num;
	int mod_menu;
	struct mod_t* next;
};

struct order_t{
	char name[50];
	int itm_num;
	int qty;
	int highlight;
	int menu;
	float charge;
	float open_food;
	struct order_t* next;
	struct mod_t *child;
};

struct order_t *head=NULL;
struct mod_t *child=NULL;

/*
 * The main function for adding items to the current order.
 * 
 * The current order is a linked list that treats each 'node' as an 
 * individual item. This function controls adding new items to the list
 */
void add_itm(int itm_num, int menu)
{
	/*
	 * Return if desired item does not exist
	 */
	if(get_item_max() < itm_num)
	{
		return;
	}
	/*
	 * Create and initialize new node
	 */
	struct order_t *lk = (struct order_t*)malloc(sizeof(struct order_t));
	lk->itm_num = itm_num;
	lk->qty = 1;
	get_name(menu,lk->name,itm_num);
	lk->charge = get_itm(menu,"COST",itm_num);
	lk->menu = menu;
	lk->next = NULL;
	lk->child = NULL;
	/*
	 * Point to head of list 
	 */
	struct order_t *ord = head;
	/*
	 * If order list is empty place new node to head
	 */
	if(!ord)
	{
		/*
		 * Place new node at head of list and highlight entry on order_win
		 */
		ord = lk;
		ord->next = head;
		head = ord;
		highlight(0);
	}
	/*
	 * If order list is not empty, place new node to tail
	 */
	else if(ord)
	{
		/*
		 * Check duplicate returns 1 and adds 1 to ord->qty if an item
		 * is determined to be a duplicate (unmodified items only) 
		 * otherwise, it returns 0
		 */
		if(check_duplicate(itm_num,menu) == 0)
		{
			/*
			 * Find tail of list
			 */
			while(ord->next != NULL)
			{
				ord = ord->next;
			}
			/*
			 * Insert node to tail of list and highlight it
			 */
			ord->next = lk;
			highlight(total_cells()-1);
		}
	}
	/*
	 * Update index for scrolling order_win
	 */
	set_scrolldex("MAX_LINE",total_lines());
	/*
	 * If the item we are adding goes over scrolldex MAX, scroll to end
	 */
	if(get_scrolldex("MAX_LINE") == get_scrolldex("MAX")+3)
	{
		scroll_to_end();
	}
	/*
	 * If the item we are adding is a duplicate and exists in the first 
	 * 27 lines of the list, scroll to top
	 */
	else if(find_item_max_line() < 27)
	{
		scroll_to_top();
	}
	/*
	 * If item is in the middle of the list, but not visible focus 
	 * highlighted item
	 */
	else{
		int max = find_item_max_line();
		if(max + 15 >= get_scrolldex("MAX_LINE"))
		{
			scroll_to_end();
		}
		else{
			set_scrolldex("MIN",find_item_min_line()-11);
			set_scrolldex("MAX",find_item_max_line()+12);
		}
	}
}


/*
 * Check if an item is a duplicate of another (non-modified) item.
 */
int check_duplicate(int itm_num,int menu)
{
	struct order_t *ord = head;
	struct mod_t *mods;
	
	/*
	 * Loop through entire order and check if an item is a duplicate
	 */
	int i = 0;
	while(ord)
	{
		if((ord->itm_num == itm_num) & (ord->menu == menu))
		{
			/*
			 * Check if item is modified -- we do not count modified 
			 * items as duplicate. It is assumed that each modified item
			 * is unique.
			 */
			mods = ord->child;
			if(mods == NULL)
			{
				ord->qty++;
				if(get_state("PREV_ITM") != i)
				{
					highlight(i);
				}
				return 1;	
			}
		}
		i++;
		ord = ord->next;
	}
	
	return 0;
}

/*
 * Add open food charge to order
 */
void add_open_food(float total)
{
	struct order_t *lk = (struct order_t*) malloc(sizeof(struct order_t));
	strncpy(lk->name,"OPEN FOOD",10);
	lk->qty = 1;
	lk->itm_num = 0;
	lk->menu = 5;
	lk->open_food = total;
	lk->charge = total;
	lk->next = NULL;
	lk->child = NULL;
	
	struct order_t *ord = head;
	if(ord == NULL)
	{
		lk->next = head;
		head = lk;
		highlight(0);
		return;
	}
	while(ord->next != NULL)
	{
		ord = ord->next;
	}
	ord->next = lk;
	highlight(total_cells() - 1);
	set_scrolldex("MAX_LINE",total_lines());
	if(get_scrolldex("MAX") < get_scrolldex("MAX_LINE"))
	{
		scroll_to_end();
	}
}

/*
 * Change quantity of specified item.
 */
void modify_qty(int itm_num,int qty)
{
	struct order_t *ord = head;
	int i=0;
	if(itm_num == 0)
	{
		if(ord == NULL)
		{
			return;
		}
	}
	else if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
			if(ord == NULL)
			{
				err_dialog("COULD NOT ADJUST QUANTITY -- ILLEGAL POSITION");
				return;
			}
		}
	}
	ord->qty = qty;
}

/*
 * Calculate total number of items in order by passing through the list
 */
int total_items(void)
{
	int total=0;
	struct order_t *ord = head;
	if(ord == NULL)
	{
		total = 0;
		return total;
	}
	while(ord)
	{
		total += ord->qty;
		ord = ord->next;
	}
	return total;
}

/*
 * Get total unique number of items in order
 */
int total_cells(void)
{
	struct order_t *ord = head;
	int total = 0;
	if(!ord)
	{
		return 0;
	}
	while(ord)
	{
		total++;
		ord = ord->next;
	}
	return total;
}

/*
 * Get total number of lines from current order
 */
int total_lines(void)
{
	/*
	 * Make pointers to head and child lists
	 */
	struct order_t *ord = head;
	struct mod_t *mod;
	/*
	 * Set total lines explicitly to 0
	 */
	int total = 0;
	/*
	 * If order is null, it contains 0 lines
	 */
	if(!ord)
	{
		return 0;
	}
	/*
	 * While order is not null, find lines
	 */
	while(ord)
	{
		/*
		 * Each item has 3 lines by default
		 */
		total += 3;
		mod = ord->child;
		/*
		 * Pass through child list
		 */
		while(mod)
		{
			/*
			 * Each mod is +1 lines
			 */
			total++;
			/*
			 * Each charge has one mod line plus one price line
			 */
			if(mod->mod_menu == 4)
			{
				total++;
			}
			mod = mod->next;
		}
		ord = ord->next;
	}
	return total;
}

/*
 * Reset item price. Used by recall 
 */
void modify_price(int itm_num,float price)
{
	/*
	 * Point to head
	 */
	struct order_t *ord = head;
	int i=0;
	/*
	 * If node 0 is to be modified, check if it is NULL and return if 
	 * it is
	 */
	if(itm_num == 0)
	{
		if(ord == NULL)
		{
			return;
		}
	}
	/*
	 * Otherwise iterate to specified node 
	 */
	else if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
			if(ord == NULL)
			{
				return;
			}
		}
	}
	/*
	 * Set new price
	 */
	ord->charge = price;
	ord->open_food = price;
}

/*
 * Insert user message to mod (child) list
 */
void add_msg(int itm_num, char msg[])
{
	/*
	 * Point to head of order list
	 */
	struct order_t *ord = head;
	/*
	 * Allocate memory for new node to mod (child) list
	 */
	struct mod_t *new_mod = (struct mod_t*)malloc(sizeof(struct mod_t));
	/*
	 * Initialize new mod node -- only copying a message to mod_list->msg
	 */
	strncpy(new_mod->msg,msg,strlen(msg) + 1);
	new_mod->mod_menu = 0;
	new_mod->next = NULL;
	
	int i=0;
	/*
	 * Return if order list is null
	 */
	if(!ord)
	{
		return;
	}
	/*
	 * Iterate through order list until itm_num node is reached
	 */
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
		}
	}
	
	/*
	 * Point to base address of mod (child) list
	 */
	struct mod_t **mod_link = &ord->child;
	/*
	 * While mod (child) list is not null, iterate through list
	 */
	while(*mod_link)
	{
		mod_link = &(*mod_link)->next;
	}
	/*
	 * Insert new node into mod list
	 */
	*mod_link = new_mod;
	/*
	 * Update maximum possible line baesd on total_lines() and check
	 * to see if order_win needs to be scrolled based on 
	 * find_item_max_line()
	 */
	set_scrolldex("MAX_LINE",total_lines());
	int max_line = find_item_max_line();
	if(max_line > get_scrolldex("MAX"))
	{
		set_scrolldex("MAX",max_line);
		set_scrolldex("MIN",max_line-27);
	}
}

/*
 * This function deletes a specific node from the list.
 */
void del_itm(int itm_num)
{
	/*
	 * Point to head of list and make previos pointer
	 */
	struct order_t *temp = head, *prev;
	/*
	 * Find how many lines are occupied by item (for adjusting list position)
	 */
	int max_line = find_item_max_line();
	int max_diff = max_line - find_item_min_line();
	/*
	 * Return if list is NULL
	 */
	if(!temp)
	{
		return;
	}
	/*
	 * Iterate through list until itm_num node
	 */
	int i=0;
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			/*
			 * Set previous to current node, and continue to next node
			 */
			prev = temp;
			temp = temp->next;
			i++;
			
			if(temp == NULL)
			{
				return;
			}
		}
		/*
		 * Remove old link from list
		 */
		prev->next = temp->next;
	}
	/*
	 * If item number is 0, change head pointer
	 */
	else if(itm_num == 0)
	{
		head = head->next;
	}
	/*
	 * If mod or charge tab is selected, revert to food tab
	 */
	if((get_state("BSTATE") == 3) | (get_state("BSTATE") == 4))
	{
		set_state("BSTATE",1);
		shuffle_tabs();
		write_to_menu_buttons();
	}
	/*
	 * No item is highlighted after deleting a node
	 */
	set_state("HIGHLIGHT",0);
	set_state("PREV_ITM",-1);
	/*
	 * Update total lines in list for scrolling
	 */
	set_scrolldex("MAX_LINE",total_lines());
	/*
	 * If list is small enough to fit in order_win without scrolling
	 * go to top
	 */
	if(get_scrolldex("MAX_LINE") <= 27)
	{
		scroll_to_top();
	}
	/*
	 * If you have removed the tail element, scroll to bottom
	 */
	else if(max_line-max_diff == get_scrolldex("MAX_LINE"))
	{
		scroll_to_end();
	}
	else{
		int min = get_state("MIN_LINE");
		set_scrolldex("MIN",min-max_diff);
		set_scrolldex("MAX",(min-max_diff)+27);
		//set_scrolldex("MAX",max_line + max_diff);
		//set_scrolldex("MIN",(max_line + max_diff)-27);
	}
}

/*
 * Delete entire list. 
 */	
void del_order(void)
{
	struct order_t *temp = head;
	if(temp)
	{
		head = NULL;
		temp->next = NULL;
		set_state("HIGHLIGHT",0);
		set_scrolldex("MAX_LINE",1);
		set_scrolldex("MIN",0);
		set_scrolldex("MAX",27);
	}
}

/*
 * Find how many lines each item occupies in order_win (for highlight)
 */
int find_item_lines(int line)
{
	/*
	 * order_win is drawn to the 5th line of stdscr, but line 0 is 
	 * not used. So to get the position of the current line, add min
	 * value from scrolldex and subtract 6
	 */
	line = (line + get_scrolldex("MIN")) - 6;
	int i=0,j,item=0;
	
	struct order_t *ord = head;
	struct mod_t *mod;
	
	if(!ord)
	{
		return false;
	}
	/*
	 * i tracks the maximum line for current item, and j tracks the 
	 * number of lines each item occupies
	 */
	while(ord)
	{
		j=3;
		i+=3;
		mod = ord->child;
		while(mod)
		{
			i++;
			j++;
			if(mod->mod_menu == 4)
			{
				i++;
				j++;
			}
			mod = mod->next;	
		}
		/*
		 * i - j gives the minimum line, and i is the maximum line for current item
		 * using this, we check to see if the current item was selected
		 */
		if((line >= i - j) & (line < i))    
		{
			return item;
		}
		item++;
		ord = ord->next;
	}
	/*
	 * Return invalid response if no item was selected
	 */
	return -1;
}

/*
 * Find maximum line of highlighted item. Used to move order list
 * for making all modifications to an item visible.
 */
int find_item_max_line(void)
{
	/*
	 * Point to order list
	 */
	struct order_t *ord = head;
	struct mod_t *mod;
	/*
	 * Return false if order list is NULL
	 */
	if(!ord)
	{
		return false;
	}
	/*
	 * Track lines traversed 
	 */
	int i = 0;
	/*
	 * While order list is not null, iterate through list
	 */
	while(ord)
	{
		/*
		 * Each item by default has three lines
		 */
		i += 3;
		/*
		 * Point to child list
		 */
		mod = ord->child;
		/*
		 * While child list is not null, iterate through list
		 */
		while(mod)
		{
			/*
			 * Each mod has at least one line, so we count one more
			 */
			i++;
			/*
			 * If mod menu = 4, an extra line is drawn, so we count one more
			 */
			if(mod->mod_menu == 4)
			{
				i++;
			}
			/*
			 * Point to next node in child list
			 */
			mod = mod->next;
		}
		/*
		 * If we are counting the highlighted item, this is as far as we
		 * need to go, and we return the maximum line of the list
		 */
		if(ord->highlight == 1)
		{
			return i;
		}
		/*
		 * Otherwise, point to next node in order list
		 */
		ord = ord->next;
	}
	/*
	 * If we made it here, there is no useful information. Return false
	 */
	return false;
}

/*
 * Find minimum line occupied by an item (Used by remove mods and del_item)
 */
int find_item_min_line(void)
{
	/*
	 * Point to head of order list and create pointer for mods list
	 */
	struct order_t *ord = head;
	struct mod_t *mod;
	/*
	 * Return false if order is empty
	 */
	if(!ord)
	{
		return false;
	}
	/*
	 * Track number of lines traversed 
	 */
	int i=0;
	/*
	 * Begin iterating through order
	 */
	while(ord)
	{
		/*
		 * If current node is the highlighted node, return current position
		 */
		if(ord->highlight == 1)
		{
			return i;
		}
		/*
		 * Add 3 default lines
		 */
		i += 3;
		/*
		 * Point to child list
		 */
		mod = ord->child;
		/*
		 * Begin iterating through child list
		 */
		while(mod)
		{
			/*
			 * Add 1 default line
			 */
			i += 1;
			/*
			 * If extra charge, add one more line
			 */
			if(mod->mod_menu == 4)
			{
				i += 1;
			}
			/*
			 * Continue to next node in mod list if any
			 */
			mod = mod->next;
		}
		/*
		 * Continue to next node in order list if any
		 */
		ord = ord->next;
	}
	return false;
}

/*
 * If an item is an open food charge, this retrieves it's cost from
 * the list
 */
float get_of_val(int itm_num)
{
	/*
	 * Declare float to hold value, and point to head of order list
	 */
	float val;
	struct order_t *ord = head;
	/*
	 * Return false if order is empty
	 */
	if(ord == NULL)
	{
		return false;
	}
	/*
	 * Track position in list
	 */
	int i;
	/*
	 * Continue traversing list until desired node reached
	 */
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
		}
	}
	/*
	 * Return open_food value from desired node in list
	 */
	val = ord->open_food;
	return val;
}

/*
 * Send visible lines from order to order_win for display
 */
void write_list(void)
{
	/*
	 * Clear order window 
	 */
	clear_order_win();
	
	
	struct order_t *ord = head;
	struct mod_t *mod;
	char *menu = (char *)malloc(51);
	char *name = (char *)malloc(51);
	char *details = (char *)malloc(101);
	float charges=0;
	set_scrolldex("CURRENT",0);
	
	/*
	 * Return if order is empty
	 */
	if(!ord)
	{
		return;
	}
	/*
	 * Begin iterating through list
	 */
	while(ord)
	{
		/*
		 * Explicitly set charges to 0
		 */
		charges = 0;
		/*
		 * Copy name and some blanks for higligting
		 */
		strncpy(name,ord->name,strlen(ord->name) + 1);
		concat_blanks(34-strlen(name),name);
		write_to_order_win(name,1,ord->highlight);
		if(ord->menu != 5)
		{
			charges += ord->charge;
		}
		mod = ord->child;
		/*
		 * Begin iterating through child list
		 */
		while(mod != NULL)
		{
			if(mod->mod_menu == 0)
			{
				strncpy(details,"   ",4);
				strncat(details,mod->msg,strlen(mod->msg) + 1);
				concat_blanks(31-strlen(mod->msg),details);
				write_to_order_win(details,0,ord->highlight);
			}
			if(mod->mod_menu > 0)
			{
				strncpy(details,"   ",4);
				strncat(details,mod->name,strlen(mod->name)+1);
				concat_blanks(34-strlen(details),details);
				write_to_order_win(details,0,ord->highlight);
			}
			if(mod->mod_menu == 4)
			{
				sprintf(details,"    +$%.2f",get_itm(4,"COST",mod->mod_num));
				concat_blanks(34-strlen(details),details);
				write_to_order_win(details,0,ord->highlight);	
				charges += get_itm(4,"COST",mod->mod_num);
			}
			mod = mod->next;
		}
		if(ord->menu < 5)
		{
			sprintf(details,"x%d        @$%.2f",ord->qty,(charges * ord->qty));
		}
		else if(ord->menu == 5)
		{
			charges += ord->open_food;
			sprintf(details,"x%d        @$%.2f",ord->qty,(charges*ord->qty));
		}
		concat_blanks(34-strlen(details),details);
		write_to_order_win(details,0,ord->highlight);
		write_to_order_win("----------------------------------",0,0);
		ord = ord->next;
	}
	free(name);
	free(menu);
	free(details);
}

/*
 * Find which menu an item in the order is associated with. 
 */
int which_menu(int itm_num)
{
	struct order_t *ord = head;
	if(ord == NULL)
	{
		err_dialog("ITM NOT FOUND");
		return false;
	}
	int i=0,val;
	while(i < itm_num)
	{
		ord = ord->next;
		i++;
	}
	val = ord->menu;
	return val;
}

/*
 * Select specified item from order
 */
void highlight(int itm_num)
{
	struct order_t *ord = head;
	/*
	 * Return if order is NULL or an illegal position is selected
	 */
	if((!ord) | (itm_num >= total_cells()))
	{
		return;
	}
	/*
	 * Use i to track position in the loop
	 */
	int i=0;
	while(i < itm_num)
	{
		/*
		 * If itm_num = 0, we do not need to keep looking
		 */
		if(itm_num == 0)
		{
			break;
		}
		/*
		 * Otherwise, increment i, and go to next link in the list
		 */
		i++;
		ord = ord->next;
	}
	/*
	 * If something is already highlighted
	 */
	if(get_state("HIGHLIGHT") == 1)
	{
		/*
		 * Use *temp to iterate through list without changing ord
		 */
		struct order_t *temp = head;
		/*
		 * Return if something goes wrong
		 */
		if(!temp)
		{
			return;
		}
		/*
		 * Set entire list non-highlight
		 */
		while(temp)
		{
			temp->highlight = 0;
			temp = temp->next;
		}
		/*
		 * Tell system nothing is highlighted
		 */
		set_state("HIGHLIGHT",0);
		/*
		 * If an already highlighted item was selected
		 */
		if(itm_num == get_state("PREV_ITM"))
		{
			/*
			 * Tell system, there is no previously selected item
			 */
			set_state("PREV_ITM",-1);
			/*
			 * Show changes in order_win
			 */
			write_list();
			/*
			 * If mod or extra charge menu is visible, reset to food menu visible
			 */
			if((get_state("BSTATE") == 3) | (get_state("BSTATE") == 4))
			{
				set_state("BSTATE",1);
				shuffle_tabs();
				write_to_menu_buttons();
			}
			return;
		}
		set_state("PREV_ITM",-1);
	}
	/*
	 * If a non-highlighted item was selected, set it to be highlighted
	 */
	ord->highlight = 1;
	/*
	 * Tell system which item is selected
	 */
	set_state("PREV_ITM",i);
	/*
	 * Tell system an item is highlighted
	 */
	set_state("HIGHLIGHT",1);
	/*
	 * Record minimum line of highlighted item
	 */
	set_state("MIN_LINE",get_scrolldex("MIN"));
	/*
	 * Show changes in order_win
	 */
	write_list();
}
 

/*
 * Write order list to file, print file, and create/update daily report 
 */
void save_order(float paid,float change)
{	
	char full_name[100];
	
	/*
	 * Get length of file name string and allocate it
	 */
	int length = snprintf(NULL,0,"%d",find_orderno());
	char* orderno = malloc(length + 1);
	snprintf(orderno,length + 5,"%d",find_orderno() +1);
	
	/*
	 * Assemble path string
	 */
	get_dir_date(full_name);
	strncat(full_name,"order-",7);
	strncat(full_name,orderno,strlen(orderno) + 1);
	
	/*
	 * Attempt to create order receipt (file)
	 */
	FILE *cur_order;
	cur_order = fopen(full_name,"w");
	
	/*
	 * Return on failure
	 */
	if(cur_order == NULL)
	{
		return;
	}
	/*
	 * Set some pointers to our linked list and allocate some memory for strings
	 */
	struct order_t *ord = head;
	struct mod_t *mods;

	char *mod = (char *)malloc(50);
	char *name = (char *)malloc(50);
	char *msg = (char *)malloc(50);
	char *time = (char *)malloc(50);
	
	/*
	 * Begin building receipt with msg= value from .conf (if any) 
	 * Also print order number, and date
	 */
	get_file_data(".conf","msg=",name);
	int start=0,i=0;
	/*
	 * If message from .conf is less than 28 chars, center single line
	 * and print message
	 */
	if(strlen(name) < 27)
	{
		/*
		 * Find out how many spaces to copy for centering message
		 */
		start = 28-strlen(name);
		for(i = 0; i < start/2; i++)
		{
			fprintf(cur_order," ");
		}
		fprintf(cur_order,"%s\n\n",name);
	}
	/*
	 * If .conf message is more than 28 chars, separate lines based on
	 * whitespace and message length (100 char max)
	 */
	else if(strlen(name) > 27)
	{
		char temp[50];
		int j=0;
		/*
		 * Loop through message 
		 */
		while(i < strlen(name))
		{
			/*
			 * Set i to rightmost position or i + 27
			 */
			if(i + 27 < strlen(name))
			{
				i += 27;
			}
			else{
				i = strlen(name);
			}
			/*
			 * Start moving backwards (left) through message looking for
			 * whitespace
			 */
			for(int k = i; k > j;k--)
			{
				/*
				 * If whitespace is found, preserve position and break loop
				 */
				if(name[k] == ' ')
				{
					i = k;
					break;
				}
			}
			/*
			 * Copy partial message from i (rightmost position) to j
			 * (beginning position)
			 */
			copy_fragment(name,j,i,temp);
			/*
			 * After copying, set j = i to change starting index
			 */
			j = i;
			/*
			 * Find out how many spaces (if any) to copy in front of
			 * message fragment
			 */
			start = 28-strlen(temp);
			for(int k = 0;k < start/2; k++)
			{
				fprintf(cur_order," ");
			}
			/*
			 * Print message fragment
			 */
			fprintf(cur_order,"%s\n",temp);
		}
		fprintf(cur_order,"\n");
	}
	get_file_data(".conf","contact=",name);
	start = 28-strlen(name);
	for(int k =0;k < strlen(name)/2;k++)
	{
		fprintf(cur_order," ");
	}
	fprintf(cur_order,"%s\n\n\n",name);
	fprintf(cur_order,"Order no. %d\n",find_orderno());
	get_time(time);
	fprintf(cur_order,"%s\n\n",time);
	free(time);
	fprintf(cur_order,"-------------------\n");
	/*
	 * Begin iterating through list
	 */
	while(ord)
	{
		/*
		 * These if blocks set us up to retrieve our information from 
		 * the correct menu files. Also checks for special message
		 */
		float itm_price = 0;
		if(ord->menu < 5)
		{
			strncpy(name,ord->name,strlen(ord->name) + 1);
		}
		else if(ord->menu == 5)
		{
			strncpy(name,"OPEN FOOD\0",11);
		}
		fprintf(cur_order,"%s\n",name);
		mods = ord->child;
		/*
		 * Begin iterating through child list
		 */
		while(mods)
		{
			/*
			 * As above, these if blocks make sure we retrieve from the 
			 * correct files
			 */
			if(mods->mod_menu == 4)
			{
				strncpy(mod,"CHARGE\0",8);
				itm_price += (get_itm(4,"COST",mods->mod_num)*ord->qty);
			}
			if(strlen(mods->msg) > 0)
			{
				fprintf(cur_order,"   %s\n",mods->msg);
			}
			strncpy(mod,mods->name,strlen(mods->name) + 1);
			/*
			 * Regular mod vs extra charge
			 */
			if(mods->mod_menu == 3)
			{
				fprintf(cur_order,"   %s\n",mod);
			}
			else if(mods->mod_menu == 4)
			{
				fprintf(cur_order,"   %s\n\t$%.2f\n",mod,get_itm(4,"COST",mods->mod_num));
			}
			mods = mods->next;
		}
		/*
		 * Print quanity and cost for total items @ qty
		 */
		//fprintf(cur_order,"x%d\t$%.2f\n",ord->qty,(get_itm(ord->menu,"COST",ord->itm_num) * (ord->qty) + itm_price));
		fprintf(cur_order,"x%d\t$%.2f\n",ord->qty,(ord->charge * ord->qty) + itm_price);
		ord = ord->next;
	}
	fprintf(cur_order,"-------------------\n\n");
	fprintf(cur_order,"Items Sold:\t%d\n",total_items());
	fprintf(cur_order,"Total  Due:\t$%.2f\n",calc_total());
	fprintf(cur_order,"Total Paid:\t$%.2f\n",paid);
	fprintf(cur_order,"Change Due:\t$%.2f\n",change);
	
	fclose(cur_order);
	free(msg);
	free(orderno);
	free(name);
	free(mod);
	print_reciept(full_name);
}

/*
 * Add modifier to a specified item
 */
void add_mod(int itm_num,int mod_num,int menu)
{
	/*
	 * Allocate memory for a new mod node, and initialize
	 */
	struct mod_t *new_mod = (struct mod_t*)malloc(sizeof(struct mod_t));
	get_name(menu,new_mod->name,mod_num);
	if(menu == 4)
	{
		new_mod->charge = get_itm(4,"COST",itm_num);
	}
	new_mod->msg[0] = '\0';
	new_mod->mod_num = mod_num;
	new_mod->mod_menu = menu;
	new_mod->next = NULL;
	
	/*
	 * Begin iterating through parent list until specified node
	 */
	struct order_t *new_node = head;
	if(new_node == NULL)
	{
		return;
	}
	int i=0;
	while(i < itm_num)
	{
		new_node = new_node->next;
		i++;
	}
	/*
	 * Fail if specified node does not exist
	 */
	if(new_node == NULL)
	{
		err_dialog("COULD NOT ADD MOD -- ILLEGAL POSITION");
		return;
	}
	/*
	 * Get base address of child list head, and insert new node
	 */
	struct mod_t **mod_link = &new_node->child;
	while(*mod_link)
	{
		mod_link = &(*mod_link)->next;
	}
	*mod_link = new_mod;
	set_scrolldex("MAX_LINE",total_lines());
	int max_line = find_item_max_line();
	if(max_line > get_scrolldex("MAX"))
	{
		set_scrolldex("MAX",max_line);
		set_scrolldex("MIN",max_line-27);
	}
	write_list();
}

/*
 * Sets the child list of a given item number to NULL. //deletes child (mod) list
 */
void remove_mods(int itm_num)
{ 
	/*
	 * Point to head
	 */
	struct order_t *itm = head;
	/*
	 * Return if null
	 */
	if(!itm)
	{
		return;
	}
	/*
	 * Iterate through list until itm_num is reached
	 */
	int i=0;
	while(i < itm_num)
	{
		itm = itm->next;
		i++;
	}
	/*
	 * Set child (mod) list to NULL
	 */
	//int max_line = find_item_min_line();
	itm->child = NULL;
	/*
	 * Update maximum number of lines in order for scrolling
	 */
	set_scrolldex("MAX_LINE",total_lines());
	/*
	 * Find max item line for itm_num and difference between max item line
	 * and maximum visible line of order list 
	 */
	int max_line = find_item_min_line();
	int max_diff = get_scrolldex("MAX") - max_line;
	/*
	 * If order list is under 27 lines, reset scrolldex
	 */
	if(get_scrolldex("MAX_LINE") <= 27)
	{
		scroll_to_top();
	}
	/*
	 * If maximum item line number is the maximum line, scroll to end
	 */
	else if(max_line == get_scrolldex("MAX_LINE")-3)
	{
		scroll_to_end();
	}
	/*
	 * Otherwise, draw entries below highlighted item and reserve list
	 * position
	 */
	else if(max_diff > 0)
	{
		set_scrolldex("MAX",get_state("MIN_LINE")+27);
		set_scrolldex("MIN",get_state("MIN_LINE"));
	}
}

/*
 * Calculate total price due by passing through entire list.
 */
float calc_total()
{
	float total=0;
	/*
	 * Point to head of list with *ord, and child(mod) list with *mods
	 */
	struct order_t *ord = head;
	struct mod_t *mods;
	/*
	 * If *ord has value, continue with loop
	 */
	while(ord)
	{
		/*
		 * Check menu value to find correct charge value
		 */
		if(ord->menu < 5)
		{
			total += (get_itm(ord->menu,"COST",ord->itm_num) * (ord->qty));
		}
		else if(ord->menu == 5)
		{
			total += (ord->open_food * ord->qty);
		}
		/*
		 * Check if *mods has value, if so, check for extra charges
		 */
		mods = ord->child;
		while(mods)
		{
			if(mods->mod_menu == 4)
			{
				total += (get_itm(4,"COST",mods->mod_num) * (ord->qty));
			}
			mods = mods->next;
		}
		ord = ord->next;
	}
	return total;
}
