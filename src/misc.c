#include <panel.h>
#include <string.h>

#include "../lib/misc.h"
#include "../lib/item.h"
#include "../lib/state.h"
#include "../lib/draw.h"
#include "../lib/file.h"

/*
 * Print date and time information to stdscr
 */
void print_clock(void)
{
	char time[50];
	get_time(time);
	move(0,66);
	clrtoeol();
	attron(COLOR_PAIR(1));
	printw("%s",time);
	attroff(COLOR_PAIR(1));
	update_panels();
	doupdate();
}

/*
 * Search through food and drink menu for name match
 */
void search(char val[])
{
	int itm_num,i;
	/*
	 * Loop through both menus looking for a match
	 */
	for(i = 1; i < 3; i++)
	{
		/*
		 * itm_num > 0 means a match is found
		 */
		itm_num = check_menu_line(val,i);
		if(itm_num > 0)
		{
			/*
			 * Make sure that correct menu is visible
			 */
			set_state("BSTATE",i);
			shuffle_tabs();
			/*
			 * If i = 1, match was found in food menu, set index 
			 * accordingly
			 */
			if(i == 1)
			{
				set_index("FOOD",itm_num/13+1);
				itm_num = (itm_num - (12 * (get_index("FOOD") - 1)));
			}
			/*
			 * If i = 2, match was found in drink menu, set index 
			 * accordingly
			 */
			else if(i == 2)
			{
				set_index("DRINK",itm_num/13+1);
				itm_num = (itm_num - (12 * (get_index("DRINK") - 1)));
			}
			/*
			 * Show updates on screen 
			 */
			write_to_menu_buttons();
			write_pages();
			select_menu_button(itm_num);
			set_state("CUR_CELL",itm_num-1);
			return;
		} 
	}
}

/*
 * Copy a fragment of a string from source to dest -- used by save_order
 */
void copy_fragment(char source[],int begin,int end,char *dest)
{
	char temp[end-begin+1];
	int i=begin,k=0;
	for(i = begin; i < end; i++)
	{
		temp[k] = source[i];
		temp[k + 1] = '\0';
		k++;
	}
	strncpy(dest,temp,strlen(temp) + 1);
}

WINDOW *go_back;
PANEL *go_backp;
/*
 * Draw return to main menu button
 */
void draw_return(void)
{
	del_return();
	go_back = newwin(3,15,38,5);
	box(go_back,0,0);
	mvwprintw(go_back,1,5,"RETURN");
	go_backp = new_panel(go_back);
}

/*
 * Delete return button from memory
 */
void del_return(void)
{
	if(go_back != NULL)
	{
		del_panel(go_backp);
		go_back = NULL;
	}
}

/*
 * Use unicode symbols to draw 3x3 block size numbers
 */
void draw_number(char num,char *line1,char *line2,char *line3)
{
	if(num == '1')
	{
		strncpy(line1," \u259D\u2588",13);
		strncpy(line2,"  \u2588",10);
		strncpy(line3,"  \u2580",10);
	}
	else if(num == '2')
	{
		strncpy(line1,"\u2580\u2580\u2588",13);
		strncpy(line2,"\u2588\u2580\u2580",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '3')
	{
		strncpy(line1,"\u2580\u2580\u2588",13);
		strncpy(line2," \u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '4')
	{
		strncpy(line1,"\u2588 \u2588",10);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"  \u2580",8);
	}
	else if(num == '5')
	{
		strncpy(line1,"\u2588\u2580\u2580",13);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '6')
	{
		strncpy(line1,"\u2588\u2580\u2580",13);
		strncpy(line2,"\u2588\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '7')
	{
		strncpy(line1,"\u2580\u2580\u2588",14);
		strncpy(line2,"  \u2588",8);
		strncpy(line3,"  \u2580",8);
	}
	else if(num == '8')
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2588\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '9')
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"  \u2580",8);
	}
	else if(num == '0')
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2588 \u2588",10);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(num == '.')
	{
		strncpy(line1,"   ",5);
		strncpy(line2,"   ",5);
		strncpy(line3," \u259D\u2598",11);
	}
}
