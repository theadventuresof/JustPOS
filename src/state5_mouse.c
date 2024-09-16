#include <stdio.h>

#include "../lib/mouse.h"
#include "../lib/order.h"
#include "../lib/keypad.h"

/*
 * Direct mouse input for keypad insertion
 */
void find_mouse_payment_keypad(int y, int x)
{
	if((y >= 6) & (y <= 35) & (x >= 5) & (x <= 68))
	{	
		find_mouse_keypad(y,x);
	}
}

/*
 * Copy exact dollar amounts to keypad  
 */
void find_mouse_cash_buttons(int y,int x)
{
	/*
	 * If exact change is selected 
	 */
	if((y >= 7) & (y <= 9) & (x >= 75) & (x <= 88))
	{
		char total[50];
		sprintf(total,"%.2f",calc_total());
		copy_to_keypad(total);
	}
	/*
	 * If $5 is selected 
	 */
	else if((y >= 12) & (y <= 14) & (x >= 75) & (x <= 88))
	{
		copy_to_keypad("5.00");
	}
	/*
	 * If $10 is selected
	 */
	else if((y >= 17) & (y <= 19) & (x >= 75) & (x <= 88))
	{
		copy_to_keypad("10.00");
	} 
	/*
	 * If $20 is selected
	 */
	else if((y >= 22) & (y <= 25) & (x >= 75) & (x <= 88))
	{
		copy_to_keypad("20.00");
	}
	/*
	 * If $50 is selected
	 */
	else if((y >= 27) & (y <= 29) & (x >= 75) & (x <= 88))
	{
		copy_to_keypad("50.00");
	}
	/*
	 * If $100 is selected
	 */
	else if((y >= 32) & (y <= 34) & (x >= 75) & (x <= 88))
	{
		copy_to_keypad("100.00");
	}
}
