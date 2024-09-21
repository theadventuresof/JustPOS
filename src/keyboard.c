#include <panel.h>
#include <string.h>

#include "../lib/keyboard.h"

struct keys{
	char msg[80];
	int keyboard_func;
	int keyboard_state;
}keys;

WINDOW *keyboard_win;
WINDOW *keyboard_display;
WINDOW *keyboard_close;
WINDOW *keyboard_space;
WINDOW *keyboard_backspace;
WINDOW *keyboard_clear;
WINDOW *keyboard_done;
WINDOW *keyboard_symbols;
WINDOW *keyboard_case;
WINDOW *keyboard_keys[30];

PANEL *keyboard_winp;
PANEL *keyboard_displayp;
PANEL *keyboard_closep;
PANEL *keyboard_spacep;
PANEL *keyboard_backspacep;
PANEL *keyboard_clearp;
PANEL *keyboard_donep;
PANEL *keyboard_symbolsp;
PANEL *keyboard_casep;
PANEL *keyboard_keysp[30];

/*
 * Draw windows for keyboard -- create keyboard skeleton
 */
void draw_keyboard(void)
{
	int y,x,i;
	getmaxyx(stdscr,y,x);
	
	/*
	 * Main keyboard window
	 */
	keyboard_win = newwin(y/2,x,y/2,0);
	box(keyboard_win,0,0);
	keyboard_winp = new_panel(keyboard_win);
	
	/*
	 * Keyboard close window 
	 */
	 keyboard_close = derwin(keyboard_win,3,7,1,x - 8);
	 box(keyboard_close,' ',' ');
	 mvwprintw(keyboard_close,1,1,"Close");
	 keyboard_closep = new_panel(keyboard_close);
	
	/*
	 * Keypad displkay window
	 */
	keyboard_display = derwin(keyboard_win,3,146,1,3);
	keyboard_displayp = new_panel(keyboard_display);	
	
	/*
	 * Keyboard spacebar
	 */
	keyboard_space = derwin(keyboard_win,3,40,16,(x/2)-20);
	box(keyboard_space,' ',' ');
	mvwprintw(keyboard_space,1,17,"SPACE");
	keyboard_spacep = new_panel(keyboard_space);
	
	/*
	 * Keyboard symbols (Button to switch keyboard states)
	 */
	keyboard_symbols = derwin(keyboard_win,3,10,16,(x/2)-50);
	box(keyboard_symbols,' ',' ');
	keyboard_symbolsp = new_panel(keyboard_symbols);
	if(get_keyboard("STATE") == 1)
	{
		mvwprintw(keyboard_symbols,1,4,"123");
	}
	else if(get_keyboard("STATE") == 2)
	{
		mvwprintw(keyboard_symbols,1,4,"ABC");
	}
	
	/*
	 * 
	 */
	keyboard_case = derwin(keyboard_win,3,6,16,(x/2)-60);
	box(keyboard_case,' ',' ');
	keyboard_casep = new_panel(keyboard_case);
	if(get_keyboard("STATE") == 1)
	{
		mvwprintw(keyboard_case,0,3,"|");
		mvwprintw(keyboard_case,1,3,"v");
	}
	else if(get_keyboard("STATE") == 3)
	{
		mvwprintw(keyboard_case,0,3,"^");
		mvwprintw(keyboard_case,1,3,"|");
	}
	
	/*
	 * Keyboard backspace
	 */
	keyboard_backspace = derwin(keyboard_win,3,15,9,140);
	box(keyboard_backspace,' ',' ');
	mvwprintw(keyboard_backspace,1,1,"<- Backspace");
	keyboard_backspacep = new_panel(keyboard_backspace);
	
	/*
	 * Keyboard clear
	 */
	keyboard_clear = derwin(keyboard_win,3,15,13,140);
	box(keyboard_clear,' ',' ');
	mvwprintw(keyboard_clear,1,3,"Clear Kbd");
	keyboard_clearp = new_panel(keyboard_clear);
	
	/*
	 * Keyboard done
	 */
	 keyboard_done = derwin(keyboard_win,3,15,17,140);
	 box(keyboard_done,' ',' ');
	 mvwprintw(keyboard_done,1,5,"Done");
	 keyboard_donep = new_panel(keyboard_done);
	
	/*
	 * Keyboard keys
	 */
	for(i = 0; i < 27; i++)
	{
		int start_x,start_y;
		if(i <= 9)
		{
			start_y = 4;
			start_x = i * 15 + 5;
			keyboard_keys[i] = derwin(keyboard_win,4,8,start_y,start_x);
			box(keyboard_keys[i],' ',' ');
			keyboard_keysp[i] = new_panel(keyboard_keys[i]);
		}
		else if(i <= 18)
		{
			start_y = 8;
			start_x = ((i - 10) * 15) + 8;
			keyboard_keys[i] = derwin(keyboard_win,4,8,start_y,start_x);
			box(keyboard_keys[i],' ',' ');
			keyboard_keysp[i] = new_panel(keyboard_keys[i]);
		}
		else if(i <= 25)
		{
			start_y = 12;
			start_x = ((i - 19) * 15) + 11;
			keyboard_keys[i] = derwin(keyboard_win,4,8,start_y,start_x);
			box(keyboard_keys[i],' ',' ');
			keyboard_keysp[i] = new_panel(keyboard_keys[i]);
		}
	}
	write_keys();
	if(strlen(keys.msg) > 0)
	{
		wattron(keyboard_display,A_BOLD);
		mvwprintw(keyboard_display,1,1,"%s",keys.msg);
		wattron(keyboard_display,A_BLINK);
		mvwprintw(keyboard_display,1,strlen(keys.msg)+1,"_");
		wattroff(keyboard_display,A_BOLD);
		wattroff(keyboard_display,A_BLINK);
	}
	update_panels();
	doupdate();
}

