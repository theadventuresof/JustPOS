#include <panel.h>
#include <string.h>

#include "../lib/draw_state_6.h"
#include "../lib/report.h"
#include "../lib/state.h"
#include "../lib/scroll.h"
#include "../lib/print.h"
#include "../lib/err.h"
#include "../lib/draw.h"

WINDOW *settings_return;
PANEL *settings_returnp;

WINDOW *order_dir;
PANEL *order_dirp;

WINDOW *conf_msg;
PANEL *conf_msgp;

WINDOW *phone;
PANEL *phonep;

WINDOW *printer_display;
PANEL *printer_displayp;

WINDOW *printer_win;
PANEL *printer_winp;

WINDOW *printer_button[4];
PANEL *printer_buttonp[4];

WINDOW *select_printer;
PANEL *select_printerp;

/*
 * Draw return button for settings screen
 */
void draw_settings_windows(void)
{
	del_settings_windows();
	/*
	 * Draw order directory field
	 */
	order_dir = newwin(3,50,5,10);
	box(order_dir,0,0);
	char orddir[100];
	get_file_data(".conf","dir=",orddir);
	mvwprintw(order_dir,1,2,"%s",orddir);
	mvwprintw(stdscr,4,11,"Parent directory for receipt storage");
	order_dirp = new_panel(order_dir);
	/*
	 * Draw .conf message field
	 */
	conf_msg = newwin(6,50,11,10);
	box(conf_msg,0,0);
	conf_msgp = new_panel(conf_msg);
	char msg[100];
	get_file_data(".conf","msg=",msg);
	mvwprintw(conf_msg,1,2,"%s",msg);
	mvwprintw(stdscr,10,11,"Message to appear on top of receipts");
	/*
	 * Draw phone number/social media handle
	 */
	phone = newwin(3,50,20,10);
	box(phone,0,0);
	phonep = new_panel(phone);
	char phone_num[50];
	get_file_data(".conf","contact=",phone_num);
	mvwprintw(phone,1,2,"%s",phone_num);
	mvwprintw(stdscr,19,11,"Phone number or social media handle for receipts");
	/*
	 * Draw enable printer1 button
	 */
	printer_button[0] = newwin(3,7,5,90);
	box(printer_button[0],0,0);
	printer_buttonp[0] = new_panel(printer_button[0]);
	mvwprintw(stdscr,4,91,"Enable counter printer");
	if(get_state("PRINTER_1") == 1)
	{
		wattron(printer_button[0],A_BOLD);
		mvwprintw(printer_button[0],1,3,"X");
		wattroff(printer_button[0],A_BOLD);
	}
	/*
	 * Draw enable printer2 button
	 */
	printer_button[1] = newwin(3,7,5,120);
	box(printer_button[1],0,0);
	printer_buttonp[1] = new_panel(printer_button[1]);
	mvwprintw(stdscr,4,121,"Enable grill printer");
	if(get_state("PRINTER_2") == 1)
	{
		wattron(printer_button[1],A_BOLD);
		mvwprintw(printer_button[1],1,3,"X");
		wattroff(printer_button[1],A_BOLD);
	}
	/*
	 * Draw configure printer 1 box
	 */
	printer_button[2] = newwin(3,7,10,90);
	box(printer_button[2],0,0);
	printer_buttonp[2] = new_panel(printer_button[2]);
	mvwprintw(stdscr,9,91,"Configure counter printer");
	if(get_state("WHICH_PRINTER") == 1)
	{
		wattron(printer_button[2],A_BOLD);
		mvwprintw(printer_button[2],1,3,"X");
		wattroff(printer_button[2],A_BOLD);
	}
	/*
	 * Draw configure printer 2 box
	 */
	printer_button[3] = newwin(3,7,10,120);
	box(printer_button[3],0,0);
	printer_buttonp[3] = new_panel(printer_button[3]);
	mvwprintw(stdscr,9,121,"Configure grill printer");
	if(get_state("WHICH_PRINTER") == 2)
	{
		wattron(printer_button[3],A_BOLD);
		mvwprintw(printer_button[3],1,3,"X");
		wattroff(printer_button[3],A_BOLD);
	}
	/*
	 * Draw installed printers window 
	 */
	printer_display = newwin(7,50,15,90);
	box(printer_display,0,0);
	printer_displayp = new_panel(printer_display);
	mvwprintw(stdscr,14,91,"Detected printers");
	/*
	 * Draw window to display printers
	 */
	printer_win = newwin(5,48,16,91);
	printer_winp = new_panel(printer_win);
	/*
	 * Draw select button
	 */
	select_printer = newwin(3,15,22,108);
	box(select_printer,0,0);
	mvwprintw(select_printer,1,5,"SELECT");
	select_printerp = new_panel(select_printer);
	/*
	 * Draw return to main menu button
	 */
	settings_return = newwin(3,15,38,5);
	box(settings_return,0,0);
	mvwprintw(settings_return,1,5,"RETURN");
	settings_returnp = new_panel(settings_return);
	write_printers();
	update_panels();
	doupdate();
}

