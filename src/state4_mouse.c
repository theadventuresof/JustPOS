#include <string.h>

#include "../lib/mouse.h"
#include "../lib/err.h"
#include "../lib/order.h"
#include "../lib/recall.h"
#include "../lib/scroll.h"
#include "../lib/file.h"
#include "../lib/state.h"
#include "../lib/print.h"
#include "../lib/keypad.h"
#include "../lib/report.h"

/*
 * 
 */
void find_mouse_recall_sys(int y,int x)
{
	/*
	 * If return button is pressed
	 */ 
	if((y >= 38) & (y <= 40) & (x >= 6) & (x <= 20))
	{
		del_state(4);
		reset_recall_date();
		del_recall_list();
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		draw_state(1);
	}
	/*
	 * If recall order button is pressed
	 */
	else if((y >= 13) & (y <= 15) & (x >= 10) & (x <= 30))
	{
		/*
		 * Recalldex state 1 is to tell the system orders are visible
		 * to be selected
		 */
		if(get_recalldex("STATE") == 1)
		{
			if(get_recalldex("LINE") < 0)
			{
				err_dialog("NO ORDER IS SELECTED");
				return;
			}
			char path[100];
			get_recall_date(path);
			append_order_recall(path);
			del_order();
			recall(path);
			scroll_to_top();
			sort_recall();
			write_list();
		}
		/*
		 * Recalldex state 2 is to tell the system the year folders 
		 * are to be selected
		 */
		else if(get_recalldex("STATE") == 2)
		{
			if(get_recalldex("LINE") >= 0)
			{
				set_recalldex("STATE",3);
				char path[100];
				get_recall_date(path);
				append_order_recall(path);
				strncat(path,"/",2);
				set_recalldex("CURRENT",0);
				set_recalldex("LINE",-1);
				set_recall_date(path);
				del_recall_list();
				find_recall_list(path,2);
				//sort_recall();
				write_recall();
			}
			else{
				err_dialog("NO YEAR IS SELECTED");
			}
		}
		/*
		 * Recalldex state 3 is to tell the system that month folders
		 * are to be selected
		 */
		else if(get_recalldex("STATE") == 3)
		{
			if(get_recalldex("LINE") < 0)
			{
				err_dialog("NO MONTH SELECTED");
				return;
			}	
			set_recalldex("STATE",4);
			char path[100];
			path[0] = '\0';
			append_order_recall(path);
			char full_path[100];
			get_recall_date(full_path);
			strncat(full_path,path,strlen(path) + 1);
			strncat(full_path,"/",2);
			set_recall_date(full_path);
			set_recalldex("CURRENT",0);
			set_recalldex("LINE",-1);
			del_recall_list();
			find_recall_list(full_path,2);
			sort_recall();
			write_recall();
		}
		/*
		 * Recalldex state 4 tells the system that a day folder is 
		 * to be selected
		 */
		else if(get_recalldex("STATE") == 4)
		{
			if(get_recalldex("LINE") >= 0)
			{
				set_recalldex("STATE",1);
				char path[100];
				path[0] = '\0';
				append_order_recall(path);
				char full_path[100];
				get_recall_date(full_path);
				strncat(full_path,path,strlen(path) + 1);
				strncat(full_path,"/",2);
				set_recall_date(full_path);
				del_recall_list();
				set_recalldex("CURRENT",0);
				set_recalldex("LINE",-1);
				find_recall_list(full_path,1);
				write_recall();
			}
		}
	}
	/*
	 * If change date button is pressed
	 */
	else if((y >= 20) & (y <= 22) & (x >= 10) & (x <= 30))
	{
		set_recalldex("STATE",2);
		del_recall_list();
		char path[100];
		get_file_data(".conf","dir=",path);
		find_recall_list(path,2);
		set_recall_date(path);
		set_recalldex("CURRENT",0);
		set_recalldex("LINE",-1);
		set_recalldex("MIN",0);
		set_recalldex("MAX",27);
		write_recall();
	}
	/*
	 * If search order button is pressed
	 */
	else if((y >= 16) & (y <= 18) & (x >= 10) & (x <= 30))
	{
		if(get_recalldex("STATE") != 1)
		{
			err_dialog("CHOOSE A DAY TO SEARCH");
			return;
		}
		set_state("STATE",2);
		set_state("PREV_STATE",4);
		set_keypad_state("FUNC",3);
		draw_keypad("CENTER");
		return;
	}
	/*
	 * If print order button is pressed
	 */
	else if((y >= 25) & (y <= 27) & (x >= 10) & (x <= 30))
	{
		if(get_recalldex("STATE") != 1)
		{
			err_dialog("NO ORDER SELECTED TO PRINT");
			return;
		}
		if(get_recalldex("LINE") < 0)
		{
			err_dialog("NO ORDER SELECTED TO PRINT");
			return;
		}
		char path[100];
		print_reciept(path);
		return;
	}
	/*
	 * If void order button is pressed
	 */
	else if((y >= 29) & (y <= 31) & (x >= 10) & (x <= 30))
	{
		
	}
}
