#include <panel.h>

#include "../lib/draw_state_9.h"

WINDOW *report_win;
WINDOW *report_background;

PANEL *report_winp;
PANEL *report_backgroundp;

/*
 * Draw a master window to display the report information/selections
 */
void draw_report_win(void)
{
	del_report_win();
	
	report_win = newwin(28,40,6,50);
	report_winp = new_panel(report_win);
	report_background = newwin(30,42,5,49);
	report_backgroundp = new_panel(report_background);
	box(report_background,0,0);
	
	update_panels();
	doupdate();
}

/*
 * Delete master report window when not in use
 */
void del_report_win(void)
{
	if(report_win != NULL)
	{
		del_panel(report_winp);
		report_win = NULL;
	}
	if(report_background != NULL)
	{
		del_panel(report_backgroundp);
		report_background = NULL;
	}
}

WINDOW *daily_report;
WINDOW *monthly_report;
WINDOW *yearly_report;
WINDOW *info_win;

PANEL *daily_reportp;
PANEL *monthly_reportp;
PANEL *yearly_reportp;
PANEL *info_winp;

/*
 * Draw some buttons to allow different reports to be selected
 */
void draw_report_buttons(void)
{
	del_report_buttons();
	
	daily_report = newwin(3,20,12,10);
	box(daily_report,0,0);
	daily_reportp = new_panel(daily_report);
	mvwprintw(daily_report,1,4,"DAILY REPORT");
	monthly_report = newwin(3,20,16,10);
	box(monthly_report,0,0);
	monthly_reportp = new_panel(monthly_report);
	mvwprintw(monthly_report,1,4,"MONTH REPORT");
	yearly_report = newwin(3,20,20,10);
	box(yearly_report,0,0);
	yearly_reportp = new_panel(yearly_report);
	mvwprintw(yearly_report,1,4,"YEARLY REPORT");
	info_win = newwin(3,42,2,49);
	info_winp = new_panel(info_win);
	mvwprintw(info_win,2,1,"TEST");
	update_panels();
	doupdate();
}

/*
 * Delete report selection buttons when not in use
 */
void del_report_buttons(void)
{
	if(daily_report != NULL)
	{
		del_panel(daily_reportp);
		daily_report = NULL;
	}
	if(monthly_report != NULL)
	{
		del_panel(monthly_reportp);
		monthly_report = NULL;
	}
	if(yearly_report != NULL)
	{
		del_panel(yearly_reportp);
		yearly_report = NULL;
	}
	if(info_win != NULL)
	{
		del_panel(info_winp);
		info_win = NULL;
	}

}
