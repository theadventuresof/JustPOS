#include "../lib/justpos.h"

/*
 * Keep track of what is visible so that we know how to use input
 */
struct status{
	int state;
	int prev_state;
	int bstate;
	int highlight;
	int prev_itm;
	int current_cell;
}status;

/*
 * Track maximum number of items in the menus
 */
struct itemmax{
	int max_food;
	int max_drink;
	int max_mod;
	int max_charge;
}itemmax;


/*
 * Function for changing current system state
 */
void set_state(char type[],int val)
{
	if(strcmp("STATE",type) == 0)
	{
		status.state = val;
	}
	else if(strcmp("BSTATE",type) == 0)
	{
		status.bstate = val;
	}
	else if(strcmp("HIGHLIGHT",type) == 0)
	{
		status.highlight = val;
	}
	else if(strcmp("PREV_ITM",type) == 0)
	{
		status.prev_itm = val;
	}
	else if(strcmp("CUR_CELL",type) == 0)
	{
		status.current_cell = val;
	}
	else if(strcmp("PREV_STATE",type) == 0)
	{
		status.prev_state = val;
	}
}


/*
 * Retrieve details about system state
 */
int get_state(char type[])
{
	int val;
	if(strcmp("STATE",type) == 0)
	{
		val = status.state;
	}
	else if(strcmp("BSTATE",type) == 0)
	{
		val = status.bstate;
	}
	else if(strcmp("HIGHLIGHT",type) == 0)
	{
		val = status.highlight;
	}
	else if(strcmp("PREV_ITM",type) == 0)
	{
		val = status.prev_itm;
	}
	else if(strcmp("CUR_CELL",type) == 0)
	{
		val = status.current_cell;
	}
	else if(strcmp("PREV_STATE",type) == 0)
	{
		val = status.prev_state;
	}
	return val;
}

/*
 * Initialize program
 */
void init(void)
{
	find_max_items();
	set_max_pages();
	set_state("STATE",1);
	set_state("PREV_STATE",1);
	set_state("BSTATE",1);
	set_state("HIGHLIGHT",0);
	set_state("PREV_ITM",-1);
	set_scrolldex("MIN",0);
	set_scrolldex("MAX",27);
	set_scrolldex("MAX_LINE",1);
	set_recalldex("MIN",0);
	set_recalldex("MAX",27);
	set_recalldex("MAX_LINE",1);
	draw_logo();
	index_init();
	draw_state(1); 
	check_conf(); 
}


/*
 * Draw screen according to value of state
 */
void draw_state(int state)
{
	int x,y;
	getmaxyx(stdscr,y,x);
	if((y < 39) & (x < 149))
	{
		erase();
		center_error("The screen is too small to display JustPOS");
	}
	else if((y > 39) & (x > 149))
	{
		erase();
		if(state == 1)
		{
			draw_menu_buttons();
			draw_order();
			draw_order_stat();
			draw_sys_buttons();
			draw_page_status();
			draw_menu_tabs();
			draw_logo();
			if(total_items > 0)
			{
				update_order_stat();
			}
			write_list();
			write_pages();
			print_clock();
		}
		else if(state == 2)
		{
			char val[50];
			if(get_keypad_state("STATE") == 1)
			{
				strncpy(val,"CENTER",7);
			}
			else if(get_keypad_state("STATE") == 2)
			{
				strncpy(val,"RIGHT",6);
			}
			delete_keypad();
			draw_state(get_state("PREV_STATE"));
			draw_keypad(val);
			write_keypad_val();
		}
		else if(state == 3)
		{
			del_keyboard();
			draw_state(get_state("PREV_STATE"));
			draw_keyboard();
		}
		else if(state == 4)
		{
			draw_recall_win();
			draw_recall_sys_btns();
			draw_order();
			print_clock();
			draw_logo();
			write_list();
			//if(get_recalldex("STATE") == 1)
			//{
				write_recall();
			//}
		}
		else if(state == 5)
		{
			draw_order();
			draw_keypad("LEFT");
			set_keypad_state("FUNC",4);
			write_list();
			draw_logo();
			print_clock();
		}
	} 
}

/*
 * Delete screen based on value of state
 */
