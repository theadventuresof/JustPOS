#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/err.h"
#include "../lib/order.h"
#include "../lib/scroll.h"
#include "../lib/state.h"
#include "../lib/recall.h"
#include "../lib/keypad.h"
#include "../lib/print.h"
#include "../lib/draw.h"

/*
 * The main entry point for mouse/touch input
 */
void get_mouse(int y,int x)
{
	/*
	 * If yes/no dialog is visible
	 */
	if(get_state("STATE") == -1)
	{
		find_mouse_yes_no(y,x);
		return;
	}
	/*
	 * If error dialog is visible
	 */
	if(get_state("STATE") == 0)
	{
		int maxy,maxx;
		getmaxyx(stdscr,maxy,maxx);
		maxy = (maxy/2) + 1;
		maxx = (maxx/2) - 5;
		/*
		 * If OK is pressed
		 */
		if((y >= maxy) & (y <= maxy + 2) & (x >= maxx) & (x <= maxx + 9))
		{
			del_err_dialog();
			return;
		}
	}
	/*
	 * If main menu is visible
	 */
	if(get_state("STATE") == 1)
	{
		/*
		 * If mouse event occured on menu buttons
		 */
		if((y >= 5) & (y <= 23) & (x >= 9) & (x <= 98))
		{
			if(get_state("CUR_CELL") >= 0)
			{
				reset_menu_button();
			}
			find_mouse_menu_buttons(y,x);
		}
		/*
		 * If mouse event occured on menu page adjust buttons
		 */
		if((y >= 24) & (y <= 27) & (x >= 37) & (x <= 71))
		{
			find_mouse_menu_page_adjust(y,x);
		}
		/*
		 * If mouse event occured on system buttons
		 */
		if((y >= 28) & (y <= 40) & (x <= 94))
		{
			find_mouse_system_buttons(y,x);
		}
		/*
		 * If mouse event occured on order window
		 */
		if((y >= 6) & (y <= 32) & (x >= 105) & (x <= 145))
		{
			if((y == get_scrolldex("TOUCH")) & (find_item_lines(y) >= 0))
			{
				highlight(find_item_lines(y));
			}
		}
		/* 
		 * If mouse event occurred on menu tabs
		 */
		if((y >= 5) & (y <= 27) & (x <= 8))
		{
			find_mouse_menu_tabs(y,x);
		}
	}
	/*
	 * If keypad is visible
	 */
	else if(get_state("STATE") == 2)
	{
		int maxy,maxx;
		getmaxyx(stdscr,maxy,maxx);
		maxy = (maxy/2) - 15;
		if((y >= maxy) & (y <= maxy + 30) & (x < maxx))
		{
			find_mouse_keypad(y,x);
			return;
		}
	}
	/*
	 * If keyboard is visible
	 */
	else if(get_state("STATE") == 3)
	{
		int maxy,maxx;
		getmaxyx(stdscr,maxy,maxx);
		if((y >= maxy/2) & (y <= maxy) & (x < maxx))
		{
			find_mouse_keyboard(y,x);	
		}
	}
	/*
	 * If recall menu is visible
	 */
	else if(get_state("STATE") == 4)
	{
		
		int line = find_reacall_lines(y);
		/*
		 * If mouse event occured on recall system buttons
		 */
		if((y >= 6) & (y <= 40) & (x >= 5) & (x <= 30))
		{
			find_mouse_recall_sys(y,x);
		}
		/*
		 * If mouse event occured on recall-win -- if an order/date is to be selected
		 */
		else if((y >= 5) & (y <= 35) & (x >= 51) & (x <= 86))
		{
			if((y == get_scrolldex("TOUCH")) & (line >= 0))
			{
				/*
				 * If touching a highlighted line, un-highlight it and return
				 */
				if(line == get_recalldex("LINE"))
				{
					set_recalldex("LINE",-1);
					write_recall();
					return;
				}
				/*
				 * Set selected line to be highlighted 
				 */
				set_recalldex("LINE",line);
				write_recall();
			}
		}
		/*
		 * If orders tab is pressed
		 */
		else if((y >= 5) & (y <= 11) & (x >= 41) & (x <= 48))
		{
			view_recall_orders();
		}
		/*
		 * If voids tab is pressed
		 */
		else if((y >= 13) & (y <= 18) & (x >= 41) & (x <= 48))
		{
			view_recall_voids();
		}
	}
	else if(get_state("STATE") == 5)
	{
		/*
		 * If mouse event occurs on keypad 
		 */
		if((y >= 6) & (y <= 35) & (x >= 5) & (x <= 67))
		{
			find_mouse_payment_keypad(y,x);
		}
		else if((y >= 38) & (y <= 40) & (x >= 6) & (x <= 20))
		{
			delete_keypad();
			update_panels();
			doupdate();
			del_state(5);
			erase();
			set_state("STATE",1);
			set_state("PREV_STATE",1);
			draw_state(1);
		}
		else if((y >= 7) & (y <= 35) & (x >= 74) & (x <= 89))
		{
			find_mouse_cash_buttons(y,x);
		}
	}
	/*
	 * 
	 */
	else if(get_state("STATE") == 6)
	{
		/*
		 * If mouse event occured on printer_win
		 */
		if((y >= 16) & (y <= 20) & (x >= 90) & (x <= 138))
		{
			int line = (y + get_printerdex("MIN")) - 17;
			if(get_state("WHICH_PRINTER") == 0)
			{
				err_dialog("SELECT A PRINTER TO CONFIGURE");
				return;
			}
			if((y == get_scrolldex("TOUCH")) & (line >= 0) & (get_state("WHICH_PRINTER") > 0))
			{
				if(line == get_printerdex("LINE"))
				{
					set_printerdex("LINE",-1);
					write_printers();
					return;
				}
				set_printerdex("LINE",line);
				doupdate();
				write_printers();
			}
		}
		/*
		 * All other mouse functions
		 */
		else{
			find_mouse_settings(y,x);
		}
	}
	/*
	 * 
	 */
	else if(get_state("STATE") == 7)
	{
		find_mouse_manager(y,x);
	}
	/*
	 * 
	 */
	else if(get_state("STATE") == 8)
	{
		find_mouse_itm_wiz(y,x);
	}
	else if(get_state("STATE") == 9)
	{
		find_mouse_report_menu(y,x);
	}
	move(2,1);
	clrtoeol();
	printw("%d/%d",y,x);
	doupdate();
}
