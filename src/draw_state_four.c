#include <panel.h>

#include "../lib/draw_state_4.h"
#include "../lib/scroll.h"

WINDOW *recall_border;
PANEL *recall_borderp;
WINDOW *recall_win;
PANEL *recall_winp;
WINDOW *date_win;
PANEL *date_winp;
WINDOW *order_tab[2];
PANEL *order_tabp[2];

/*
 * Draw the window to display order numbers
 */
void draw_recall_win(void)
{
	del_recall_win();
	date_win = newwin(3,42,3,49);
	date_winp = new_panel(date_win);
	order_tab[0] = newwin(7,10,5,40);
	//box(order_tab[0],0,0);
	order_tabp[0] = new_panel(order_tab[0]);
	order_tab[1] = newwin(7,10,12,40);
	//box(order_tab[1],0,0);
	order_tabp[1] = new_panel(order_tab[1]);
	recall_border = newwin(30,42,5,49);
	box(recall_border,0,0);
	recall_borderp = new_panel(recall_border);
	recall_win = newwin(28,40,6,50);
	recall_winp = new_panel(recall_win);
	
	mvwprintw(order_tab[0],1,4,"O");
	mvwprintw(order_tab[0],2,4,"R");
	mvwprintw(order_tab[0],3,4,"D");
	mvwprintw(order_tab[0],4,4,"E");
	mvwprintw(order_tab[0],5,4,"R");
	mvwprintw(order_tab[0],6,4,"S");
	
	mvwprintw(order_tab[1],1,4,"V");
	mvwprintw(order_tab[1],2,4,"O");
	mvwprintw(order_tab[1],3,4,"I");
	mvwprintw(order_tab[1],4,4,"D");
	mvwprintw(order_tab[1],5,4,"S");
	
	update_panels();
	doupdate();
}

/*
 * Set recall_win to NULL 
 */
void del_recall_win(void)
{
	if(recall_win != NULL)
	{
		del_panel(recall_winp);
		recall_win = NULL;
	}
	if(recall_border != NULL)
	{
		del_panel(recall_borderp);
		recall_border = NULL;
	}
	if(date_win != NULL)
	{
		del_panel(date_winp);
		date_win = NULL;
	}
	for(int i =0; i < 2; i++)
	{
		if(order_tab[i] != NULL)
		{
			del_panel(order_tabp[i]);
			order_tab[i] = NULL;
		}
	}
}

/*
 * 
 */
void highlight_order_tabs(void)
{
	if(get_recalldex("STATE") <= 4)
	{
		wbkgd(order_tab[0],COLOR_PAIR(1));
		wbkgd(order_tab[1],COLOR_PAIR(2));
	}
	else if(get_recalldex("STATE") == 5)
	{
		wbkgd(order_tab[0],COLOR_PAIR(2));
		wbkgd(order_tab[1],COLOR_PAIR(1));
	}
	update_panels();
	doupdate();
}

/*
 * Copy blanks to every position in recall_win
 */
void clear_recall_win(void)
{
	werase(recall_win);
}

WINDOW *recall_ord;
PANEL *recall_ordp;
WINDOW *search_recall;
PANEL *search_recallp;
WINDOW *search_date;
PANEL *search_datep;
WINDOW *print_order;
PANEL *print_orderp;
WINDOW *void_ord;
PANEL *void_ordp;

/*
 * Draw function buttons displayed on the recall menu
 */
void draw_recall_sys_btns(void)
{
	del_recall_sys_btns();
	recall_ord = newwin(3,20,12,10);
	box(recall_ord,0,0);
	mvwprintw(recall_ord,1,4,"RECALL/SELECT");
	recall_ordp = new_panel(recall_ord);
	search_recall = newwin(3,20,16,10);
	box(search_recall,0,0);
	mvwprintw(search_recall,1,4,"SEARCH ORDER");
	search_recallp = new_panel(search_recall);
	search_date = newwin(3,20,20,10);
	box(search_date,0,0);
	mvwprintw(search_date,1,4,"CHANGE DATE");
	search_datep = new_panel(search_date);
	print_order = newwin(3,20,24,10);
	box(print_order,0,0);
	mvwprintw(print_order,1,5,"PRINT ORDER");
	print_orderp = new_panel(print_order);
	void_ord = newwin(3,20,28,10);
	box(void_ord,0,0);
	mvwprintw(void_ord,1,5,"VOID ORDER");
	void_ordp = new_panel(void_ord);
	
	
	update_panels();
	doupdate();
}

/*
 * Set recall system buttons to NULL
 */
void del_recall_sys_btns(void)
{
	if(recall_ord != NULL)
	{
		del_panel(recall_ordp);
		recall_ord = NULL;
	}
	if(void_ord != NULL)
	{
		del_panel(void_ordp);
		void_ord = NULL;
	}
	if(print_order != NULL)
	{
		del_panel(print_orderp);
		print_order = NULL;
	}
	if(search_date != NULL)
	{
		del_panel(search_datep);
		search_date = NULL;	
	}
	if(search_recall != NULL)
	{
		del_panel(search_recallp);
		search_recall = NULL;
	}
}

/*
 * Write line to recall_win
 */
void write_to_recall(char line[],int format)
{
	int y,x;
	getyx(recall_win,y,x);
	/*
	 * Always start on line one, and column three
	 */
	if(y == 0)
	{
		y = 1;
	}
	if(x == 0)
	{
		x = 3;
	}
	wmove(recall_win,y,x);
	/*
	 * If format = 2, highlight it
	 */
	if(format == 2)
	{
		wattron(recall_win,COLOR_PAIR(1));
	}
	/*
	 * Augment current line of recall_win being written to by one
	 */
	set_recalldex("CURRENT",1);
	/*
	 * Check if current line is visible on recall_win
	 */
	 
	wattron(recall_win,A_BOLD);
	if((get_recalldex("CURRENT") > get_recalldex("MIN")) & (get_recalldex("CURRENT") <= get_recalldex("MAX")-1))
	{
		/*
		 * Write line in bold text to recall_win
		 */
		wprintw(recall_win,"%s",line);
		wprintw(recall_win,"\n");
	}
	wattroff(recall_win,A_BOLD);
	wattroff(recall_win,COLOR_PAIR(1));
	/*
	 * Show update
	 */
	update_panels();
	doupdate();
}

/*
 * 
 */
void write_recall_date(char date[])
{
	werase(date_win);
	mvwprintw(date_win,1,1,"Viewing: %s",date);
	update_panels();
	doupdate();
}
