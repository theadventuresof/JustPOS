#include "../lib/justpos.h"

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
		if((get_state("BSTATE") == 1) & (val > get_item_max("FOOD")))
		{
			return;
		}
		if((get_state("BSTATE") == 2) & (val > get_item_max("DRINK")))
		{
			return;
		}
		add_itm(val,get_state("BSTATE"));
		update_order_stat();
		write_list();
	}
	else if(get_state("BSTATE") == 3)
	{
		if(get_item_max("MOD") < val)
		{
			return;
		}
		add_mod(get_state("PREV_ITM"),val,3);
		write_list();
	}
	else if(get_state("BSTATE") == 4)
	{
		if(get_item_max("CHARGE") < val)
		{
			return;
		}
		add_mod(get_state("PREV_ITM"),val,4);
		update_order_stat();
		write_list();
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
	}
	/*
	 * If open food button is pressed
	 */
	if((y >= 27) & (y <= 29) & (x >= 76) & (x <= 94))
	{
		set_state("STATE",2);
		set_keypad_state("FUNC",2);
		draw_keypad("CENTER");
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
	 * If pay cash button is pressed
	 */
	if((y >= 34) & (y <= 36) & (x >= 76) & (x <= 94))
	{
		del_state(1);
		erase();
		draw_state(5);
		set_state("PREV_STATE",1);
		set_state("STATE",5);
	}
	/*
	 * Bottom left corner saves order to file (creates receipt)
	 * TESTING ONLY
	 */
	if((y >= 34) & (y <= 36) & (x <= 20))
	{
		if(total_items() > 0)
		{
			save_order();
		}
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
		//startx = startx - 58;
		startx = 5;
	}
	
	/*
	 * If number 1 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("1");
	}
	/*
	 * If number 2 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("2");
	}
	/*
	 * If number 3 is pressed
	 */
	if((y >= starty) & (y <= starty + 3) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("3");
	}
	/*
	 * If number 4 is pressed
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("4");
	} 
	/*
	 * If number 5 is pressed
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("5");
	}
	/*
	 * If number 6 is pressed 
	 */
	if((y >= starty + 6) & (y <= starty + 9) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("6");
	}
	/*
	 * If number 7 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val("7");
	}
	/*
	 * If number 8 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("8");
	}
	/*
	 * If number 9 is pressed
	 */
	if((y >= starty + 12) & (y <= starty + 15) & (x >= startx + 29) & (x <= startx + 38))
	{
		set_keypad_val("9");
	}
	/*
	 * If number 0 is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx + 15) & (x <= startx + 22))
	{
		set_keypad_val("0");
	}
	/*
	 * If decimal is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx) & (x <= startx + 8))
	{
		set_keypad_val(".");
	}
	/*
	 * If back arrow is pressed
	 */
	if((y >= starty + 18) & (y <= starty + 21) & (x >= startx + 29) & (x <= startx + 38))
	{
		keypad_back();
	}
	/*
	 * If keypad cancel is pressed
	 */
	if((y >= starty) & (y <= starty + 9) & (x >= startx + 46) & (x <= startx + 58))
	{
		if(get_state("STATE") != 5)
		{
			clear_keypad();
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
		if((get_keypad_state("FUNC") == 1) & (get_keypad_data() > 0))
		{
			int data = (int)get_keypad_data();
			modify_qty(get_state("PREV_ITM"),data);
			clear_keypad();
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			update_order_stat();
			write_list();
		}
		else if(get_keypad_state("FUNC") == 2)
		{
			add_open_food(get_keypad_data());
			clear_keypad();
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			update_order_stat();
			write_list();
		}
		else if(get_keypad_state("FUNC") == 3)
		{
			char order[50];
			sprintf(order,"order-%.0f",get_keypad_data());
			clear_keypad();
			delete_keypad();
			set_state("STATE",get_state("PREV_STATE"));
			int check = find_recall_order(order);
			if(check >= 0)
			{
				set_recalldex("LINE",check);
				write_recall();
			}
		}
	}
}	
