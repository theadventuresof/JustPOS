#include "../lib/justpos.h"

WINDOW *state_five_sys_btns[5];
PANEL *state_five_sys_btnsp[5];
WINDOW *return_payment;
PANEL *return_paymentp;

/*
 * Draw buttons to cash payment screen for quick dollar amounts
 */
void draw_cash_btns(void)
{
	int y,x;
	x = 75;
	y = 7;
	
	del_cash_btns();
	for(int i = 0; i < 5; i++)
	{
		state_five_sys_btns[i] = newwin(3,15,y+(i*5),x);
		box(state_five_sys_btns[i],0,0);
		state_five_sys_btnsp[i] = new_panel(state_five_sys_btns[i]);
	}
	
	return_payment = newwin(3,15,38,5);
	box(return_payment,0,0);
	return_paymentp = new_panel(return_payment);
	mvwprintw(return_payment,1,5,"RETURN");	
	
	mvwprintw(state_five_sys_btns[0],3/2,5,"EXACT");
	mvwprintw(state_five_sys_btns[1],3/2,7,"$5");
	mvwprintw(state_five_sys_btns[2],3/2,6,"$10");
	mvwprintw(state_five_sys_btns[3],3/2,6,"$20");
	mvwprintw(state_five_sys_btns[4],3/2,5,"$100");
	
	update_panels();
	doupdate();
}

/*
 * 
 */
void del_cash_btns(void)
{
	for(int i = 0; i < 5; i++)
	{
		if(state_five_sys_btns[i] != NULL)
		{
			del_panel(state_five_sys_btnsp[i]);
			state_five_sys_btns[i] = NULL;
		}
		if(return_payment != NULL)
		{
			del_panel(return_paymentp);
			return_payment = NULL;
		}
	}
}
