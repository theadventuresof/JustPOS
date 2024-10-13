#include <panel.h>
#include <stdlib.h>

#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/mouse.h"

/*
 * 
 */
void find_mouse_manager(int y,int x)
{
	/*
	 * If Item Wizard button is pressed
	 */
	if((y >= 6) & (y <= 8) & (x >= 5) & (x <= 23))
	{
		set_state("PREV_STATE",8);
		set_state("STATE",8);
		del_state(7);
		draw_state(8);
		update_panels();
		doupdate();
		return;
	}
	/*
	 * If return button is pressed
	 */
	if((y >= 38) & (y <= 40) & (x >= 5) & (x <= 20))
	{
		set_state("PREV_STATE",1);
		set_state("STATE",1);
		del_state(7);
		draw_state(1);
		return;
	}
	/*
	 * If quit button is pressed
	 */ 
	if((y >= 38) & (y <= 40) & (x >= 110) & (x <= 125))
	{
		/*
		* Turn off mouse reporting for terminal
		*/
		printf("\033[?1003l\n");
		endwin();
		system("reset");
		exit(0);
	}
}
