#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/err.h"

/*
 * 
 */
void find_mouse_itm_wiz(int y,int x)
{
	/*
	 * If return is pressed
	 */
	if((y >= 39) & (y <= 41) & (x >= 6) & (x <= 20))
	{
		set_state("STATE",7);
		set_state("PREV_STATE",7);
		del_state(8);
		draw_state(7);
		update_panels();
		doupdate();
		return;
	}
	/*
	 * If home is pressed
	 */
	if((y >= 39) & (y <= 41) & (x >= 80) & (x <= 94))
	{
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		del_state(8);
		draw_state(1);
		update_panels();
		doupdate();
		return;
	}
}