/*
 * Display value for all windows in keyboard that represent different keys
 */
void write_keys(void)
{
	if(get_keyboard("STATE") == 1)
	{
		mvwprintw(keyboard_keys[0],2,4,"Q");
		mvwprintw(keyboard_keys[1],2,4,"W");
		mvwprintw(keyboard_keys[2],2,4,"E");
		mvwprintw(keyboard_keys[3],2,4,"R");
		mvwprintw(keyboard_keys[4],2,4,"T");
		mvwprintw(keyboard_keys[5],2,4,"Y");
		mvwprintw(keyboard_keys[6],2,4,"U");
		mvwprintw(keyboard_keys[7],2,4,"I");
		mvwprintw(keyboard_keys[8],2,4,"O");
		mvwprintw(keyboard_keys[9],2,4,"P");
		mvwprintw(keyboard_keys[10],2,4,"A");
		mvwprintw(keyboard_keys[11],2,4,"S");
		mvwprintw(keyboard_keys[12],2,4,"D");
		mvwprintw(keyboard_keys[13],2,4,"F");
		mvwprintw(keyboard_keys[14],2,4,"G");
		mvwprintw(keyboard_keys[15],2,4,"H");
		mvwprintw(keyboard_keys[16],2,4,"J");
		mvwprintw(keyboard_keys[17],2,4,"K");
		mvwprintw(keyboard_keys[18],2,4,"L");
		mvwprintw(keyboard_keys[19],2,4,"Z");
		mvwprintw(keyboard_keys[20],2,4,"X");
		mvwprintw(keyboard_keys[21],2,4,"C");
		mvwprintw(keyboard_keys[22],2,4,"V");
		mvwprintw(keyboard_keys[23],2,4,"B");
		mvwprintw(keyboard_keys[24],2,4,"N");
		mvwprintw(keyboard_keys[25],2,4,"M");
		mvwprintw(keyboard_symbols,1,4,"123");
		mvwprintw(keyboard_case,0,3,"|");
		mvwprintw(keyboard_case,1,3,"v");
	}
	else if(get_keyboard("STATE") == 2)
	{
		mvwprintw(keyboard_keys[0],2,4,"1");
		mvwprintw(keyboard_keys[1],2,4,"2");
		mvwprintw(keyboard_keys[2],2,4,"3");
		mvwprintw(keyboard_keys[3],2,4,"4");
		mvwprintw(keyboard_keys[4],2,4,"5");
		mvwprintw(keyboard_keys[5],2,4,"6");
		mvwprintw(keyboard_keys[6],2,4,"7");
		mvwprintw(keyboard_keys[7],2,4,"8");
		mvwprintw(keyboard_keys[8],2,4,"9");
		mvwprintw(keyboard_keys[9],2,4,"0");
		mvwprintw(keyboard_keys[10],2,4,"-");
		mvwprintw(keyboard_keys[11],2,4,"/");
		mvwprintw(keyboard_keys[12],2,4,":");
		mvwprintw(keyboard_keys[13],2,4,"_");
		mvwprintw(keyboard_keys[14],2,4,"(");
		mvwprintw(keyboard_keys[15],2,4,")");
		mvwprintw(keyboard_keys[16],2,4,"$");
		mvwprintw(keyboard_keys[17],2,4,"&");
		mvwprintw(keyboard_keys[18],2,4,"@");
		mvwprintw(keyboard_keys[19],2,4,"!");
		mvwprintw(keyboard_keys[20],2,4,"#");
		mvwprintw(keyboard_keys[21],2,4,"?");
		mvwprintw(keyboard_keys[22],2,4,"*");
		mvwprintw(keyboard_keys[23],2,4,"+");
		mvwprintw(keyboard_keys[24],2,4,".");
		mvwprintw(keyboard_keys[25],2,4,"^");
		mvwprintw(keyboard_symbols,1,4,"ABC");
	}
	else if(get_keyboard("STATE") == 3)
	{
		mvwprintw(keyboard_keys[0],2,4,"q");
		mvwprintw(keyboard_keys[1],2,4,"w");
		mvwprintw(keyboard_keys[2],2,4,"e");
		mvwprintw(keyboard_keys[3],2,4,"r");
		mvwprintw(keyboard_keys[4],2,4,"t");
		mvwprintw(keyboard_keys[5],2,4,"y");
		mvwprintw(keyboard_keys[6],2,4,"u");
		mvwprintw(keyboard_keys[7],2,4,"i");
		mvwprintw(keyboard_keys[8],2,4,"o");
		mvwprintw(keyboard_keys[9],2,4,"p");
		mvwprintw(keyboard_keys[10],2,4,"a");
		mvwprintw(keyboard_keys[11],2,4,"s");
		mvwprintw(keyboard_keys[12],2,4,"d");
		mvwprintw(keyboard_keys[13],2,4,"f");
		mvwprintw(keyboard_keys[14],2,4,"g");
		mvwprintw(keyboard_keys[15],2,4,"h");
		mvwprintw(keyboard_keys[16],2,4,"j");
		mvwprintw(keyboard_keys[17],2,4,"k");
		mvwprintw(keyboard_keys[18],2,4,"l");
		mvwprintw(keyboard_keys[19],2,4,"z");
		mvwprintw(keyboard_keys[20],2,4,"x");
		mvwprintw(keyboard_keys[21],2,4,"c");
		mvwprintw(keyboard_keys[22],2,4,"v");
		mvwprintw(keyboard_keys[23],2,4,"b");
		mvwprintw(keyboard_keys[24],2,4,"n");
		mvwprintw(keyboard_keys[25],2,4,"m");
		mvwprintw(keyboard_case,0,3,"^");
		mvwprintw(keyboard_case,1,3,"|");
	}
	if(strlen(keys.msg) == 0)
	{
		wattron(keyboard_display,A_BOLD);
		wattron(keyboard_display,A_BLINK);
		mvwprintw(keyboard_display,1,1,"_");
		wattroff(keyboard_display,A_BOLD);
		wattroff(keyboard_display,A_BLINK);
	}
	update_panels();
	doupdate();
}


