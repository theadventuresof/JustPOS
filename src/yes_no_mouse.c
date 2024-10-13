#include <panel.h>
#include <unistd.h>

#include "../lib/err.h"
#include "../lib/mouse.h"
#include "../lib/state.h"

/*
 * 
 */
void find_mouse_yes_no(int y,int x)
{
	/*
	 * Find screen size
	 */
	int maxy,maxx;
	getmaxyx(stdscr,maxy,maxx);
	/*
	 * Make y equal to where yes and no are drawn
	 */
	maxy = (maxy/2) + 1;
	/*
	 * Make x equal to left hand corner of yes no winow
	 */
	maxx = (maxx/2) - 30;
	
	move(1,0);
	printw("%d/%d",maxy,maxx);
	doupdate();
	
	/*
	 * If yes is pressed 
	 */
	if((y >= maxy) & (y <= maxy + 2) & (x >= maxx + 10) & (x <= maxx + 20))
	{
		del_yes_no_dialog();
		usleep(100);
		return;
	}
	/*
	 * If no is pressed
	 */
	if((y >= maxy) & (y <= maxy + 2) & (x >= maxx + 40) & (x <= maxx + 50))
	{
		move(0,0);
		del_yes_no_dialog();
		usleep(100);
		return;
	}
	
}
