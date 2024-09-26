#include <panel.h>
#include <string.h>

#include "../lib/draw_state_6.h"
#include "../lib/report.h"
#include "../lib/state.h"

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

WINDOW *printer_button[4];
PANEL *printer_buttonp[4];

/*
 * Draw return button for settings screen
 */
void draw_settings_windows(void)
{
	//del_settings_windows();
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
	
	/*
	 * Draw configure printer 2 box
	 */
	printer_button[3] = newwin(3,7,10,120);
	box(printer_button[3],0,0);
	printer_buttonp[3] = new_panel(printer_button[3]);
	mvwprintw(stdscr,9,121,"Configure grill printer");
	
	/*
	 * Draw installed printers window 
	 */
	printer_display = newwin(6,50,15,90);
	box(printer_display,0,0);
	printer_displayp = new_panel(printer_display);
	mvwprintw(stdscr,14,91,"Detected printers");
	/*
	 * Draw return to main menu button
	 */
	settings_return = newwin(3,15,38,5);
	box(settings_return,0,0);
	mvwprintw(settings_return,1,5,"RETURN");
	settings_returnp = new_panel(settings_return);
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
	for(int i = 0; i < 4; i++)
	{
		if(printer_button[i] != NULL)
		{
			del_panel(printer_buttonp[i]);
			printer_button[i] = NULL;
		}
	}
}
