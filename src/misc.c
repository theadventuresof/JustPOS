#include "../lib/justpos.h"

/*
 * Print date and time information to stdscr
 */
void print_clock(void)
{
	char time[50];
	get_time(time);
	move(1,0);
	clrtoeol();
	printw("%s",time);
	update_panels();
	doupdate();
}

/*
 * 
 */
void search(char val[])
{
	int itm_num,i;
	for(i = 1; i < 3; i++)
	{
		itm_num = check_menu_line(val,i);
		if(itm_num > 0)
		{
			char name[50];
			get_name(i,name,itm_num);
			set_state("BSTATE",i);
			shuffle_tabs();
			write_to_menu_buttons();
			if(i == 1)
			{
				set_index("FOOD",itm_num/12 + 1);
				write_to_menu_buttons();
				write_pages();
				itm_num = (itm_num - (12 * (get_index("FOOD") -1)));
				select_menu_button(itm_num);
				return;
			}
			else if(i == 2)
			{
				set_index("DRINK",itm_num/12 + 1);
				write_to_menu_buttons();
				write_pages();
				itm_num = (itm_num - (12 * (get_index("DRINK") - 1)));
				select_menu_button(itm_num);
				return;
			}
			doupdate();
		} 
		else if(itm_num == 0)
		{
			//err_dialog("ITM NOT FOUND!!");
			write_to_menu_buttons();
		}
	}
}
