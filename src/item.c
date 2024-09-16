#include <panel.h>
#include <string.h>
#include <stdlib.h>

#include "../lib/item.h"
#include "../lib/order.h"
#include "../lib/err.h"

/*
 * Get item name from specified menu
 */
void get_name(int menu,char* name,int itm_num)
{
	char *line = (char *)malloc(100);
	char *dir = (char *)malloc(21);
	char c;
	int i;
	/*
	 * Associate with the correct data file
	 */
	if(menu == 1)
	{
		strncpy(dir,"dat/food.dat",13);
	}
	else if(menu == 2)
	{
		strncpy(dir,"dat/drinks.dat",15);
	}
	/*
	 * If name of mod or charge is desired, use get_mod function
	 */
	else if(menu == 3)
	{
		get_mod(3,name,itm_num);
		free(line);
		free(dir);
		return;
	}
	else if(menu == 4)
	{
		get_mod(4,name,itm_num);
		free(line);
		free(dir);
		return;
	}
	/*
	 * If item is an open food item, copy OPEN FOOD to name
	 */
	else if(which_menu(itm_num) == 5)
	{
		strncpy(name,"OPEN FOOD",10);
		free(line);
		free(dir);
		return;
	}
	/*
	 * Attempt to open data file
	 */
	FILE *data;
	data = fopen(dir,"r");
	
	/*
	 * Fail if file does not exist
	 */
	if(data == NULL)
	{
		return;
	}
	/*
	 * Item 0 does not exist (line 0 of data files contains description)
	 */
	if(itm_num == 0)
	{
		return;
	}
	/*
	 * Get line after line until desired line is retrieved
	 */
	for(i = 0; i < itm_num + 1; i++)
	{
		fgets(line,99,data);
	}
	/*
	 * Loop through line copying to name until first ',' is found
	 */
	for(i = 0;i < 99;i++)
	{
		c = line[i];
		if(c == ',')
		{
			break;
		}
		name[i] = c;
		name[i + 1] = '\0';
	}
	free(line);
	free(dir);
	fclose(data);
}

/*
 * Retrieve either cost or food cost for an item and return as float	
 */
float get_itm(int menu,char val[],int itm_num)
{
	int i,comma=0,j=0,target;
	char *dir = (char *)malloc(50);
	char *line = (char *)malloc(99);
	char *temp = (char *)malloc(50);
	char c;
	float value;
	/*
	 * Associate with correct data file
	 */
	if(menu == 1)
	{
		strncpy(dir,"dat/food.dat\0",13);
	}
	else if(menu == 2)
	{
		strncpy(dir,"dat/drinks.dat\0",15);
	}
	else if(menu == 3)
	{
		strncpy(dir,"dat/mods.dat\0",14);
	}
	else if(menu == 4)
	{
		strncpy(dir,"dat/charge.dat\0",16);
	}
	/*
	 * If item is open food value, retrieve price from order list
	 */
	else if(which_menu(itm_num) == 5)
	{
		value = get_of_val(itm_num);	
		free(line);
		free(dir);
		free(temp);
		return value;
	}
	/*
	 * Is cost or food cost desired
	 */
	if(strcmp(val,"COST") == 0)
	{
		target = 1; //target second column -- after one comma
	}
	else if(strcmp(val,"FC") == 0)
	{
		target = 2; //target third column -- after two commas
	}
	/*
	 * Attempt to open specified menu, failing if not found
	 */
	FILE *data;
	data = fopen(dir,"r");
	
	if(data == NULL)
	{
		move(36,1);
		clrtoeol();
		printw("%s",dir);
		err_dialog("SYSTEM FILE NOT FOUND!! GET_ITM");
		free(dir);
		free(line);
		free(temp);
		fclose(data);
		return -1;
	}
	/*
	 * Get desired line
	 */
	for(i = 0; i < itm_num + 1; i++)
	{
		fgets(line,99,data);
	}	
	/*
	 * Get relavent data from line
	 */
	for(i = 0; i < 99; i++)
	{
		c = line[i];
		if(c == ',')
		{
			comma++;
			continue;
		}
		if(comma == target)
		{
			temp[j] = c;
			temp[j + 1] = '\0';
			j++;
		}
	}
	fclose(data);
	value = strtof(temp,NULL);
	free(line);
	free(dir);
	free(temp);
	return value;
}

/*
 * Get a modification from mods.dat
 */
void get_mod(int menu,char *name,int itm_num)
{
	int i=0;
	char *line = (char*)malloc(50);
	char *dir = (char*)malloc(20);
	char c;
	if(menu == 3)
	{
		strncpy(dir,"dat/mods.dat\0",15);
	}
	else if(menu == 4)
	{
		strncpy(dir,"dat/charge.dat\0",16);
	}
	FILE *data;
	data = fopen(dir,"r");
	if(data == NULL)
	{
	 	err_dialog("MODS.DAT NOT FOUND");
		return;
	}
	if(itm_num == 0)
	{
		return;
	}
	for(i = 0;i < itm_num + 1; i++)
	{
		fgets(line,50,data);
	}
	
	for(i = 0; i < 50; i++)
	{
		if((line[i] == '\n') | (line[i] == ','))
		{
			return;
		}
		c = line[i];
		name[i] = c;
		name[i + 1] = '\0';
	}
	free(line);
	free(dir);
	fclose(data);
}