void del_state(int state)
{
	if(state == 1)
	{
		del_menu_buttons();
		del_sys_buttons();
		del_menu_tabs();
		del_page_status();
		del_order_stat();
		del_order_win();
	}
	
	else if(state == 4)
	{
		del_recall_win();
		del_recall_sys_btns();
		del_order_win();
	}
}


/*
 * Get maximum values from menus -- part of init
 */
void find_max_items(void)
{
	char temp[100];
	int i=0;
	FILE *food;
	food = fopen("dat/food.dat","r");
	if(!food)
	{
		center_error("dat/food.dat NOT FOUND!!! CANNOT CONTINUE");
		return;
	}
	while(!feof(food))
	{
		fgets(temp,99,food);
		i++;
	}
	i -= 2;
	itemmax.max_food = i;
	i = 0;
	fclose(food);

	FILE *drink;
	drink = fopen("dat/drinks.dat","r");
	if(!drink)
	{
		center_error("dat/drinks.dat NOT FOUND!! CANNOT CONTINUE");
		return;
	}
	while(!feof(drink))
	{
		fgets(temp,99,drink);
		i++;
	}
	i -= 2;
	itemmax.max_drink = i;
	i = 0;
	fclose(drink);
	
	FILE *mod;
	mod = fopen("dat/mods.dat","r");
	if(!mod)
	{
		center_error("dat/mods.dat NOT FOUND!! CANNOT CONTINUE");
		return;
	}
	while(!feof(mod))
	{
		fgets(temp,99,mod);
		i++;
	}
	i -= 2;
	itemmax.max_mod = i;
	i = 0;
	fclose(mod);
	
	FILE *charge;
	charge = fopen("dat/charge.dat","r");
	if(!charge)
	{
		center_error("dat/charge.dat NOT FOUND!! CANNOT CONTINUE");
		return;
	}
	while(!feof(charge))
	{
		fgets(temp,99,charge);
		i++;
	}
	i -= 2;
	itemmax.max_charge = i;
	i = 0; 
	fclose(charge);
}

/*
 * Retrieve max number of items from structure itemmax
 */
int get_item_max(char type[])
{
	int val;
	if(strcmp("FOOD",type) == 0)
	{
		val = itemmax.max_food;
	}
	else if(strcmp("DRINK",type) == 0)
	{
		val = itemmax.max_drink;
	}
	else if(strcmp("MOD",type) == 0)
	{
		val = itemmax.max_mod;
	}
	else if(strcmp("CHARGE",type) == 0)
	{
		val = itemmax.max_charge;
	}
	return val;
}


/*
 *  Retrieve current page index for specified type
 */
int get_page(char type[])
{
	int val;
	/*
	 * If food menu is visible
	 */
	if(get_state("BSTATE") == 1)
	{
		if(strcmp("PG_MAX",type) == 0)
		{
			val = (get_index("FOOD") * 12) - 1; 
		}
		else if(strcmp("PG_MIN",type) == 0)
		{
			val = 12 * (get_index("FOOD") -1);
		}
	}
	/*
	 * If drink menu is visible
	 */
	else if(get_state("BSTATE") == 2)
	{
		if(strcmp("PG_MAX",type) == 0)
		{
			val = (get_index("DRINK") * 12) - 1;
		}
		else if(strcmp("PG_MIN",type) == 0)
		{
			val = 12 * (get_index("DRINK") -1);
		}
	}
	/*
	 * If mod menu is visible
	 */
	else if(get_state("BSTATE") == 3)
	{
		if(strcmp("PG_MAX",type) == 0)
		{
			val = (get_index("MOD") * 9) - 1;
		}
		else if(strcmp("PG_MIN",type) == 0)
		{
			val = 9 * (get_index("MOD") - 1);
		}
	}
	/*
	 * If charge menu is visible
	 */
	else if(get_state("BSTATE") == 4)
	{
		if(strcmp("PG_MAX",type) == 0)
		{
			val = (get_index("CHARGE") * 9) - 1;
		}
		else if(strcmp("PG_MIN",type) == 0)
		{
			val = 9 * (get_index("CHARGE") - 1);
		}
	}
	return val;
}
