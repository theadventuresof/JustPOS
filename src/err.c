#include "../lib/justpos.h"

/*
 * This function receives a string as an argument and prints said string
 * to the third line of the main curses window. 
 * 
 * Usage is simple. Call the function and add the error message in 
 * double quotation marks. 
 * 
 * Example usage:
 * 
 * print_err("This is a system error message");
 */
void print_err(char err[])
{
	move(3,0);
	clrtoeol();
	attron(A_BOLD);
	attron(COLOR_PAIR(3));
	printw("%s",err);
	attroff(COLOR_PAIR(3));
	attroff(A_BOLD);
	doupdate();
}

/*
 * This function is almost the same as above except it produces messages
 * with blue text to represnt a non-error system message. 
 * 
 * Usage is same as above
 * 
 * print_msg("This is a non-error message");
 */
void print_msg(char msg[])
{
	move(3,0);
	clrtoeol();
	attron(A_BOLD);
	attron(COLOR_PAIR(5));
	printw("%s",msg);
	attroff(A_BOLD);
	attroff(COLOR_PAIR(5));
	doupdate();
}

/*
 * This function is again similar to those above except that it writes
 * an error message to the center of the screen. This is for when the
 * screen is too small, or the program detects an error from which it 
 * cannot recover. This function takes a string as an argument and 
 * should be used after clear() for visibility. 
 * 
 * Example usage:
 * 
 * clear();
 * center_error("This error message will appear in the center of the screen");
 */
void center_error(char msg[])
{
	int y,x;
	getmaxyx(stdscr,y,x);
	attron(A_BOLD);
	attron(COLOR_PAIR(3));
	mvwprintw(stdscr,y/2,(x/2 - (strlen(msg)/2)),"%s",msg);
	attroff(A_BOLD);
	attroff(COLOR_PAIR(3));
	doupdate();
}

WINDOW *err;
PANEL *errp;
WINDOW *ok;
PANEL *okp;

/*
 * Draw an error message window to the screen 
 */
void err_dialog(char error[])
{
	del_err_dialog();
	int y,x;
	getmaxyx(stdscr,y,x);
	set_state("PREV_STATE",get_state("STATE"));
	set_state("STATE",0);
	err = newwin(10,60,(y/2)-5,(x/2)-30);
	box(err,0,0);
	errp = new_panel(err);
	ok = newwin(3,10,(y/2)+1,(x/2)-5);	
	box(ok,0,0);
	okp = new_panel(ok);
	mvwprintw(err,4,(30-strlen(error)/2),"%s",error);
	mvwprintw(ok,1,4,"OK");
	update_panels();
	doupdate();
}

/*
 * 
 */
void del_err_dialog(void)
{
	if(err != NULL)
	{
		del_panel(errp);
		err = NULL;
	}
	if(ok != NULL)
	{
		del_panel(okp);
		ok = NULL;
	}
	set_state("STATE",get_state("PREV_STATE"));
	update_panels();
	doupdate();
}
