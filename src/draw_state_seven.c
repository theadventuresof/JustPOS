#include <panel.h>

#include "../lib/draw_state_7.h"

WINDOW *manager_return;
PANEL *manager_returnp;

WINDOW *help;
PANEL *helpp;

WINDOW *quit;
PANEL *quitp;

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
	
	/*
	 * Draw help button
	 */
	help = newwin(3,15,38,80);
	box(help,0,0);
	mvwprintw(help,1,6,"HELP");
	helpp = new_panel(help);
	
	/*
	 * Draw quit button
	 */
	quit = newwin(3,15,38,110);
	box(quit,0,0);
	mvwprintw(quit,1,6,"QUIT");
	quitp = new_panel(quit);
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
	if(help != NULL)
	{
		del_panel(helpp);
		help = NULL;
	}
	if(quit != NULL)
	{
		del_panel(quitp);
		quit = NULL;
	}
}
