#include <panel.h>
#include <string.h>
#include <stdio.h>

#include "../lib/err.h"
#include "../lib/order.h"
#include "../lib/state.h"
#include "../lib/draw.h"
#include "../lib/item.h"
#include "../lib/scroll.h"
#include "../lib/justpos.h"

/*
 * Keep track of current menu status
 */
struct mydex{
	int food_menu_page;
	int drink_menu_page;
	int mod_menu_page;
	int charge_menu_page;
}mydex;

/*
 * Keep track of maximum menu page status
 */
struct maxdex{
	int max_food_page;
	int max_drink_page;
	int max_mod_page;
	int max_charge_page;
}maxdex;

/*
 * Define some curses "windows" and use panels to keep track of them
 */
WINDOW *itm_btns[14];
PANEL *itm_btnsp[14];


/*
 * Draw 12 buttons to hold menu items
 */
void draw_menu_buttons(void)
{
	del_menu_buttons();
	int j,starty,startx;
	for(j=0;j<12;j++)
	{
		if(j<=2)
		{
			starty = 5;
			startx = (30 * j) + 9;
			itm_btns[j] = newwin(4,30,starty,startx);
			box(itm_btns[j],' ',' ');
			itm_btnsp[j] = new_panel(itm_btns[j]);
			
		}
		else if(j<=5)
		{
			starty = 10;
			startx = (30 * (j - 3)) + 9;
			itm_btns[j] = newwin(4,30,starty,startx);
			box(itm_btns[j],' ',' ');
			itm_btnsp[j] = new_panel(itm_btns[j]);
		}
		else if(j<=8)
		{
			starty = 15;
			startx = (30 * (j - 6)) + 9;
			itm_btns[j] = newwin(4,30,starty,startx);
			box(itm_btns[j],' ',' ');
			itm_btnsp[j] = new_panel(itm_btns[j]);
		}
		else if(j<=12)
		{
			starty = 20;
			startx = (30 * (j - 9)) + 9;
			itm_btns[j] = newwin(4,30,starty,startx);
			box(itm_btns[j],' ',' ');
			itm_btnsp[j] = new_panel(itm_btns[j]);
		}
	}
	write_to_menu_buttons();
	update_panels();
	doupdate();
}

/*
 * Write items from appropriate menu to itm_btns
 */
void write_to_menu_buttons(void)
{
	clear_menu_buttons();
	char menu[50];
	int type;
	/*
	 * If food menu is visible
	 */
	if(get_state("BSTATE") == 1)
	{
		strncpy(menu,"FOOD",5);
		type = 1;
	}
	/*
	 * If drink menu is visible
	 */
	else if(get_state("BSTATE") == 2)
	{
		strncpy(menu,"DRINK",6);
		type = 2;
	}
	/*
	 * If mod menu is visible
	 */
	else if(get_state("BSTATE") == 3)
	{
		strncpy(menu,"MOD",4);
		type = 3;
	}
	/*
	 * If extra charge menu is visible
	 */
	else if(get_state("BSTATE") == 4)
	{
		strncpy(menu,"CHARGE",7);
		type = 4;
	}
	for(int i = 0; i < 12; i++)
	{
		if(get_state("BSTATE") == 1)
		{
			wattron(itm_btns[i],COLOR_PAIR(4));
		}
		else if(get_state("BSTATE") == 2)
		{
			wattron(itm_btns[i],COLOR_PAIR(11));
		}
		else if(get_state("BSTATE") == 3)
		{	
			wattron(itm_btns[i],COLOR_PAIR(6));
			wattron(itm_btns[11],COLOR_PAIR(2));
		}
		else if(get_state("BSTATE") == 4) 
		{
			wattron(itm_btns[i],COLOR_PAIR(3));
		}
		int position = get_page("PG_MIN") + i + 1;
		if((position <= get_item_max()) & (i <= get_page("PG_MAX")) & (position >= get_page("PG_MIN")))
		{
			char name[100];
			get_name(type,name,position);
			int len = (30 - (strlen(name)))/2;
			mvwprintw(itm_btns[i],1,len,"%s",name);
			if(get_state("BSTATE") != 3)
			{
				mvwprintw(itm_btns[i],2,12,"%.2f",get_itm(type,"COST",position));
			}
		}
		if(get_state("BSTATE") == 3)
		{
			wattron(itm_btns[9],A_BOLD);
			mvwprintw(itm_btns[9],2,10,"CLEAR MODS");
			wattron(itm_btns[11],A_BOLD);
			mvwprintw(itm_btns[11],2,11,"KEYBOARD");
			wattroff(itm_btns[9],A_BOLD);
			wattroff(itm_btns[11],A_BOLD);
		}
		wattroff(itm_btns[i],COLOR_PAIR(4));
		wattroff(itm_btns[i],COLOR_PAIR(11));
		wattroff(itm_btns[i],COLOR_PAIR(6));
		wattroff(itm_btns[i],COLOR_PAIR(3));
	}
	update_panels();
	doupdate();
}

