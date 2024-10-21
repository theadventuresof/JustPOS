#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/keypad.h"
#include "../lib/misc.h"

/*
 * Keep track of where keypad is on the screen as well as what to do
 * with the input from keypad
 */
struct kpd{
	int keypad_state;
	int keypad_func;
	char keypad_val[10];
}kpd;

WINDOW *keypad_win;
WINDOW *keypad_display;
WINDOW *keypad_nums[12];
WINDOW *keypad_cancel;
WINDOW *keypad_clr;
WINDOW *keypad_ok;

PANEL *keypad_winp;
PANEL *keypad_displayp;
PANEL *keypad_numsp[12];
PANEL *keypad_cancelp;
PANEL *keypad_clrp;
PANEL *keypad_okp;

/*
 * Draw keypad to screen according to STATE
 */
void draw_keypad(char ref[])
{
	delete_keypad();
	int y,x,startx;
	getmaxyx(stdscr,y,x);
	if(strcmp(ref,"CENTER") == 0)
	{
		startx = x/2 - (62/2);
		set_keypad_state("STATE",1);
	}
	else if(strcmp(ref,"LEFT") == 0)
	{
		//startx = x - 62;
		startx = 5;
		set_keypad_state("STATE",2);
	}
	
	/*
	 * Main keypad window. This is a large window that will encapsulate
	 * all the other keypad windows.
	 */
	if(get_keypad_state("STATE") == 1)
	{
		keypad_win = newwin(30,62,(y/2) - 15,startx);
	}
	else if(get_keypad_state("STATE") == 2)
	{
		keypad_win = newwin(30,62,6,startx);
	}
	box(keypad_win,0,0);
	keypad_winp = new_panel(keypad_win);
	
	/*
	 * Keypad display window. This window shows the user what value 
	 * they input
	 */
	 keypad_display = derwin(keypad_win,4,54,2,4);
	 wbkgd(keypad_display,COLOR_PAIR(1));
	 keypad_displayp = new_panel(keypad_display);
	 
	 /*
	  * Use a loop to draw twelve number buttons to the keypad
	  */
	  int i;
	  for(i = 0; i < 12; i++)
	  {
		  int row,col;
		  if(i <= 2)
		  {
			  row = 7;
			  col = (i * 14) + 4;
			  keypad_nums[i] = derwin(keypad_win,4,8,row,col);
			  box(keypad_nums[i],' ',' ');
			  mvwprintw(keypad_nums[i],2,4,"%d",i+1);
		  }
		  else if(i <= 5)
		  {
			  row = 13;
			  col = ((i-3) * 14) + 4;
			  keypad_nums[i] = derwin(keypad_win,4,8,row,col);
			  box(keypad_nums[i],' ',' ');
			  mvwprintw(keypad_nums[i],2,4,"%d",i+1);
		  }
		  else if(i <= 8)
		  {
			  row = 19;
			  col = ((i-6) * 14) + 4;
			  keypad_nums[i] = derwin(keypad_win,4,8,row,col);
			  box(keypad_nums[i],' ',' ');
			  mvwprintw(keypad_nums[i],2,4,"%d",i+1);
		  }
		  else if(i <= 11)
		  {
			  row = 25;
			  col = ((i-9) * 14) + 4;
			  keypad_nums[i] = derwin(keypad_win,4,8,row,col);
			  box(keypad_nums[i],' ',' ');
		  }
	  }
	 
	  mvwprintw(keypad_nums[9],2,4,".");
	  mvwprintw(keypad_nums[10],2,4,"0");
	  mvwprintw(keypad_nums[11],2,3,"DEL");
	  
	/*
    * Keypad cancel button
    */
    keypad_cancel = derwin(keypad_win,10,8,7,50);
    box(keypad_cancel,' ',' ');
	mvwprintw(keypad_cancel,2,4,"C\n    A\n    N\n    C\n    E\n    L");
	keypad_cancelp = new_panel(keypad_cancel);
	   
    /*
	* Clear keypad button
    */
	keypad_clr = derwin(keypad_win,4,8,19,50);
	box(keypad_clr,' ',' ');
	mvwprintw(keypad_clr,2,3,"CLR");
	keypad_clrp = new_panel(keypad_clr);
	
	/*
	 * Keypad OK button
	 */
	keypad_ok = derwin(keypad_win,4,8,25,50);
	box(keypad_ok,' ',' ');
	mvwprintw(keypad_ok,2,3,"OK");
	keypad_okp = new_panel(keypad_ok);
	
	update_panels(); 
	doupdate();
}

/*
 * Delete panels and windows related to keypad from memory
 */
