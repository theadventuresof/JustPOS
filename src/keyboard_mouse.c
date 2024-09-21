#include <panel.h>
#include <string.h>

#include "../lib/keyboard.h"
#include "../lib/state.h"
#include "../lib/order.h"
#include "../lib/misc.h"
#include "../lib/file.h"

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
		/*
		 * If special instruction is to be added to an item
		 */
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
		/*
		 * If an item is to be searched for in the menu files
		 */
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
		/*
		 * If default order directory is to be changed (settings menu)
		 */
		else if(get_keyboard("FUNC") == 3)
		{
			char full[100];
			char dir[100];
			strncpy(full,"dir=",5);
			copy_keyboard_val(dir);
			strncat(full,dir,strlen(dir) + 1);
			if(full[strlen(full)] != '/')
			{
				strncat(full,"/",2);
			}
			change_conf_line("dir=",full);
			clear_keyboard();
			del_keyboard();
			set_state("STATE",get_state("PREV_STATE"));
			return;
		}
		/*
		 * If the receipt header is to be changed (settings menu) 
		 */
		else if(get_keyboard("FUNC") == 4)
		{
			char full[120];
			char msg[100];
			strncpy(full,"msg=",5);
			copy_keyboard_val(msg);
			strncat(full,msg,strlen(msg) + 1);
			change_conf_line("msg=",full);
			clear_keyboard();
			del_keyboard();
			set_state("STATE",get_state("PREV_STATE"));
			return;
		}
		/*
		 * If the phone number/socail media/contact info is to be 
		 * updated (settings menu)
		 */
		else if(get_keyboard("FUNC") == 5)
		{
			char full[100];
			char phone[50];
			strncpy(full,"contact=",9);
			copy_keyboard_val(phone);
			strncat(full,phone,strlen(phone)+1);
			change_conf_line("contact=",full);
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
	/*
	 * If 123/ABC is pressed (if state is to be changed)
	 */
	if((y >= max_y + 16) & (y <= max_y + 19) & (x >= (max_x/2) - 50) & (x <= ((max_x/2) - 50) + 10))
	{
		if(get_keyboard("STATE") == 1)
		{
			set_keyboard("STATE",2);
			write_keys();
		}
		else if(get_keyboard("STATE") == 2)
		{
			set_keyboard("STATE",1);
			write_keys();
		}
	}
	//Row 1 keys
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (0 * 15) + 5) & (x <= (0 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("Q");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("1");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (1 * 15) + 5) & (x <= (1 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("W");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("2");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (2 * 15) + 5) & (x <= (2 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("E");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("3");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (3 * 15) + 5) & (x <= (3 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("R");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("4");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (4 * 15) + 5) & (x <= (4 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("T");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("5");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (5 * 15) + 5) & (x <= (5 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("Y");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("6");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (6 * 15) + 5) & (x <= (6 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("U");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("7");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (7 * 15) + 5) & (x <= (7 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("I");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("8");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (8 * 15) + 5) & (x <= (8 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("O");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("9");
		}
	}
	else if((y >= max_y + 4) & (y <= max_y + 7) & (x >= (9 * 15) + 5) & (x <= (9 * 15) + 13))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("P");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("0");
		}
	}
	//Row 2 keys
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (0 * 15) + 8) & (x <= (0 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("A");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("-");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (1 * 15) + 8) & (x <= (1 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("S");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("/");
		}	
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (2 * 15) + 8) & (x <= (2 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("D");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key(":");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (3 * 15) + 8) & (x <= (3 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("F");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("_");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (4 * 15) + 8) & (x <= (4 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("G");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("(");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (5 * 15) + 8) & (x <= (5 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("H");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key(")");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (6 * 15) + 8) & (x <= (6 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("J");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("$");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (7 * 15) + 8) & (x <= (7 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("K");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("&");
		}
	}
	else if((y >= max_y + 8) & (y <= max_y + 11) & (x >= (8 * 15) + 8) & (x <= (8 * 15) + 16))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("L");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("@");
		}
	}
	//Row three keys
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (0 * 15) + 11) & (x <= (0 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("Z");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("!");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (1 * 15) + 11) & (x <= (1 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("X");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("#");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (2 * 15) + 11) & (x <= (2 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("C");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("?");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (3 * 15) + 11) & (x <= (3 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("V");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("*");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (4 * 15) + 11) & (x <= (4 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("B");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("+");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (5 * 15) + 11) & (x <= (5 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("N");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key(".");
		}
	}
	else if((y >= max_y + 12) & (y <= max_y + 15) & (x >= (6 * 15) + 11) & (x <= (6 * 15) + 19))
	{
		if(get_keyboard("STATE") == 1)
		{
			add_key("M");
		}
		else if(get_keyboard("STATE") == 2)
		{
			add_key("^");
		}
	}

}
