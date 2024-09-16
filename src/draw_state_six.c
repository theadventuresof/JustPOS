#include <panel.h>

#include "../lib/draw_state_6.h"

WINDOW *settings_return;
PANEL *settings_returnp;

/*
 * Draw return button for settings screen
 */
void draw_settings_return(void)
{
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
}
