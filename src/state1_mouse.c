#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/mouse.h"
#include "../lib/report.h"
#include "../lib/keypad.h"
#include "../lib/order.h"
#include "../lib/draw.h"
#include "../lib/state.h"
#include "../lib/err.h"
#include "../lib/keyboard.h"
#include "../lib/recall.h" 
#include "../lib/scroll.h"
#include "../lib/file.h"
#include "../lib/draw_state_6.h"

/*
 * Find mouse events that occur over the 12 menu buttons
 */
void find_mouse_menu_buttons(int y,int x)
{
	int val;
	/*
	 * If menu button 1 is pressed
	 */
	if((y >= 5) & (y <= 9) & (x >= 9) & (x <= 38))
	{
		val = 1;
	}
	/*
	 * If menu button 2 is pressed
	 */
	if((y >= 5) & (y <= 9) & (x >= 39) & (x <= 68))
	{
		val = 2;
	}
	/*
	 * If menu button 3 is pressed
	 */
	if((y >= 5) & (y <= 9) & (x >= 69) & (x <= 98))
	{
		val = 3;
	}
	/*
	 * If menu button 4 is pressed
	 */
	if((y >= 10) & (y <= 14) & (x >= 9) & (x <= 38))
	{
		val = 4;
	}
	/*
	 * If menu button 5 is pressed
	 */
	if((y >= 10) & (y <= 14) & (x >= 39) & (x <= 68))
	{
		val = 5;
	}
	/*
	 * If menu button 6 is pressed
	 */
	if((y >= 10) & (y <= 14) & (x >= 69) & (x <= 98))
	{
		val = 6;
	}
	/*
	 * If menu button 7 is pressed
	 */
	if((y >= 15) & (y <= 19) & (x >= 9) & (x <= 29))
	{
		val = 7;
	}
	/*
	 * If menu button 8 is pressed
	 */
	if((y >= 15) & (y <= 19) & (x >= 39) & (x <= 68))
	{
		val = 8;
	}
	/*
	 * If menu button 9 is pressed
	 */
	if((y >= 15) & (y <= 19) & (x >= 69) & (x <= 98))
	{
		val = 9;
	}
	/*
	 * If menu button 10 is pressed
	 */
	if((y >= 20) & (y <= 24) & (x >= 9) & (x <= 38))
	{
		val = 10;
		if(get_state("BSTATE") == 3)
		{
			remove_mods(get_state("PREV_ITM"));
			write_list();
			update_order_stat();
			return;
		}
	}
	/*
	 * If menu button 11 is pressed
	 */
	if((y >= 20) & (y <= 24) & (x >= 39) & (x <= 68))
	{
		if(get_state("BSTATE") == 3)
		{
			return;
		}
		val = 11;
	}
	/*
	 * If menu button 12 is pressed
	 */
	if((y >= 20) & (y <= 24) & (x >= 69) & (x <= 98))
	{
		val = 12;
		if(get_state("BSTATE") == 3)
		{
			draw_keyboard();
			set_keyboard("FUNC",1);
			set_state("PREV_STATE",get_state("STATE"));
			set_state("STATE",3);
			return;
		}
	}
	val += get_page("PG_MIN");
	if(val == 0)
	{
		return;
	}
	if(get_state("BSTATE") < 3)
	{
		if((get_state("BSTATE") == 1) & (val > get_item_max()))
		{
			return;
		}
		if((get_state("BSTATE") == 2) & (val > get_item_max()))
		{
			return;
		}
		add_itm(val,get_state("BSTATE"));
		update_order_stat();
		write_list();
		return;
	}
	else if(get_state("BSTATE") == 3)
	{
		if(get_item_max() < val)
		{
			return;
		}
		add_mod(get_state("PREV_ITM"),val,3);
		write_list();
		return;
	}
	else if(get_state("BSTATE") == 4)
	{
		if(get_item_max() < val)
		{
			return;
		}
		add_mod(get_state("PREV_ITM"),val,4);
		update_order_stat();
		write_list();
		return;
	}
}

/*
 * Find mouse events that occur over menu page adjust buttons
 */
void find_mouse_menu_page_adjust(int y,int x)
{
	char menu[50];
	if(get_state("BSTATE") == 1)
	{
		strncpy(menu,"FOOD",5);
	}
	else if(get_state("BSTATE") == 2)
	{
		strncpy(menu,"DRINK",6);
	}
	else if(get_state("BSTATE") == 3)
	{
		strncpy(menu,"MOD",4);
	}
	else if(get_state("BSTATE") == 4)
	{
		strncpy(menu,"CHARGE",7);
	}
	
	/*
	 * If menu page <- is pressed
	 */
	if((y >= 24) & (y <= 27) & (x >= 37) & (x <= 41))
	{
		if((get_index(menu) - 1) >= 1)
		{
			set_index(menu,(get_index(menu) - 1));
			write_to_menu_buttons();
			write_pages();
		} 
	}
	/*
	 * If menu page -> is pressed
	 */
	if((y >= 24) & (y <= 27) & (x >= 66) & (x <= 71))
	{
		if((get_index(menu) + 1) <= get_max_pages(menu))
		{
			set_index(menu,(get_index(menu) + 1));
			write_to_menu_buttons();
			write_pages();
		}
	}
	update_panels();
	doupdate();
}