/*
 * Delete menu buttons
 */
void del_menu_buttons(void)
{
	for(int i=0; i < 12; i++)
	{
		if(itm_btns[i] != NULL)
		{
			del_panel(itm_btnsp[i]);
			itm_btns[i] = NULL;
		}
	}
	update_panels();
	doupdate();
}

WINDOW *order_stat;
PANEL *order_statp;

/*
 * Draw order statistics window to screen
 */
void draw_order_stat(void)
{
	del_order_stat();
	order_stat = newwin(5,35,34,107);
	order_statp = new_panel(order_stat);
}

/*
 * Delete order statistics window from memory
 */
void del_order_stat(void)
{
	if(order_stat != NULL)
	{
		del_panel(order_statp);
		order_stat = NULL;
	}
}

WINDOW *order_win;
PANEL *order_winp;
/*
 * Testing a new order display
 */
void draw_order(void)
{
	del_order_win();
	order_win = newwin(28,40,5,105);
	order_winp = new_panel(order_win);
} 

/*
 * Clear order window
 */
void clear_order_win(void)
{
	werase(order_win);
}

/*
 * Delete order window from memory.
 */
void del_order_win(void)
{
	if(order_win != NULL)
	{
		del_panel(order_winp);
		//delwin(order_win);
		order_win = NULL;
	}
}



/*
 * Clear all menu buttons 
 */
void clear_menu_buttons(void)
{
	for(int i=0; i < 12; i++)
	{
		wbkgd(itm_btns[i],COLOR_PAIR(2));
		werase(itm_btns[i]);
		box(itm_btns[i],' ',' ');
	}
	update_panels();
	doupdate();
}

/*
 * 
 */
void select_menu_button(int button)
{
	wbkgd(itm_btns[button-1],COLOR_PAIR(1));
	update_panels();
	doupdate();
}

/*
 * Set all pages to 1 part of init
 */
void index_init(void)
{
	mydex.food_menu_page = 1;
	mydex.drink_menu_page = 1;
	mydex.mod_menu_page = 1;
	mydex.charge_menu_page = 1;
}

/*
 * Change page index	
 */
void set_index(char type[],int val)
{
	if(strcmp(type,"FOOD") == 0)
	{
		mydex.food_menu_page = val;
	}
	else if(strcmp(type,"DRINK") == 0)
	{
		mydex.drink_menu_page = val;
	}
	else if(strcmp(type,"MOD") == 0)
	{
		mydex.mod_menu_page = val;
	}
	else if(strcmp(type,"CHARGE") == 0)
	{
		mydex.charge_menu_page = val;
	}
}


/*
 * Retrieve page index
 */
int get_index(char type[])
{
	int val=0;
	if(strcmp(type,"FOOD") == 0)
	{
		val = mydex.food_menu_page;
	}
	else if(strcmp(type,"DRINK") == 0)
	{
		val = mydex.drink_menu_page;
	}
	else if(strcmp(type,"MOD") == 0)
	{
		val = mydex.mod_menu_page;
	}
	else if(strcmp(type,"CHARGE") == 0)
	{
		val = mydex.charge_menu_page;
	}
	return val;
}

/*
 * Print string passed here to order_win 
 */
void write_to_order_win(char details[],int format,int highlight)
{
	/*
	 * If item is selected, use highlight format
	 */
	if(highlight == 1)
	{
		wattron(order_win,COLOR_PAIR(1));
	}
	/*
	 * Write item names in bold
	 */
	if(format == 1)
	{
		wattron(order_win,A_BOLD);
	}
	else if(format == 2)
	{
	
	}
	/*
	 * Set cursor position in order_win to start on first line and third column
	 */
	int y,x;
	getyx(order_win,y,x);
	if(y < 1)
	{
		y = 1;
		wmove(order_win,y,x);
	}
	if(x == 0)
	{
		x = 3;
		wmove(order_win,y,x);
	}
	set_scrolldex("CURRENT",1);
	if((get_scrolldex("CURRENT") >= get_scrolldex("MIN")) & (get_scrolldex("CURRENT") < get_scrolldex("MAX")))
	{
		wprintw(order_win,"%s",details);
		wprintw(order_win,"\n");
	}
	wattroff(order_win,COLOR_PAIR(1));
	wattroff(order_win,A_BOLD);
	update_panels();
	doupdate();
}

