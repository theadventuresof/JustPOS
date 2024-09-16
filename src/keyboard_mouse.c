#include <panel.h>

#include "../lib/keyboard.h"
#include "../lib/state.h"
#include "../lib/order.h"
#include "../lib/misc.h"

/*
 * Find mouse events that occur over keyboard
 */
void find_mouse_keyboard(int y,int x)
{
	int max_y,max_x;
	getmaxyx(stdscr,max_y,max_x);
	max_y = max_y/2;
	/*
	 * If close keyboard is pressed
	 */
	if((y >= max_y + 1) & (y <= max_y + 4) & (x >= max_x - 8) & (x <= max_x -1))
	{
		clear_keyboard();
		del_keyboard();
		set_keyboard("FUNC",0);
		set_state("STATE",get_state("PREV_STATE"));
	}
	/*
	 * If keyboard clear is pressed
	 */
	if((y >= max_y + 13) & (y <= max_y + 16) & (x >= 140) & (x <= 153))
	{
		clear_keyboard();
	}
	/*
	 * If keyboard backspace is pressed
	 */
	if((y >= max_y + 9) & (y <= max_y + 12) & (x >= 140) & (x <= 153))
	{
		backspace_keyboard();
	}
	/*
	 * If keyboard done is pressed
	 */
	if((y >= max_y + 17) & (y <= max_y + 20) & (x >= 140) & (x <= 153))
	{
		if(get_keyboard("FUNC") == 1)
		{
			char msg[50];
			copy_keyboard_val(msg);
			add_msg(get_state("PREV_ITM"),msg);
			clear_keyboard();
			del_keyboard();
			write_list();
			set_state("STATE",get_state("PREV_STATE"));
			update_panels();
			doupdate();
			return;
		}
		else if(get_keyboard("FUNC") == 2)
		{
			char msg[50];
			copy_keyboard_val(msg);
			search(msg);
			clear_keyboard();
			del_keyboard();
			set_state("STATE",get_state("PREV_STATE"));
			return;
		}
	}
	/*
	 * If keyboard space is pressed
	 */
	if((y >= max_y + 16) & (y <= max_y + 19) & (x >= (max_x/2) - 20) & (x <= ((max_x/2) - 20) + 40))
	{
		add_key(" ");
	} 
	//Row 1 keys
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (0 * 15) + 5) & (x <= (0 * 15) + 13))
	{
		add_key("Q");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (1 * 15) + 5) & (x <= (1 * 15) + 13))
	{
		add_key("W");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (2 * 15) + 5) & (x <= (2 * 15) + 13))
	{
		add_key("E");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (3 * 15) + 5) & (x <= (3 * 15) + 13))
	{
		add_key("R");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (4 * 15) + 5) & (x <= (4 * 15) + 13))
	{
		add_key("T");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (5 * 15) + 5) & (x <= (5 * 15) + 13))
	{
		add_key("Y");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (6 * 15) + 5) & (x <= (6 * 15) + 13))
	{
		add_key("U");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (7 * 15) + 5) & (x <= (7 * 15) + 13))
	{
		add_key("I");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (8 * 15) + 5) & (x <= (8 * 15) + 13))
	{
		add_key("O");
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (9 * 15) + 5) & (x <= (9 * 15) + 13))
	{
		add_key("P");
	}
	//Row 2 keys
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (0 * 15) + 8) & (x <= (0 * 15) + 16))
	{
		add_key("A");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (1 * 15) + 8) & (x <= (1 * 15) + 16))
	{
		add_key("S");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (2 * 15) + 8) & (x <= (2 * 15) + 16))
	{
		add_key("D");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (3 * 15) + 8) & (x <= (3 * 15) + 16))
	{
		add_key("F");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (4 * 15) + 8) & (x <= (4 * 15) + 16))
	{
		add_key("G");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (5 * 15) + 8) & (x <= (5 * 15) + 16))
	{
		add_key("H");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (6 * 15) + 8) & (x <= (6 * 15) + 16))
	{
		add_key("J");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (7 * 15) + 8) & (x <= (7 * 15) + 16))
	{
		add_key("K");
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (8 * 15) + 8) & (x <= (8 * 15) + 16))
	{
		add_key("L");
	}
	//Row three keys
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (0 * 15) + 11) & (x <= (0 * 15) + 19))
	{
		add_key("Z");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (1 * 15) + 11) & (x <= (1 * 15) + 19))
	{
		add_key("X");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (2 * 15) + 11) & (x <= (2 * 15) + 19))
	{
		add_key("C");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (3 * 15) + 11) & (x <= (3 * 15) + 19))
	{
		add_key("V");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (4 * 15) + 11) & (x <= (4 * 15) + 19))
	{
		add_key("B");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (5 * 15) + 11) & (x <= (5 * 15) + 19))
	{
		add_key("N");
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (6 * 15) + 11) & (x <= (6 * 15) + 19))
	{
		add_key("M");
	}

}
