#include "../lib/justpos.h"

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
	char type[50];
	if(menu == 1)
	{
		strncpy(type,"FOOD",5);
	}
	else if(menu == 2)
	{
		strncpy(type,"DRINK",6);
	}
	else if(menu == 3)
	{
		strncpy(type,"MOD",4);
	}
	else if(menu == 4)
	{
		strncpy(type,"CHARGE",7);
	}
	if(get_item_max(type) < itm_num)
	{
		return;
	}
	
	if(total_items() == 0)
	{
		insert_head(itm_num,menu,type);
		highlight(0);
	}
	else if(total_items() > 0)
	{
		if(check_duplicate(itm_num,menu) == 0)
		{
			insert_tail(itm_num,menu,type);
			highlight(total_cells()-1);
			write_pages();
		}
		else{
			return;
		}
	}
	set_scrolldex("MAX_LINE",total_lines());
	if(get_scrolldex("MAX_LINE") > get_scrolldex("MAX"))
	{
		scroll_to_end();
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
				if(((i/12) + 1) != get_index("ORDER"))
				{
					set_index("ORDER",(i/12) + 1);
					write_list();
					write_pages();
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
 * Insert node at head of parent list
 */
void insert_head(int itm_num, int menu, char name[])
{
	/*
	 * Allocate memory and initialize 
	 */
	struct order_t *lk = (struct order_t*) malloc(sizeof(struct order_t));
	lk->itm_num = itm_num;
	lk->qty = 1;
	get_name(menu,lk->name,itm_num);
	lk->charge = get_itm(menu,"COST",itm_num);
	lk->menu = menu;
	lk->next=head;
	lk->child = NULL;
	/*
	 * Insert node to head
	 */
	head=lk;
}

/*
 * This function adds an item to the end of the parent list. 
 */
void insert_tail(int itm_num, int menu, char name[])
{
	/*
	 * Allocate memory and initialize 
	 */
	struct order_t *lk = (struct order_t*) malloc(sizeof(struct order_t));
	lk->itm_num = itm_num;
	lk->qty = 1;
	get_name(menu,lk->name,itm_num);
	lk->charge = get_itm(menu,"COST",itm_num);
	lk->menu = menu;
	lk->next = NULL;
	lk->child = NULL;
	/*
	 * Find end of parent list
	 */
	struct order_t *list = head;
	while(list->next != NULL)
	{
		list = list->next;
	}
	/*
	 * Insert new node to tail
	 */
	list->next = lk;
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
	//lk->msg[0] = '\0';
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
	struct order_t *ord = head;
	struct mod_t *mod;
	int total = 0;
	if(!ord)
	{
		return 0;
	}
	while(ord)
	{
		total += 3;
		mod = ord->child;
		while(mod)
		{
			total++;
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
				return;
			}
		}
	}
	ord->open_food = price;
}

/*
 * Add special message or instruction to an item in order (eg ALLERGY
 * or CHEESE ON HALF)
 */
void add_msg(int itm_num, char msg[])
{
	struct order_t *ord = head;
	struct mod_t *new_mod = (struct mod_t*)malloc(sizeof(struct mod_t));
	
	strncpy(new_mod->msg,msg,strlen(msg) + 1);
	new_mod->mod_menu = 0;
	new_mod->next = NULL;
	
	int i=0;
	if(!ord)
	{
		return;
	}
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
		}
	}
	
	struct mod_t **mod_link = &ord->child;
	while(*mod_link)
	{
		mod_link = &(*mod_link)->next;
	}
	*mod_link = new_mod;
	//strncpy(ord->msg,msg,50);
	set_scrolldex("MAX_LINE",total_lines());
}

/*
 * This function deletes a specific node from the list.
 */
void del_itm(int itm_num)
{
	struct order_t *temp = head, *prev;
	
	int i=0;
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			prev = temp;
			temp = temp->next;
			i++;
			
			if(temp == NULL)
			{
				return;
			}
		}
		prev->next = temp->next;
	}
	else if(itm_num == 0)
	{
		head = head->next;
	}
	if((get_state("BSTATE") == 3) | (get_state("BSTATE") == 4))
	{
		set_state("BSTATE",1);
		shuffle_tabs();
		write_to_menu_buttons();
		write_pages();
		update_panels();
		doupdate();
	}
	set_state("HIGHLIGHT",0);
	set_state("PREV_ITM",-1);
	set_scrolldex("MAX_LINE",total_lines());
	if(get_scrolldex("MAX") > get_scrolldex("MAX_LINE"))
	{
		if(get_scrolldex("MAX_LINE") < 27)
		{
			scroll_to_top();
			return;
		}
		scroll_to_end();
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
 * If an item is an open food charge, this retrieves it's cost from
 * the list
 */
float get_of_val(int itm_num)
{
	float val;
	struct order_t *ord = head;
	if(ord == NULL)
	{
		return false;
	}
	
	int i;
	if(itm_num > 0)
	{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
		}
	}
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
			if(mod->mod_menu == 3)
			{
				strncpy(menu,"MOD",4);
			}
			else if(mod->mod_menu == 4)
			{
				strncpy(menu,"CHARGE",7);
				charges += get_itm(4,"COST",mod->mod_num);
			}
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
			}
			mod = mod->next;
		}
		if(ord->menu < 5)
		{
			sprintf(details,"x%d\t\t@$%.2f",ord->qty,(charges * ord->qty));
		}
		else if(ord->menu == 5)
		{
			charges += ord->open_food;
			sprintf(details,"x%d\t\t@$%.2f",ord->qty,(charges * ord->qty));
		}
		concat_blanks(25-strlen(details),details);
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
	//if(itm_num > 0)
	//{
		while(i < itm_num)
		{
			ord = ord->next;
			i++;
		}
	//}
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
				update_panels();
				doupdate();
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
	 * Show changes in order_win
	 */
	write_list();
}
 

/*
 * 
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
	fprintf(cur_order,"%s\n\n",name);
	fprintf(cur_order,"Order no. %d\n",find_orderno());
	get_time(time);
	fprintf(cur_order,"%s\n\n",time);
	free(time);
	fprintf(cur_order,"------------------\n");
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

	/*
	 * Generate daily.rpt (In case of program crash)
	 * 
	 * Probably more efficient to get the value from daily.rpt and add
	 * value of total_sales...
	 */
	char date[100];
	get_dir_date(date);
	gen_daily(date);
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
}

/*
 * Sets the child list of a given item number to NULL. //deletes child (mod) list
 */
void remove_mods(int itm_num)
{ 
	struct order_t *itm = head;
	int i=0;
	while(i < itm_num)
	{
		itm = itm->next;
		i++;
	}
	itm->child = NULL;
	set_scrolldex("MAX_LINE",total_lines());
	if(get_scrolldex("MAX") > get_scrolldex("MAX_LINE"))
	{
		scroll_to_end();
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
