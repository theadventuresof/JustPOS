#include <panel.h>
#include <sys/time.h>
#include <signal.h>

#include "../lib/err.h"
#include "../lib/misc.h"
#include "../lib/state.h"
#include "../lib/mouse.h"
#include "../lib/scroll.h"
#include "../lib/print.h"

int main(void)
{
	
	initscr();	 			//Start curses mode
	curs_set(0);			//Do not display cursor
	raw();					//Process input immediately
	noecho();				//Do not echo to screen
	cbreak();				//Allow exit via ctrl+c
	keypad(stdscr, TRUE);	//Get mouse/touch/function keys
	erase();				//Clear screen
	start_color();			//Start color mode
	mouseinterval(0);	    //Make buttons respond faster
	/*
	 * Using init_pair(NUMBER,FG_COLOR,BG_COLOR) syntax, create 
	 * background/foreground combinations to later use in our program
	 * called with COLOR_PAIR(NUMBER) These attributes can be mixed
	 * with other attributes such as BOLD, BLINK, etc
	 */
	init_color(COLOR_CYAN,0,600,1000);
	init_color(COLOR_GREEN,150,1000,150);
	init_color(COLOR_RED,1000,0,0);
	init_pair(1,COLOR_BLACK,COLOR_GREEN);   //highlighted text color
	init_pair(2,COLOR_WHITE,COLOR_BLACK);   //regular text color
	init_pair(3,COLOR_RED,COLOR_BLACK);     //err text color
	init_pair(4,COLOR_GREEN,COLOR_BLACK);   //money text color
	init_pair(5,COLOR_BLUE,COLOR_BLACK);    //system message color (non-error)
	init_pair(6,COLOR_YELLOW,COLOR_BLACK);  //keyboard text color
	init_pair(7,COLOR_BLACK,COLOR_RED);
	init_pair(8,COLOR_BLACK,COLOR_GREEN);
	init_pair(9,COLOR_BLACK,COLOR_BLUE);
	init_pair(10,COLOR_BLACK,COLOR_YELLOW);
	init_pair(11,COLOR_CYAN,COLOR_BLACK);
	init_pair(12,COLOR_BLACK,COLOR_BLUE);
	
	
	/*
	 * MEVENT ev creates a structure called ev which is a structure that 
	 * holds information about mouse events; such as what button was 
	 * pressed and where on the screen it occured. 
	 * 
	 * mousemask tells curses which mouse events should be read
	 * 
	 * The escape character \033[?1003h tells the terminal to report
	 * all mouse movement events. Using this we can do touch-scrolling
	 */
	MEVENT ev;
	mousemask(ALL_MOUSE_EVENTS | REPORT_MOUSE_POSITION, NULL);
	printf("\033[?1003h\n"); 
	
	/*
	 * Get screen size
	 */
	int key,y,x;
	getmaxyx(stdscr,y,x);
	
	/*
	 * This if/else condition checks to see if the screen is big enough
	 * before displaying anything. We also check screen size before 
	 * accepting any input from the user and in the case of a screen
	 * re-size. 
	 */
	if((y < 50) & (x < 150))
	{
		erase();
		center_error("The screen is too small to display JustPOS");
		doupdate();
	}
	else{
		/*
		 * Set system state and draw main screen.
		 */
		init(); 
	}
	
	/*
	 * Use setitimer to update draw clock every 15 seconds. 
	 */
	struct itimerval it_val;
	if(signal(SIGALRM,(void(*)(int))print_clock) == SIG_ERR)
	{
		err_dialog("NO CLOCK FOR YOU");
	}
	/*
	 * According to 'man setitimer' if tv_sec and tv_usec are not empty
	 * an interval timer is created.
	 */
	it_val.it_value.tv_sec = 15;
	it_val.it_value.tv_usec = (15*1000) % 1000000;
	it_val.it_interval = it_val.it_value;
	if(setitimer(ITIMER_REAL,&it_val,NULL) == -1)
	{
		err_dialog("NO CLOCK FOR YOU");
	}
	print_clock();
	/*
	 * Main curses loop
	 */
	while((key = getch()) != 'q')
	{
		/*
		 * Only process input if screen is big enough
		 */
		getmaxyx(stdscr,y,x);
		if((key == KEY_MOUSE) & (y > 40) & (x > 150))
		{
			/*
			 * Capture mouse event in struct ev
			 */
			getmouse(&ev);
			/*
			 * Most button actions are not performed until the mouse 
			 * button or touch event is released
			 */
			if(ev.bstate & BUTTON1_RELEASED)
			{
				get_mouse(ev.y,ev.x);
				set_scrolldex("TOUCH",0);
			}	
			/*
			 * If mouse button 1 is pressed, we record 
			 * the touch events y value in case of scrolling
			 */
			else if(ev.bstate & BUTTON1_PRESSED)
			{
				set_scrolldex("TOUCH",ev.y);
			}
			/*
			 * If mouse movement is detected, check if it occured in 
			 * order_win
			 */
			else{
				if((ev.y >= 5) & (ev.y <= 33) & (ev.x >= 105) & (ev.x <= 145) & ((get_state("STATE") == 1) | (get_state("STATE") == 4) | (get_state("STATE") == 5)))
				{
					/*
					 * If the touch event is not the same y axis as current y touch event
					 * do scrolling functions 
					 */
					if((get_scrolldex("DIFF") != get_scrolldex("TOUCH")-ev.y) & (get_scrolldex("TOUCH") > 0))
					{
						set_scrolldex("DIFF",get_scrolldex("TOUCH")-ev.y);
						scroll_order(get_scrolldex("TOUCH")-ev.y);
					}
				}
				/*
				 * Check for recall state (4) and check if event occured in recall win
				 */
				else if((ev.y >= 5) & (ev.y <= 33) & (ev.x >= 50) & (ev.x <= 89) & (get_state("STATE") == 4))
				{
					if((get_scrolldex("DIFF") != get_scrolldex("TOUCH")-ev.y) & (get_scrolldex("TOUCH") > 0))
					{
						set_scrolldex("DIFF",get_scrolldex("TOUCH")-ev.y);
						scroll_recall(get_scrolldex("DIFF"));
					}
				}
				/*
				 * Check for settings menu state (6) and check if event occured on
				 * printer display window
				 */
				else if((ev.y >= 15) & (ev.y <= 20) & (ev.x >= 90) & (ev.x <= 139) & (get_state("STATE") == 6))
				{
					if((get_scrolldex("DIFF") != get_scrolldex("TOUCH")-ev.y) & (get_scrolldex("TOUCH") > 0))
					{
						set_scrolldex("DIFF",get_scrolldex("TOUCH")-ev.y);
						scroll_print(get_scrolldex("DIFF"));
					}
				}
			}
		}
		/*
		 * If window is resized, re-draw current state
		 */
		else if(key == KEY_RESIZE)
		{
			draw_state(get_state("STATE"));
		}
	}
	
	/*
	 * Turn off mouse reporting for terminal
	 */
	printf("\033[?1003l\n");
	endwin();
	return 0;
}