/*
 * Find mouse events that occur over system (Function) buttons
 */
void find_mouse_system_buttons(int y,int x)
{
	/*
	 * If clear order button is pressed
	 */
	if((y >= 27) & (y <= 29) & (x <= 19))
	{
		del_order();
		write_list();	
		update_order_stat();
		write_pages();
		set_state("HIGHLIGHT",0);
		set_state("PREV_ITM",0);
		set_scrolldex("MAX_LINE",1);
		if(get_state("BSTATE") > 2)
		{
			set_index("FOOD",1);
			set_state("BSTATE",1);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		update_panels();
		doupdate();
		return;
	}
	/*
	 * If del itm button is pressed
	 */ 
	if((y >= 27) & (y <= 29) & (x >= 25) & (x <= 44))
	{
		if(get_state("HIGHLIGHT") == 0)
		{
			err_dialog("PLEASE SELECT AN ITEM TO DELETE");
			return;
		}
		del_itm(get_state("PREV_ITM"));
		write_list();
		update_order_stat();
		update_panels();
		doupdate();
		return;
	}
	/*
	 * If adj qty button is pressed
	 */
	if((y >= 27) & (y <= 29) & (x >= 51) & (x <= 68))
	{
		if(get_state("HIGHLIGHT") == 1)
		{
			set_state("STATE",2);
			set_keypad_state("FUNC",1);
			draw_keypad("CENTER");
		}
		else{
			err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
		}
		return;
	}
	/*
	 * If open food button is pressed
	 */
	if((y >= 27) & (y <= 29) & (x >= 76) & (x <= 94))
	{
		set_state("STATE",2);
		set_keypad_state("FUNC",2);
		draw_keypad("CENTER");
		return;
	}
	/*
	 * If recall button is pressed
	 */
	if((y >= 30) & (y <= 32) & (x <= 19))
	{
		del_state(1);
		set_state("PREV_STATE",4);
		set_state("STATE",4);
		draw_state(4);
		del_recall_list();
		char date[100];
		get_dir_date(date);
		find_recall_list(date,1);
		set_recall_date(date);
		set_recalldex("STATE",1);
		set_recalldex("CURRENT",0);
		set_recalldex("LINE",-1);
		write_list();
		write_recall();
		return;
	}
	/*
	 * If search button is pressed
	 */
	if((y >= 30) & (y <= 32) & (x >= 25) & (x <= 44))
	{
		draw_keyboard();
		set_state("PREV_STATE",get_state("STATE"));
		set_state("STATE",3);
		set_keyboard("FUNC",2);
		return;
	}
	/*
	 * If settings is pressed
	 */
	if((y >= 34) & (y <= 36) & (x <= 20))
	{
		del_state(1);
		set_state("PREV_STATE",6);
		set_state("STATE",6);
		draw_state(6);
		return;
	}
	/*
	 * If manager button is pressed
	 */
	if((y >= 34) & (y <= 36) & (x >= 25) & (x <= 44))
	{
		del_state(1);
		set_state("PREV_STATE",7);
		set_state("STATE",7);
		draw_state(7);
	}
	/*
	 * If pay card button is pressed
	 */
	if((y >= 34) & (y <= 36) & (x >= 50) & (x <= 69))
	{
		yes_no_dialog("MSG",1);
	}
	/*
	 * If pay cash button is pressed
	 */
	if((y >= 34) & (y <= 36) & (x >= 76) & (x <= 94))
	{
		if(total_items() == 0)
		{
			err_dialog("NO ITEMS TO PAY FOR");
			return;
		}
		del_state(1);
		erase();
		draw_state(5);
		set_state("PREV_STATE",5);
		set_state("STATE",5);
		return;
	}
}

/*
 * Find mouse events that occur over menu tabs
 */
void find_mouse_menu_tabs(int y,int x)
{
	if(get_state("BSTATE") == 1)
	{
		if((y >= 12) & (y <= 16) & (x <= 8))
		{
			set_state("BSTATE",2);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 17) & (y <= 22) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",3);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
		if((y >= 22) & (y <= 27) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",4);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
	}
	else if(get_state("BSTATE") == 2)
	{
		if((y >= 5) & (y <= 10) & (x <= 8))
		{
			set_state("BSTATE",1);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 17) & (y <= 21) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",3);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
		if((y >= 21) & (y <= 27) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",4);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
	}
	else if(get_state("BSTATE") == 3)
	{
		if((y >= 5) & (y <= 10) & (x <= 8))
		{
			set_state("BSTATE",1);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 10) & (y <= 15) & (x <= 8))
		{
			set_state("BSTATE",2);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 22) & (y <= 27) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",4);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
	}
	else if(get_state("BSTATE") == 4)
	{
		if((y >= 5) & (y <= 10) & (x <= 8))
		{
			set_state("BSTATE",1);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 10) & (y <= 15) & (x <= 8))
		{
			set_state("BSTATE",2);
			shuffle_tabs();
			write_pages();
			write_to_menu_buttons();
		}
		if((y >= 15) & (y <= 20) & (x <= 8))
		{
			if(get_state("HIGHLIGHT") == 1)
			{
				set_state("BSTATE",3);
				shuffle_tabs();
				write_pages();
				write_to_menu_buttons();
			}
			else{
				err_dialog("PLEASE SELECT AN ITEM TO MODIFY");
			}
		}
	}
}


/*
 * Find mouse events that occur over keypad 
 */
void find_mouse_keypad(int y,int x)
{
	int starty,startx;
	getmaxyx(stdscr,starty,startx);
	if(get_keypad_state("STATE") == 1)
	{
		starty = (starty/2) - 8;
	}
	else if(get_keypad_state("STATE") == 2)
	{
		starty = 13;
	}
	if(get_keypad_state("STATE") == 1)
	{
		startx = (startx/2) - 27;
	}
	else if(get_keypad_state("STATE") == 2)
	{
		startx = 9;
	}
	
	/*
	 * If number 1 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("1");
		return;
	}
	/*
	 * If number 2 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("2");
		return;
	}
	/*
	 * If number 3 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("3");
		return;
	}
	/*
	 * If number 4 is pressed
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("4");
		return;
	} 
	/*
	 * If number 5 is pressed
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("5");
		return;
	}
	/*
	 * If number 6 is pressed 
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("6");
		return;
	}
	/*
	 * If number 7 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("7");
		return;
	}
	/*
	 * If number 8 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("8");
		return;
	}
	/*
	 * If number 9 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("9");
		return;
	}
	/*
	 * If number 0 is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("0");
		return;
	}
	/*
	 * If decimal is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val(".");
		return;
	}
	/*
	 * If back arrow is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx + 29) & (x <= startx + 38))
	{
		keypad_back();
		return;
	}
	/*
	 * If keypad cancel is pressed
	 */
	if((y >= starty) & (y <= starty + 9) & (x >= startx + 46) & (x <= startx + 58))
	{
		if(get_state("STATE") != 5)
		{
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
		}
	}
	/*
	 * If keypad clear is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx + 46) & (x <= startx + 58))
	{
		clear_keypad();
	}
	/*
	 * If keypad OK is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx + 46) & (x <= startx + 58))
	{
		/*
		 * FUNC 1 = modify item quantity
		 */
		if((get_keypad_state("FUNC") == 1) & (get_keypad_data() > 0))
		{
			int data = (int)get_keypad_data();
			modify_qty(get_state("PREV_ITM"),data);
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			update_order_stat();
			write_list();
			return;
		}
		/*
		 * FUNC 2 = Add open food item/charge
		 */
		else if(get_keypad_state("FUNC") == 2)
		{
			add_open_food(get_keypad_data());
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			update_order_stat();
			write_list();
			return;
		}
		/*
		 * FUNC 3 = Search for order in specified day -- recall menu
		 */
		else if(get_keypad_state("FUNC") == 3)
		{
			char order[50];
			sprintf(order,"order-%.0f",get_keypad_data());
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			int check = find_recall_order(order);
			if(check >= 0)
			{
				set_recalldex("LINE",check);
				write_recall();
			}
			return;
		}
		/*
		 * FUNC 4 = Evaluate cash payment
		 */
		else if(get_keypad_state("FUNC") == 4)
		{
			float total = get_keypad_data();
			float owed = calc_total();
			float change;
			if(total < owed)
			{
				err_dialog("INSUFFICIENT FUNDS!");
				return;
			}
			else if(total >= owed)
			{
				change = total - owed;
				save_order(total,change);
				gen_today();
				del_order();
				change_dialog(change);
			}
			return;
		}
		/*
		 * FUNC 5 = printer1-copies
		 */
		else if(get_keypad_state("FUNC") == 5)
		{
			char num[50];
			float copies = get_keypad_data();
			sprintf(num,"printer1-copies=%.0f",copies);
			change_conf_line("printer1-copies=",num);
			delete_keypad();
			set_state("STATE",6);
			draw_settings_windows();
			return;
		}
		/*
		 * FUNC 6 = printer2-copies
		 */
		else if(get_keypad_state("FUNC") == 6)
		{
			char num[50];
			float copies = get_keypad_data();
			sprintf(num,"printer2-copies=%.0f",copies);
			change_conf_line("printer2-copies=",num);
			delete_keypad();
			set_state("STATE",6);
			draw_settings_windows();
			return;	
		}
	}
}	
