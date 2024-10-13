#include <panel.h>

#include "../lib/draw_state_8.h"

WINDOW *main_menu;
PANEL *main_menup;

/*
 * 
 */
void draw_itm_wiz_btns(void)
{
	del_itm_wiz_btns();
	/*
	 * Draw main menu button
	 */
	main_menu = newwin(3,15,38,80);
	box(main_menu,0,0);
	mvwprintw(main_menu,1,6,"HOME");
	main_menup = new_panel(main_menu);
}

/*
 * 
 */
void del_itm_wiz_btns(void)
{
	if(main_menu != NULL)
	{
		del_panel(main_menup);
		main_menu = NULL;
	}
}
