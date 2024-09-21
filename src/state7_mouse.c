#include <panel.h>


#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/mouse.h"

/*
 * 
 */
void find_mouse_manager(int y,int x)
{
	if((y >= 38) & (y <= 40) & (x >= 5) & (x <= 20))
	{
		set_state("PREV_STATE",1);
		set_state("STATE",1);
		del_state(7);
		draw_state(1);
	} 
}
