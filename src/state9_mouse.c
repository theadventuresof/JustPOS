#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/state.h"

void find_mouse_report_menu(int y,int x)
{
	/*
	 * If return button is pressed
	 */
	if((y >= 38) & (y <= 40) & (x >= 5) & (x <= 18))
	{
		del_state(9);
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		draw_state(1);
		return;
	}
}
