#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/err.h"
#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/recall.h"
#include "../lib/scroll.h"
#include "../lib/recall.h"

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
	/*
	 * If yes is pressed 
	 */
	if((y >= maxy) & (y <= maxy + 2) & (x >= maxx + 10) & (x <= maxx + 20))
	{
		/*
		 * If an order is to be voided (from recall menu)
		 */
		if(get_state("YES NO") == 1)
		{
			char command[250];
			char name[50];
			name[0] = '\0';
			char path[100];
			strncpy(command,"mv ",4);
			get_recall_date(path);
			strncat(command,path,strlen(path) + 1);
			append_order_recall(command);
			strncat(command," ",2);
			strncat(command,path,strlen(path) + 1);
			append_order_recall(name);
			char *num = name + 6;
			strncat(command,"void-",6);
			strncat(command,num,strlen(num) + 1); 
			system(command);
			del_recall_list();
			find_recall_list(path,1);
			//populate_recall_list(path);
			write_recall();
		}
		del_yes_no_dialog();
		return;
	}
	/*
	 * If no is pressed
	 */
	if((y >= maxy) & (y <= maxy + 2) & (x >= maxx + 40) & (x <= maxx + 50))
	{
		del_yes_no_dialog();
		return;
	}
	
}
