#include "../lib/justpos.h"

WINDOW *recall_win;
PANEL *recall_winp;

/*
 * Draw the window to display order numbers
 */
void draw_recall_win(void)
{
	del_recall_win();
	recall_win = newwin(28,40,5,50);
	recall_winp = new_panel(recall_win);
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
}

/*
 * Copy blanks to every position in recall_win
 */
void clear_recall_win(void)
{
	werase(recall_win);
}

WINDOW *recall_back;
PANEL *recall_backp;
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
	recall_back = newwin(3,15,38,5);
	box(recall_back,0,0);
	mvwprintw(recall_back,1,5,"RETURN");
	recall_backp = new_panel(recall_back);
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
	if(recall_back != NULL)
	{
		del_panel(recall_backp);
		recall_back = NULL;
	}
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