/*
 * Delete all windows and panels associated with keyboard from memory
 */
void del_keyboard(void)
{
	int i;
	if(keyboard_win != NULL)
	{
		del_panel(keyboard_winp);
		delwin(keyboard_win);
		del_panel(keyboard_displayp);
		delwin(keyboard_display);
		del_panel(keyboard_closep);
		delwin(keyboard_close);
		del_panel(keyboard_spacep);
		delwin(keyboard_space);
		del_panel(keyboard_backspacep);
		delwin(keyboard_backspace);
		del_panel(keyboard_clearp);
		delwin(keyboard_clear);
		del_panel(keyboard_donep);
		delwin(keyboard_done);
		del_panel(keyboard_symbolsp);
		delwin(keyboard_symbols);
		del_panel(keyboard_casep);
		delwin(keyboard_case);
		for(i = 0; i < 27; i++)
		{
			del_panel(keyboard_keysp[i]);
			delwin(keyboard_keys[i]);
		}
	}
	set_keyboard("STATE",1);
	update_panels();
	doupdate();
}

/*
 * Add character to keyboard_msg string
 */
void add_key(char key[])
{
	/*
	 * Limit special instruction messages to 25 char
	 */
	if(get_keyboard("FUNC") == 1)
	{
		if(strlen(keys.msg) < 24)
		{
			strncat(keys.msg,key,2);
		}
	}
	if((get_keyboard("FUNC") == 2) | (get_keyboard("FUNC") == 5))
	{
		if(strlen(keys.msg) < 49)
		{
			strncat(keys.msg,key,2);
		}
	}
	if((get_keyboard("FUNC") == 3) | (get_keyboard("FUNC") == 4))
	{
		if(strlen(keys.msg) < 99)
		{
			strncat(keys.msg,key,2);
		}
	}
	/*
	 * Preserve formatting, and draw blinking underscore
	 */
	wattron(keyboard_display,A_BOLD);
	mvwprintw(keyboard_display,1,1,"%s",keys.msg);
	wattron(keyboard_display,A_BLINK);
	mvwprintw(keyboard_display,1,strlen(keys.msg)+1,"_");
	wattroff(keyboard_display,A_BLINK);
	wattroff(keyboard_display,A_BOLD);
	update_panels();
	doupdate();
}

