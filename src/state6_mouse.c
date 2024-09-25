#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/keyboard.h"
#include "../lib/draw_state_6.h"

void find_mouse_settings(int y,int x)
{
	/*
	 * If order_dir window is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 10) & ( x <= 60))
	{	
		draw_keyboard();
		set_keyboard("FUNC",3);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
	/*
	 * If conf_msg window is pressed
	 */
	if((y >= 11) & (y <= 16) & (x >= 10) & (x <= 60))
	{
		draw_keyboard();
		set_keyboard("FUNC",4);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
	/*
	 * If phone/social window is pressed
	 */
	if((y >= 20) & (y <= 22) & (x >= 10) & (x <= 60))
	{
		draw_keyboard();
		set_keyboard("FUNC",5);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
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
	}
	/*
	 * If enable printer_1 is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 90) & (x <= 96))
	{
		if(get_state("PRINTER_1") == 1)
		{
			set_state("PRINTER_1",0);
		}
		else if(get_state("PRINTER_1") == 0)
		{
			set_state("PRINTER_1",1);
		}
		draw_settings_windows();
	}
	/*
	 * If enable printer_2 is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 121) & (x <= 127))
	{
		if(get_state("PRINTER_2") == 1)
		{
			set_state("PRINTER_2",0);
		}
		else if(get_state("PRINTER_2") == 0)
		{
			set_state("PRINTER_2",1);
		}
		draw_settings_windows();
	}
}
