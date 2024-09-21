#include <panel.h>

#include "../lib/draw_state_7.h"

WINDOW *manager_return;
PANEL *manager_returnp;

/*
 * 
 */
void draw_manager_buttons(void)
{
	/*
	 * Draw return to main menu button
	 */
	manager_return = newwin(3,15,38,5);
	box(manager_return,0,0);
	mvwprintw(manager_return,1,5,"RETURN");
	manager_returnp = new_panel(manager_return);
	update_panels();
	doupdate();
}

/*
 * 
 */
void del_manager_buttons(void)
{
	if(manager_return != NULL)
	{
		del_panel(manager_returnp);
		manager_return = NULL;
	}
}