void delete_keypad(void)
{
	clear_keypad();
	if(keypad_win != NULL)
	{
		del_panel(keypad_winp);
		keypad_win = NULL;
	}
	if(keypad_display != NULL)
	{
		del_panel(keypad_displayp);
		keypad_display = NULL;
	}
	if(keypad_cancel != NULL)
	{
		del_panel(keypad_cancelp);
		keypad_cancel = NULL;
	}
	if(keypad_ok != NULL)
	{
		del_panel(keypad_okp);
		keypad_ok = NULL;
	}
	if(keypad_clr != NULL)
	{
		del_panel(keypad_clrp);
		keypad_clr = NULL;
	}
	int i;
	for(i = 0; i < 12; i++)
	{
		if(keypad_nums[i])
		{
			del_panel(keypad_numsp[i]);
			keypad_nums[i] = NULL;
		}
	}	
	update_panels();
	doupdate();
}

/*
 * Set keypad state and function
 */
void set_keypad_state(char ref[],int val)
{
	if(strcmp("STATE",ref) == 0)
	{
		kpd.keypad_state = val;
	}
	else if(strcmp("FUNC",ref) == 0)
	{
		kpd.keypad_func = val;
	}
}

/*
 * Get keypad state and function
 */
int get_keypad_state(char ref[])
{
	int val;
	if(strcmp("STATE",ref) == 0)
	{
		val = kpd.keypad_state;
	}
	else if(strcmp("FUNC",ref) == 0)
	{
		val = kpd.keypad_func;
	}
	return val;
}

/*
 * Add a number or decimal to keypad_val
 */
void set_keypad_val(char val[1])
{
	/*
	 * Don't accept more than 10 digits (including after decimal if 
	 * applicable) 
	 */
	if(strlen(kpd.keypad_val) < 10)
	{
		/*
		 * Don't allow two decimals 
		 */
		if((has_decimal() == 1) & (val[0] == '.'))
		{
			return;
		}
		/*
		 * If decimal is present, check to see if total string length is 
		 * greater than the index of the decimal + 2 spaces
		 * 
		 * TLDR don't allow more than two digits after decimal
		 */
		if(has_decimal() == 1)
		{
			if(strlen(kpd.keypad_val) > (decimal_check(kpd.keypad_val,'.')+2))
			{
				return;
			} 
		}
		/*
		 * Only allow two digits when setting number of copies from
		 * settings menu
		 */
		if((get_keypad_state("FUNC") == 5) | (get_keypad_state("FUNC") == 6))
		{
			if(strlen(kpd.keypad_val) + 1 > 2)
			{
				return;
			} 
		}
		strncat(kpd.keypad_val,val,2);
	}
	/*
	 * Show updates
	 */
	write_keypad_val();
	update_panels();
	doupdate();
}

/*
 * Copy specific dollar amount to keypad -- for cash buttons on payment screen 
 */
void copy_to_keypad(char val[])
{
	clear_keypad();
	strncpy(kpd.keypad_val,val,strlen(val) + 1);
	write_keypad_val();
	update_panels();
	doupdate();
}

/*
 * Write keypad_val to keypad_display
 */
void write_keypad_val(void)
{
	/*
	 * Declare some strings to hold unicode chars (Each char is more than one byte)
	 */
	char line1[50];
	char line2[50];
	char line3[50];
	/*
	 * Each unicode block is 3x3, so we need to multiply strlen by 3 for positioning
	 */
	int x = (strlen(kpd.keypad_val) + 1)*4;
	for(int i = 0; i < strlen(kpd.keypad_val);i++)
	{
		char num;
		num = kpd.keypad_val[i];
		draw_number(num,line1,line2,line3);
		mvwprintw(keypad_display,1,(54-x)+(i*4),"%s",line1);
		mvwprintw(keypad_display,2,(54-x)+(i*4),"%s",line2);
		mvwprintw(keypad_display,3,(54-x)+(i*4),"%s",line3);
	}
	update_panels();
	doupdate();
}

/*
 *Check for decimal '.'
 */
int has_decimal(void)
{
	int val=0;
	if(strstr(kpd.keypad_val,".") > 0)
	{
		val = 1;
	}
	return val;
}

/*
 * Find index of specified char in string -1 = not found
 */
int decimal_check(char target[],char ref)
{
	int index;
	char *c;
	
	c = strchr(target,ref);
	if(c)
	{
		index = (int)(c - target);
	}
	else{
		index = -1;
	}
	return index;
}

/*
 * Clear keypad input and reset keypad_val
 */
void clear_keypad(void)
{
	memset(kpd.keypad_val,0,sizeof(kpd.keypad_val));
	werase(keypad_display);
	update_panels();
	doupdate();
}

/*
 * If there is one or more digit in keypad_val, remove rightmost digit
 */
void keypad_back(void)
{
	if(strlen(kpd.keypad_val) > 0)
	{
		kpd.keypad_val[strlen(kpd.keypad_val) -1] = '\0';
	}
	werase(keypad_display);
	write_keypad_val();
}

/*
 * Retrieve keypad data
 */
float get_keypad_data(void)
{
	float data = strtof(kpd.keypad_val,NULL);
	return data;
}
