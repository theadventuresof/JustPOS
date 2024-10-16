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
				set_state("CUR_CELL",itm_num-1);
				return;
			}
			else if(i == 2)
			{
				set_index("DRINK",itm_num/12 + 1);
				write_to_menu_buttons();
				write_pages();
				itm_num = (itm_num - (12 * (get_index("DRINK") - 1)));
				select_menu_button(itm_num);
				set_state("CUR_CELL",itm_num-1);
				return;
			}
			doupdate();
		} 
		else if(itm_num == 0)
		{
			//return;
			//write_to_menu_buttons();
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
 * Use unicode symbols to draw 3x3 block size numbers (for keypad)
 */
void draw_number(char num[],char *line1,char *line2,char *line3)
{
	if(strncmp(num,"1",1) == 0)
	{
		strncpy(line1," \u259D\u2588",13);
		strncpy(line2,"  \u2588",8);
		strncpy(line3,"  \u2588",13);
	}
	else if(strncmp(num,"2",1) == 0)
	{
		strncpy(line1,"\u2580\u2580\u2588",13);
		strncpy(line2,"\u2588\u2580\u2580",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(strncmp(num,"3",1) == 0)
	{
		strncpy(line1,"\u2580\u2580\u2588",13);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(strncmp(num,"4",1) == 0)
	{
		strncpy(line1,"\u2588 \u2588",10);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"  \u2588",8);
	}
	else if(strncmp(num,"5",1) == 0)
	{
		strncpy(line1,"\u2588\u2580\u2580",13);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(strncmp(num,"6",1) == 0)
	{
		strncpy(line1,"\u2588\u2580\u2580",13);
		strncpy(line2,"\u2588\u2580\u2588",13);
		strncpy(line3,"\u2580\u2580\u2580",13);
	}
	else if(strncmp(num,"7",1) == 0)
	{
		strncpy(line1,"\u2580\u2580\u2588",14);
		strncpy(line2,"  \u2588",8);
		strncpy(line3,"  \u2588",8);
	}
	else if(strncmp(num,"8",1) == 0)
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2588\u2583\u2588",13);
		strncpy(line3,"\u2588\u2583\u2588",13);
	}
	else if(strncmp(num,"9",1) == 0)
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2580\u2580\u2588",13);
		strncpy(line3,"  \u2588",8);
	}
	else if(strncmp(num,"0",1) == 0)
	{
		strncpy(line1,"\u2588\u2580\u2588",13);
		strncpy(line2,"\u2588 \u2588",8);
		strncpy(line3,"\u2588\u2583\u2588",13);
	}
	else if(strncmp(num,".",1) == 0)
	{
		strncpy(line3," \u2597\u2596",11);
	}
}
