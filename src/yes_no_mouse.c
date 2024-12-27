#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/err.h"
#include "../lib/mouse.h"
#include "../lib/state.h"
#include "../lib/recall.h"
#include "../lib/scroll.h"
#include "../lib/recall.h"
#include "../lib/report.h"

/*
 * Take action if user presses 'yes' or delete window if press 'no'
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
		 * (Rename from 'order-' to 'void-'
		 */
		if(get_state("YES NO") == 1)
		{
			char command[250];
			char name[50];
			name[0] = '\0';
			char path[100];
			/*
			 * Begin making system rename command 'mv file1 file2'
			 */
			strncpy(command,"mv ",4);
			/*
			 * Add date path (mv order_path/year/month/day/)
			 */
			get_recall_date(path);
			strncat(command,path,strlen(path) + 1);
			/*
			 * Add desired order file and blank space
			 */
			append_order_recall(command);
			strncat(command," ",2);
			/*
			 * Add path again after space
			 * (mv order_path/year/month/day/order-num order_path/year/month/day/)
			 */
			strncat(command,path,strlen(path) + 1);
			/*
			 * Get desired order number and change 'order-' for 'void-'
			 */
			append_order_recall(name);
			char *num = name + 6;
			strncat(command,"void-",6);
			/*
			 * Add void-order_num to complete command
			 */
			strncat(command,num,strlen(num) + 1); 
			/*
			 * Execute command 
			 */
			system(command);
			/*
			 * Re-build order list and show updates
			 */
			del_recall_list();
			find_recall_list(path,1);
			write_recall();
			//gen_today();
		}
		/*
		 * Delete yes_no window and reset STATE
		 */
		del_yes_no_dialog();
		return;
	}
	/*
	 * If no is pressed
	 */
	if((y >= maxy) & (y <= maxy + 2) & (x >= maxx + 40) & (x <= maxx + 50))
	{
		/*
		 * Delete yes_no window and reset STATE
		 */
		del_yes_no_dialog();
		return;
	}
	
}
