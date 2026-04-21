#include <panel.h>

#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/report.h"
#include "../lib/file.h"
#include "../lib/err.h"

void find_mouse_report_menu(int y,int x)
{
	/*
	 * If return button is pressed
	 */
	if((y >= 38) & (y <= 40) & (x >= 5) & (x <= 18))
	{
		del_state(9);
		set_state("STATE",1);
		set_state("PREV_STATE",1);
		draw_state(1);
		return;
	}
	/*
	 * If Daily Report button is pressed
	 */
	if((y >= 12) & (y <= 14) & (x >= 9) & (x <= 27))
	{
		char today[100];
		get_dir_date(today);
		gen_daily(today);
		err_dialog("CHECK FOR GENERATED REPORTS");
		return;
	}
}