WINDOW *sys_btns[11];
PANEL *sys_btnsp[11];

/*
 * Draw 12 sys_btns to screen
 */
void draw_sys_buttons(void)
{
	del_sys_buttons();
	int i,startx,starty;
	for(i = 0; i < 12; i++)
	{
		if(i <= 3)
		{
			starty = 27;
			startx = 25 * i;
			sys_btns[i] = newwin(3,20,starty,startx);
			box(sys_btns[i],0,0);
			sys_btnsp[i] = new_panel(sys_btns[i]);
		}
		else if(i <= 7)
		{
			starty = 30;
			startx = 25 * (i - 4);
			sys_btns[i] = newwin(3,20,starty,startx);
			box(sys_btns[i],0,0);
			sys_btnsp[i] = new_panel(sys_btns[i]);
		}
		else if(i <= 11)
		{
			starty = 33;
			startx = 25 * (i - 8);
			sys_btns[i] = newwin(3,20,starty,startx);
			box(sys_btns[i],0,0);
			sys_btnsp[i] = new_panel(sys_btns[i]);
		}
	}
	
	mvwprintw(sys_btns[0],1,5,"CLEAR ORDER");
	mvwprintw(sys_btns[1],1,7,"DEL ITM");
	mvwprintw(sys_btns[2],1,7,"ADJ QTY");
	mvwprintw(sys_btns[3],1,6,"OPEN FOOD");
	mvwprintw(sys_btns[4],1,7,"RECALL");
	mvwprintw(sys_btns[5],1,7,"SEARCH");
	
	mvwprintw(sys_btns[8],1,6,"SETTINGS");
	mvwprintw(sys_btns[9],1,7,"MANAGER");
	mvwprintw(sys_btns[10],1,6,"PAY CARD");
	mvwprintw(sys_btns[11],1,6,"PAY CASH");
	
	update_panels();
	doupdate();
}

/*
 * Delete sys_btns panels/windows
 */
void del_sys_buttons(void)
{
	for(int i = 0; i < 12; i++)
	{
		if(sys_btns[i] != NULL)
		{
			del_panel(sys_btnsp[i]);
			//delwin(sys_btns[i]);
			sys_btns[i] = NULL;
		}
	}
}

/*
 * Clear all sys_btns and re-draw borders
 */
void clear_sys_buttons(void)
{
	for(int i = 0; i < 12; i++)
	{
		werase(sys_btns[i]);
		box(sys_btns[i],0,0);
	}
}

/*
 * Set maximum order pages according to number of entries and menu buttons
 * occupied
 */
void set_max_pages(void)
{
	double decimal_value;
	int int_value,num_value;
	
	set_state("BSTATE",1);
	num_value = get_item_max();
	decimal_value = num_value/11.00;
	int_value = (int)decimal_value;
	if(decimal_value > int_value)
	{
		maxdex.max_food_page = int_value + 1;
	}
	else if(decimal_value == int_value)
	{
		maxdex.max_food_page = decimal_value;
	}
	else{
		maxdex.max_food_page = 1;
	}
	
	set_state("BSTATE",2);
	num_value = get_item_max();
	decimal_value = num_value/11.00;
	int_value = (int)decimal_value;
	if(decimal_value  > int_value)
	{
		maxdex.max_drink_page = int_value + 1;
	}
	else if(decimal_value == int_value)
	{
		maxdex.max_drink_page = decimal_value;
	}
	else{
		maxdex.max_drink_page = 1;
	}
	
	set_state("BSTATE",3);
	num_value = get_item_max();
	decimal_value = num_value/9.00;
	int_value = (int)decimal_value;
	if(decimal_value > int_value)
	{
		maxdex.max_mod_page = int_value + 1;
	}
	else if(decimal_value == int_value)
	{
		maxdex.max_mod_page = decimal_value;
	}
	else{
		maxdex.max_drink_page = 1;
	}
	
	set_state("BSTATE",4);
	num_value = get_item_max();
	decimal_value = num_value/9.00;
	int_value = (int)decimal_value;
	if(decimal_value > int_value)
	{
		maxdex.max_charge_page = int_value + 1;
	}
	else if(decimal_value == int_value) 
	{
		maxdex.max_charge_page = decimal_value;
	}
	else{
		maxdex.max_charge_page = 1;
	}
	set_state("BSTATE",1);
}

