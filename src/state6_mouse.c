#include <panel.h>
#include <string.h>

#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/keyboard.h"
#include "../lib/draw_state_6.h"
#include "../lib/scroll.h"
#include "../lib/print.h"
#include "../lib/file.h"
#include "../lib/err.h"

void find_mouse_settings(int y,int x)
{
	/*
	 * If order_dir window is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 10) & ( x <= 60))
	{	
		draw_keyboard();
		set_keyboard("FUNC",3);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
	/*
	 * If conf_msg window is pressed
	 */
	if((y >= 11) & (y <= 16) & (x >= 10) & (x <= 60))
	{
		draw_keyboard();
		set_keyboard("FUNC",4);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
	/*
	 * If phone/social window is pressed
	 */
	if((y >= 20) & (y <= 22) & (x >= 10) & (x <= 60))
	{
		draw_keyboard();
		set_keyboard("FUNC",5);
		set_state("STATE",3);
		set_state("PREV_STATE",6);
	}
	/*
	 * If return is pressed
	 */
	if((y >= 38) & (y <= 40) & (x >= 6) & (x <= 20))
	{
		erase();
		del_state(6);
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		set_printerdex("LINE",-1);
		draw_state(1);
	}
	/*
	 * If enable printer_1 is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 90) & (x <= 96))
	{
		if(get_state("PRINTER_1") == 1)
		{
			set_state("PRINTER_1",0);
		}
		else if(get_state("PRINTER_1") == 0)
		{
			set_state("PRINTER_1",1);
		}
		draw_settings_windows();
	}
	/*
	 * If enable printer_2 is pressed
	 */
	if((y >= 5) & (y <= 7) & (x >= 121) & (x <= 127))
	{
		if(get_state("PRINTER_2") == 1)
		{
			set_state("PRINTER_2",0);
		}
		else if(get_state("PRINTER_2") == 0)
		{
			set_state("PRINTER_2",1);
		}
		draw_settings_windows();
	}
	/*
	 * If configure printer_1 is pressed
	 */
	if((y >= 10) & (y <= 12) & (x >= 90) & (x <= 96))
	{
		if(get_state("WHICH_PRINTER") == 1)
		{
			set_state("WHICH_PRINTER",0);
		}
		else{
			set_state("WHICH_PRINTER",1);
		}
		set_printerdex("LINE",-1);
		draw_settings_windows();
	}
	/*
	 * If configure printer_2 is pressed
	 */
	if((y >= 10) & (y <= 12) & (x >= 121) & (x <= 127))
	{
		if(get_state("WHICH_PRINTER") == 2)
		{
			set_state("WHICH_PRINTER",0);
		}
		else{
			set_state("WHICH_PRINTER",2);
		}
		set_printerdex("LINE",-1);
		draw_settings_windows();
	}
	/*
	 * If select printer button is pressed
	 */
	if((y >= 23) & (y <= 25) & (x >= 110) & (x <= 123))
	{
		if((get_state("WHICH_PRINTER") == 0) | (get_printerdex("LINE") == -1))
		{
			return;
		}
		char full_line[100];
		char device[100];
		get_printer_device(device);
		if(get_state("WHICH_PRINTER") == 1)
		{
			strncpy(full_line,"printer1=",10);
		}
		else if(get_state("WHICH_PRINTER") == 2)
		{
			strncpy(full_line,"printer2=",10);
		}
		strncat(full_line,device,strlen(device)+1);
		if(get_state("WHICH_PRINTER") == 1)
		{
			change_conf_line("printer1=",full_line);
		}
		else if(get_state("WHICH_PRINTER") == 2)
		{
			change_conf_line("printer2=",full_line);
		}
		set_printerdex("LINE",-1);
		write_printers();
	}
}