/*
 * Completely clear keyboard message
 */
void clear_keyboard(void)
{
	memset(keys.msg,0,sizeof(keys.msg));
	werase(keyboard_display);
	wattron(keyboard_display,A_BOLD);
	wattron(keyboard_display,A_BLINK);
	mvwprintw(keyboard_display,1,1,"_");
	wattroff(keyboard_display,A_BOLD);
	wattroff(keyboard_display,A_BLINK);
	update_panels();
	doupdate();
}

/*
 * Remove last character from keyboard message, draw blinking underscore
 * to directly after keyboard message
 */
void backspace_keyboard(void)
{
	/*
	 * If there are more than 0 characters in keyboard_msg, delete last
	 * character from string
	 */
	if(strlen(keys.msg) > 0)
	{
		keys.msg[strlen(keys.msg)-1] = '\0';
	}
	/*
	 * Preserve formatting and write blinking underscore
	 */
	werase(keyboard_display);
	wattron(keyboard_display,A_BOLD);
	mvwprintw(keyboard_display,1,1,"%s",keys.msg);
	wattron(keyboard_display,A_BLINK);
	mvwprintw(keyboard_display,1,strlen(keys.msg)+1,"_");
	wattroff(keyboard_display,A_BLINK);
	wattroff(keyboard_display,A_BOLD);
	update_panels();
	doupdate();
}

/*
 * Set keyboard function -- decide what to do with keyboard input
 */
void set_keyboard(char ref[],int val)
{
	if(strcmp(ref,"FUNC") == 0)
	{
		keys.keyboard_func = val;
	}
	else if(strcmp(ref,"STATE") == 0)
	{
		keys.keyboard_state = val;
	}
}

/*
 * Find out what to do with keyboard input
 */
int get_keyboard(char ref[])
{
	int val;
	if(strcmp(ref,"FUNC") == 0)
	{
		val = keys.keyboard_func;
	}
	else if(strcmp(ref,"STATE") == 0)
	{
		val = keys.keyboard_state;
	}
	return val;
}

/*
 * Copy keyboard message to external char*
 */
void copy_keyboard_val(char *val)
{
	strncpy(val,keys.msg,strlen(keys.msg)+1);
}