/*
 * Find maximum order pages 
 */
int get_max_pages(char type[])
{
	int val;
	if(strncmp(type,"FOOD",4) == 0)
	{
		val = maxdex.max_food_page;
	}
	else if(strncmp(type,"DRINK",5) == 0)
	{
		val = maxdex.max_drink_page;
	}
	else if(strncmp(type,"MOD",3) == 0)
	{
		val = maxdex.max_mod_page;
	}
	else if(strncmp(type,"CHARGE",6) == 0)
	{
		val = maxdex.max_charge_page;
	}
	return val;
}

WINDOW *pg[2];
PANEL *pgp[2];

/*
 * Draw page change buttons
 */
void draw_page_status(void)
{
	del_page_status();
	
	pg[0] = newwin(3,6,24,37);
	pg[1] = newwin(3,6,24,66);
	
	mvwprintw(pg[0],1,2,"<-");
	mvwprintw(pg[1],1,2,"->");
		
	for(int i = 0; i < 2; i++)
	{
		pgp[i] = new_panel(pg[i]);
		box(pg[i],' ',' ');
	}
	update_panels();
	doupdate();
}

/*
 * Delete page change buttons
 */
void del_page_status(void)
{
	for(int i = 0; i <= 1; i++)
	{
		if(pg[i] != NULL)
		{
			del_panel(pgp[i]);
			//delwin(pg[i]);
			pg[i] = NULL;
		}
	}
}

/*
 * Copy blank spaces to stdscr from startx to endx
 */
void clear_until(int starty,int startx,int endx)
{
	move(starty,startx);
	int diff = endx - startx;
	for(int i = 0; i < diff; i++)
	{
		printw(" ");
	}
}

/*
 * Concat blank spaces to a given destination string. 
 * 
 * Used when writing to order_win to highlight entire line
 */
void concat_blanks(int num,char *dest)
{
	for(int i = 0; i < num; i++)
	{
		strncat(dest," ",2);
	}
} 

/*
 * Write current menu page information
 */
void write_pages(void)
{
	clear_until(25,37,66);
	move(25,52);
	int cur_page,max_page;
	if(get_state("BSTATE") == 1)
	{
		cur_page = get_index("FOOD");
		max_page = get_max_pages("FOOD");
	}
	else if(get_state("BSTATE") == 2)
	{
		cur_page = get_index("DRINK");
		max_page = get_max_pages("DRINK");
	}
	else if(get_state("BSTATE") == 3)
	{
		cur_page = get_index("MOD");
		max_page = get_max_pages("MOD");
	}
	else if(get_state("BSTATE") == 4)
	{
		cur_page = get_index("CHARGE");
		max_page = get_max_pages("CHARGE");
	}
	printw("%d/%d",cur_page,max_page);
	
	update_panels();
	doupdate();
}

/*
 * Define some windows/panels for the menu tabs
 */
WINDOW *menu_tab[4];
PANEL *menu_tabp[4];

/*
 * Draw menu tabs, write to them, and give them color
 */
void draw_menu_tabs(void)
{
	del_menu_tabs();
	
	menu_tab[3] = newwin(7,8,20,1);
	wbkgd(menu_tab[3],COLOR_PAIR(7));
	menu_tabp[3] = new_panel(menu_tab[3]);
	mvwprintw(menu_tab[3],1,3,"C");
	mvwprintw(menu_tab[3],2,3,"H");
	mvwprintw(menu_tab[3],3,3,"R");
	mvwprintw(menu_tab[3],4,3,"G");
	mvwprintw(menu_tab[3],5,3,"E");
		
	menu_tab[2] = newwin(7,8,15,1);
	wbkgd(menu_tab[2],COLOR_PAIR(10));
	menu_tabp[2] = new_panel(menu_tab[2]);
	mvwprintw(menu_tab[2],1,3,"M");
	mvwprintw(menu_tab[2],2,3,"O");
	mvwprintw(menu_tab[2],3,3,"D");
	mvwprintw(menu_tab[2],4,3,"S");
	
	menu_tab[1] = newwin(7,8,10,1);
	wbkgd(menu_tab[1],COLOR_PAIR(9));
	menu_tabp[1] = new_panel(menu_tab[1]);
	mvwprintw(menu_tab[1],1,3,"D");
	mvwprintw(menu_tab[1],2,3,"R");
	mvwprintw(menu_tab[1],3,3,"I");
	mvwprintw(menu_tab[1],4,3,"N");
	mvwprintw(menu_tab[1],5,3,"K");

	menu_tab[0] = newwin(7,8,5,1);
	wbkgd(menu_tab[0],COLOR_PAIR(8));
	menu_tabp[0] = new_panel(menu_tab[0]);
	mvwprintw(menu_tab[0],1,3,"F");
	mvwprintw(menu_tab[0],2,3,"O");
	mvwprintw(menu_tab[0],3,3,"O");
	mvwprintw(menu_tab[0],4,3,"D");
	
	shuffle_tabs();
	
	update_panels();
	doupdate();
}


