#include <stdio.h>
#include <string.h>

#include "../lib/scroll.h"
#include "../lib/order.h"
#include "../lib/recall.h"

/*
 * This structure tracks data relevant to scrolling the order
 */
struct scrolldex{
	int touch_location_y;
	int min_line;
	int max_line;
	int max_index;
	int current;
	int diff;
}scrolldex;


/*
 * Based on touch point -- using the difference in y axis, we decide
 * to scroll the order window (if possible) up or down.
 */
void scroll_order(int lines)
{
	/* lines is calculated by the difference in y axis between a touch
	 * event and current cursor (finger) position -- think dragging gesture
	 * 
	 * If lines is negative -- eg being dragged down
	 */
	if(lines < 0)
	{
		/*
		 * If at top of list, do nothing
		 */
		if(get_scrolldex("MIN") - 1 < 1)
		{
			return;
		}
		/*
		 * Scroll up one line
		 */
		else{
			set_scrolldex("MIN",get_scrolldex("MIN")-1);
			set_scrolldex("MAX",get_scrolldex("MAX")-1);
		}
		write_list();
	}
	/*
	 * If lines is positive -- eg being dragged up
	 */
	else if(lines > 0)
	{
		/*
		 * If at end of list, do nothing
		 */
		if(get_scrolldex("MAX") + 1 > get_scrolldex("MAX_LINE"))
		{
			return;
		}
		/*
		 * Scroll down one line
		 */
		else{
			set_scrolldex("MAX",get_scrolldex("MAX")+1);
			set_scrolldex("MIN",get_scrolldex("MIN")+1);
		}
		/*
		 * Show changes
		 */
		write_list();
	}
}

/*
 * Set values in scrolldex struct
 */
void set_scrolldex(char type[],int val)
{
	if(strcmp(type,"TOUCH") == 0)
	{
		scrolldex.touch_location_y = val;
	}
	else if(strcmp(type,"MIN") == 0)
	{
		scrolldex.min_line = val;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		scrolldex.max_line = val;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		scrolldex.max_index = val;
	}
	else if(strcmp(type,"DIFF") == 0)
	{
		scrolldex.diff = val;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		if(val == 0)
		{
			scrolldex.current = 0;
		}
		else if(val == 1)
		{
			scrolldex.current++;
		}
	}
}


/*
 * Get values from scrolldex struct 
 */
int get_scrolldex(char type[])
{
	int val;
	if(strcmp(type,"TOUCH") == 0)
	{
		val = scrolldex.touch_location_y;
	}
	else if(strcmp(type,"MIN") == 0)
	{
		val = scrolldex.min_line;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		val = scrolldex.max_line;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		val = scrolldex.max_index;
	}
	else if(strcmp(type,"DIFF") == 0)
	{
		val = scrolldex.diff;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		val = scrolldex.current;
	}
	return val;
}

/*
 * Jump to end of order
 */
void scroll_to_end(void)
{
	if(get_scrolldex("MAX_LINE") > 27)
	{
		set_scrolldex("MAX",get_scrolldex("MAX_LINE"));
		set_scrolldex("MIN",get_scrolldex("MAX")-27);
	}
}

/*
 * Jump to top of order
 */
void scroll_to_top(void)
{
	set_scrolldex("MIN",0);
	set_scrolldex("MAX",27);
}

/*
 * This structure tracks scrolling information for the recall window
 */
struct recalldex{
	int min_line;
	int max_line; 
	int max_index;
	int current;
	int state;
	int line;
	char date[100];
}recalldex;

/*
 * Set values in structure recalldex
 */
void set_recalldex(char type[],int val)
{
	if(strcmp(type,"MIN") == 0)
	{
		recalldex.min_line = val;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		recalldex.max_line = val;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		recalldex.max_index = val;
	}
	else if(strcmp(type,"STATE") == 0)
	{
		recalldex.state = val;
	}
	else if(strcmp(type,"LINE") == 0)
	{
		recalldex.line = val;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		if(val == 0)
		{
			recalldex.current = 0;
		}
		else if(val == 1)
		{
			recalldex.current++;
		}
	}
}


/*
 * Get relevent information from recalldex structure 
 */
int get_recalldex(char type[])
{
	int val;
	if(strcmp(type,"MIN") == 0)
	{
		val = recalldex.min_line;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		val = recalldex.max_line;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		val = recalldex.max_index;
	}
	else if(strcmp(type,"STATE") == 0)
	{
		val = recalldex.state;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		val = recalldex.current;
	}
	else if(strcmp(type,"LINE") == 0)
	{
		val = recalldex.line;
	}
	return val;
}


/*
 * Scroll lines in recall_win (if possible)
 */
void scroll_recall(int lines)
{
	/*
	 * If lines is negative, the dragging direction is down and the list
	 * moves toward the top
	 */
	if(lines < 0)
	{
		/*
		 * Return if beginning of list is reached.
		 */
		if(get_recalldex("MIN") - 1 < 0)
		{
			return;
		}
		/*
		 * Scroll recall_win up one line at a time
		 */
		else{
			set_recalldex("MIN",get_recalldex("MIN")-1);
			set_recalldex("MAX",get_recalldex("MAX")-1);
		}
		/*
		 * Show changes
		 */
		write_recall();
	}
	/*
	 * If lines is positive, the dragging direction is up and the list
	 * needs to move toward the end
	 */
	else if(lines > 0)
	{
		/*
		 * Return if end of list is reached
		 */
		if(get_recalldex("MAX") + 1 > get_recalldex("MAX_LINE"))
		{
			return;
		}
		/*
		 * Scroll list down by one line
		 */
		else{
			set_recalldex("MIN",get_recalldex("MIN")+1);
			set_recalldex("MAX",get_recalldex("MAX")+1);
		}
		/*
		 * Show changes
		 */
		write_recall();
	}
}


/*
 * Copy a file path to a specific date so that orders can be found from
 * multiple days and are displayed from the correct date.
 */
void set_recall_date(char date[])
{
	strncpy(recalldex.date,date,strlen(date) + 1);
}

/*
 * Copy recall date to external string
 */
void get_recall_date(char *date)
{
	strncpy(date,recalldex.date,strlen(recalldex.date) + 1);
}

/*
 * Set recall date to empty
 */
void reset_recall_date(void)
{
	recalldex.date[0] = '\0';
}

struct printerdex{
	int min_line;
	int max_line; 
	int max_index;
	int current;
	int line;
	char device[100];	
}printerdex;

/*
 * 
 */
int get_printerdex(char type[])
{
	int val=0;
	if(strcmp(type,"MIN") == 0)
	{
		val = printerdex.min_line;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		val = printerdex.max_line;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		val = printerdex.max_index;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		if(val == 0)
		{
			printerdex.current = 0;
		}
		else if(val == 1)
		{
			printerdex.current++;
		}
	}
	else if(strcmp(type,"LINE") == 0)
	{
		val = printerdex.line;
	}
	return val;
}

/*
 * 
 */
void set_printerdex(char type[],int val)
{
	if(strcmp(type,"MIN") == 0)
	{
		printerdex.min_line = val;
	}
	else if(strcmp(type,"MAX") == 0)
	{
		printerdex.max_line = val;
	}
	else if(strcmp(type,"MAX_LINE") == 0)
	{
		printerdex.max_index = val;
	}
	else if(strcmp(type,"CURRENT") == 0)
	{
		printerdex.current = val;
	}
	else if(strcmp(type,"LINE") == 0)
	{
		printerdex.line = val;
	}
}
