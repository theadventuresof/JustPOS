#include <panel.h>
#include <string.h>

#include "../lib/draw_state_6.h"
#include "../lib/report.h"

WINDOW *settings_return;
PANEL *settings_returnp;

WINDOW *order_dir;
PANEL *order_dirp;

/*
 * Draw return button for settings screen
 */
void draw_settings_return(void)
{
	del_settings_windows();
	/*
	 * 
	 */
	order_dir = newwin(3,50,5,10);
	box(order_dir,0,0);
	char orddir[100];
	get_file_data(".conf","dir=",orddir);
	mvwprintw(order_dir,1,2,"%s",orddir);
	mvwprintw(stdscr,4,11,"Parent directory for receipt storage:");
	order_dirp = new_panel(order_dir);
	/*
	 * Draw return to main menu button
	 */
	settings_return = newwin(3,15,38,5);
	box(settings_return,0,0);
	mvwprintw(settings_return,1,5,"RETURN");
	settings_returnp = new_panel(settings_return);
	update_panels();
	doupdate();
}

/*
 * 
 */
void del_settings_windows(void)
{
	if(settings_return != NULL)
	{
		del_panel(settings_returnp);
		settings_return = NULL;
	}
	if(order_dir != NULL)
	{
		del_panel(order_dirp);
		order_dir = NULL;
	}
}
