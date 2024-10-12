#include <panel.h>

#include "../lib/draw_state_7.h"

WINDOW *help;
PANEL *helpp;

WINDOW *quit;
PANEL *quitp;

WINDOW *itm_wiz;
PANEL *itm_wizp;

/*
 * 
 */
void draw_manager_buttons(void)
{
	del_manager_buttons();
	/*
	 * Draw help button
	 */
	help = newwin(3,16,38,80);
	box(help,0,0);
	mvwprintw(help,1,6,"HELP");
	helpp = new_panel(help);
	
	/*
	 * Draw quit button
	 */
	quit = newwin(3,16,38,110);
	box(quit,0,0);
	mvwprintw(quit,1,6,"QUIT");
	quitp = new_panel(quit);
	
	/*
	 * Draw Item Wizard button
	 */
	itm_wiz = newwin(3,20,5,5);
	box(itm_wiz,0,0);
	mvwprintw(itm_wiz,1,5,"ITEM WIZARD");
	itm_wizp = new_panel(itm_wiz);
	
	
	update_panels();
	doupdate();
}

/*
 * 
 */
void del_manager_buttons(void)
{
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
	if(itm_wiz != NULL)
	{
		del_panel(itm_wizp);
		itm_wiz = NULL;
	}
}