/*
 * Copy blanks to each position of printer_win
 */
void clear_printer_win(void)
{
	werase(printer_win);
	
	update_panels();
	doupdate();
}

/*
 * Write detected printers to printer_win
 */
void write_to_printer_win(char device[],int format)
{
	/*
	 * Attempt to get printer name from .conf and store value in 
	 * current_printer
	 */
	char current_printer[100];
	/*
	 * Attempt to find counter printer
	 */
	if(get_state("WHICH_PRINTER") == 1)
	{
		get_file_data(".conf","printer1=",current_printer);
	}
	/*
	 * Attempt to find grill printer
	 */
	else if(get_state("WHICH_PRINTER") == 2)
	{
		get_file_data(".conf","printer2=",current_printer);
	}
	/*
	 * Remove trailing newline
	 */
	current_printer[strcspn(current_printer,"\n")] = 0;
	/*
	 * Check if printer name from .conf matches device name from 
	 * detected printer list
	 */
	if(strncmp(device,current_printer,strlen(current_printer)) == 0)
	{
		/*
		 * If current_printer is not blank and matches device from 
		 * detected printers list, draw it with blue highlighting
		 */
		if(strlen(current_printer) > 0)
		{
			wattron(printer_win,A_BOLD);
			wattron(printer_win,COLOR_PAIR(12));
		}
	}
	/*
	 * 
	 */
	if(get_printerdex("LINE") == get_printerdex("CURRENT"))
	{
		wattroff(printer_win,COLOR_PAIR(12));
		wattron(printer_win,COLOR_PAIR(1));
	}
	/*
	 * Here, we stick some blank spaces onto the end of the device name
	 * so that highlighting highlights to the end of the line and so that
	 * the next device will be drawn to the next line
	 */
	concat_blanks(48-strlen(device),device);
	/*
	 * set_printerdex("CURRENT",1) tells the system to add one to "CURRENT"
	 * this is used to track where we are in the list and if an item is 
	 * within the range of "MIN" and "MAX" visible on printer_win
	 */
	set_printerdex("CURRENT",1);
	if((get_printerdex("CURRENT") >= get_printerdex("MIN")) & (get_printerdex("CURRENT") < get_printerdex("MAX")))
	{
		/*
		 * Do actual writing/drawing
		 */
		wprintw(printer_win,"%s",device);
	}
	/*
	 * Turn off bold and color pairings, and show updates
	 */
	wattroff(printer_win,A_BOLD);
	wattroff(printer_win,COLOR_PAIR(12));
	wattroff(printer_win,COLOR_PAIR(1));
	update_panels();
	doupdate();
}

/*
 * Delete settings menu windows from memory
 */
void del_settings_windows(void)
{
	if(settings_return != NULL)
	{
		del_panel(settings_returnp);
		settings_return = NULL;
	}
	if(order_dir != NULL)
	{
		del_panel(order_dirp);
		order_dir = NULL;
	}
	if(conf_msg != NULL)
	{
		del_panel(conf_msgp);
		conf_msg = NULL;
	}
	if(phone != NULL)
	{
		del_panel(phonep);
		phone = NULL;
	}
	if(printer_display != NULL)
	{
		del_panel(printer_displayp);
		printer_display = NULL;
	}
	if(printer_win != NULL)
	{
		del_panel(printer_winp);
		printer_win = NULL;
	}
	if(select_printer != NULL)
	{
		del_panel(select_printerp);
		select_printer = NULL;
	}
	for(int i = 0; i < 4; i++)
	{
		if(printer_button[i] != NULL)
		{
			del_panel(printer_buttonp[i]);
			printer_button[i] = NULL;
		}
	}
}