/*
 * Delete panels and windows related to menu tabs
 */
void del_menu_tabs(void)
{
	for(int i = 0; i < 4; i++)
	{
		if(menu_tab[i] != NULL)
		{
			del_panel(menu_tabp[i]);
			//delwin(menu_tab[i]);
			menu_tab[i] = NULL;
		}
	}
}

/*
 * Change which menu tab is on top according to BSTATE
 */
void shuffle_tabs(void)
{
	if(get_state("BSTATE") == 1)
	{
		top_panel(menu_tabp[3]);
		top_panel(menu_tabp[2]);
		top_panel(menu_tabp[1]);
		top_panel(menu_tabp[0]);
	}
	else if(get_state("BSTATE") == 2)
	{
		top_panel(menu_tabp[0]);
		top_panel(menu_tabp[3]);
		top_panel(menu_tabp[2]);
		top_panel(menu_tabp[1]);
	}
	else if(get_state("BSTATE") == 3)
	{
		top_panel(menu_tabp[3]);
		top_panel(menu_tabp[0]);
		top_panel(menu_tabp[1]);
		top_panel(menu_tabp[2]);
	}
	else if(get_state("BSTATE") == 4)
	{
		top_panel(menu_tabp[0]);
		top_panel(menu_tabp[1]);
		top_panel(menu_tabp[2]);
		top_panel(menu_tabp[3]);
	}
	
	update_panels();
	doupdate();
}

/*
 * Draw a logo to the screen
 */
void draw_logo(void)
{
	attron(COLOR_PAIR(3));
	attron(A_BOLD);
	mvwprintw(stdscr,36,25,"      _            _   _____   ____   _____");
	mvwprintw(stdscr,37,25,"     | |          | | |  __ \\ / __ \\ / ____|     ");
	attroff(COLOR_PAIR(3));
	attron(COLOR_PAIR(2));
	mvwprintw(stdscr,38,25,"     | |_   _  ___| |_| |__) | |  | | (___");
	mvwprintw(stdscr,39,25," _   | | | | |/ __| __|  ___/| |  | |\\___ \\");
	attroff(COLOR_PAIR(2));
	attron(COLOR_PAIR(5));
	mvwprintw(stdscr,40,25,"| |__| | |_| |\\__ \\ |_| |    | |__| |____) |");
	mvwprintw(stdscr,41,25," \\____/\\ __,_|____/\\__|_|     \\____/|_____/");
	attroff(A_BOLD);
	attroff(COLOR_PAIR(5));
	mvwprintw(stdscr,42,25,"                                    %s",VERSION);
} 

/*
 * Write statistics about current order
 */
void update_order_stat(void)
{
	clear_order_stat();
	wattron(order_stat,A_BOLD);
	mvwprintw(order_stat,1,1,"Total items:\t\t%d",total_items());
	mvwprintw(order_stat,3,1,"Total price:\t\t%.2f",calc_total());
	wattroff(order_stat,A_BOLD);
	update_panels();
	doupdate();
}

/*
 * Completely clear order statistics window
 */
void clear_order_stat(void)
{
	werase(order_stat);
}

/*
 * 
 */
void print_months(void)
{
	clear_menu_buttons();
	mvwprintw(itm_btns[0],1,13,"Jan");
	mvwprintw(itm_btns[1],1,13,"Feb");
	mvwprintw(itm_btns[2],1,13,"Mar");
	mvwprintw(itm_btns[3],1,13,"Apr");
	mvwprintw(itm_btns[4],1,13,"May");
	mvwprintw(itm_btns[5],1,13,"Jun");
	mvwprintw(itm_btns[6],1,13,"Jul");
	mvwprintw(itm_btns[7],1,13,"Aug");
	mvwprintw(itm_btns[8],1,13,"Sep");
	mvwprintw(itm_btns[9],1,13,"Oct");
	mvwprintw(itm_btns[10],1,13,"Nov");
	mvwprintw(itm_btns[11],1,13,"Dec");
	
	update_panels();
	doupdate();
}
