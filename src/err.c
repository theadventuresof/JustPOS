#include <panel.h>
#include <stdio.h>
#include <string.h>

#include "../lib/order.h"
#include "../lib/state.h"
#include "../lib/draw.h"
#include "../lib/err.h"

/*
 * Draws red error text over center of screen -- used for screen size check
 */
void center_error(char msg[])
{
	/*
	 * Declare to integers and use them to get the max y and x axis of
	 * stdscr
	 */
	int y,x;
	getmaxyx(stdscr,y,x);
	/*
	 * Embolden text and make it red
	 */
	attron(A_BOLD);
	attron(COLOR_PAIR(3));
	/*
	 * Use y and x to find the center of the screen, subtracting half of 
	 * the message length
	 */
	mvwprintw(stdscr,y/2,(x/2 - (strlen(msg)/2)),"%s",msg);
	/*
	 * Turn off attributes and update screen
	 */
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
	/*
	 * Get maximum y and x axis of stdscr
	 */
	int y,x;
	getmaxyx(stdscr,y,x);
	/*
	 * Change PREV_STATE to current STATE and set STATE to 0
	 */
	set_state("PREV_STATE",get_state("STATE"));
	set_state("STATE",0);
	/*
	 * Draw error box and ok box and put a border on each
	 */
	err = newwin(10,60,(y/2)-5,(x/2)-30);
	box(err,0,0);
	errp = new_panel(err);
	ok = newwin(3,10,(y/2)+1,(x/2)-5);	
	box(ok,0,0);
	okp = new_panel(ok);
	/*
	 * Find center of error window and draw error message
	 */
	mvwprintw(err,4,(30-strlen(error)/2),"%s",error);
	mvwprintw(ok,1,4,"OK");
	update_panels();
	doupdate();
}

/*
 * Draw change due dialog to screen
 */
void change_dialog(float change)
{
	/*
	 * Delete payment screen and draw main screen
	 */
	del_state(5);
	draw_state(1);
	del_err_dialog();
	del_order();
	/*
	 * Get maximum y and x axis from stdscr
	 */
	int y,x;
	getmaxyx(stdscr,y,x);
	/*
	 * State 1 is already drawn, so we set PREV_STATE to 1 so that when
	 * the error dialog is deleted, state 1 mouse is catpured 
	 */
	set_state("PREV_STATE",1);
	set_state("BSTATE",1);
	shuffle_tabs();
	write_to_menu_buttons();
	/*
	 * err_dialog window is visible so STATE is 0 
	 */
	set_state("STATE",0);
	/*
	 * Draw error dialog and ok windows
	 */
	err = newwin(10,60,(y/2)-5,(x/2)-30);
	box(err,0,0);
	errp = new_panel(err);
	ok = newwin(3,10,(y/2)+1,(x/2)-5);
	box(ok,0,0);
	okp = new_panel(ok);
	/*
	 * Write change due amount to error dialog window
	 */
	mvwprintw(err,3,25,"CHANGE DUE");
	char change_due[50];
	sprintf(change_due,"$%.2f",change);
	mvwprintw(err,4,30-(strlen(change_due)/2),"%s",change_due);
	mvwprintw(ok,1,4,"OK");
	update_panels();
	doupdate();
}

/*
 * Delete error dialog window and OK window from memory and reset 
 * STATE to PREV_STATE
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

WINDOW *yes_no_window;
PANEL *yes_no_windowp;

WINDOW *yes;
PANEL *yesp;

WINDOW *no;
PANEL *nop;

/*
 * Draw yes/no dialog window(s)
 */
void yes_no_dialog(char msg[],int option)
{
	del_yes_no_dialog();
	/*
	 * Get screen size
	 */
	int y,x;
	getmaxyx(stdscr,y,x);
	/*
	 * Change current state to previous state and set current state to -1
	 */
	set_state("PREV_STATE",get_state("STATE"));
	set_state("STATE",-1);
	/*
	 * Draw yes_no_window
	 */
	yes_no_window = newwin(10,60,(y/2)-5,(x/2)-30);
	box(yes_no_window,0,0);
	yes_no_windowp = new_panel(yes_no_window);
	/*
	 * Draw yes window
	 */
	yes = derwin(yes_no_window,3,11,6,10);
	box(yes,0,0);
	yesp = new_panel(yes);
	mvwprintw(yes,1,4,"YES");
	/*
	 * Draw no window
	 */
	no = derwin(yes_no_window,3,11,6,40);
	box(no,0,0);
	nop = new_panel(no);
	mvwprintw(no,1,5,"NO");
	/*
	 * Print message to yes_no_window
	 */
	mvwprintw(yes_no_window,4,30-strlen(msg)/2,"%s",msg);
	/*
	 * Show updates
	 */
	update_panels();
	doupdate();
}

/*
 * Delete yes_no_window(s) from memory
 */
void del_yes_no_dialog(void)
{
	if(yes_no_window != NULL)
	{
		del_panel(yes_no_windowp);
		yes_no_window = NULL;
	}
	if(yes != NULL)
	{
		del_panel(yesp);
		yes = NULL;
	}
	if(no != NULL)
	{
		del_panel(nop);
		no = NULL;
	}
	set_state("STATE",get_state("PREV_STATE"));
	update_panels();
	doupdate();
}
