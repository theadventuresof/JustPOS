#include "../lib/justpos.h"

/*
 * The main entry point for mouse/touch input
 */
void get_mouse(int y,int x)
{
	/*
	 * If error dialog is visible
	 */
	if(get_state("STATE") == 0)
	{
		/*
		 * If OK is pressed
		 */
		if((y >= 23) & (y <= 25) & (x >= 75) & (x <= 84))
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
		if((y >= 6) & (y <= 34) & (x >= 9) & (x <= 30))
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
	}
	/*
	 * Show mouse coordinates on screen
	 */
	move(2,1);
	clrtoeol();
	printw("%d/%d",y,x);
	doupdate();
}
