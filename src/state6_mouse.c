#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/state.h"

void find_mouse_settings(int y,int x)
{
	/*
	 * If return is pressed
	 */
	if((y >= 38) & (y <= 40) & (x >= 6) & (x <= 20))
	{
		erase();
		del_state(6);
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		draw_state(1);
		return;
	}
}
