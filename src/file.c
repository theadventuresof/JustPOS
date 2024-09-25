#include <panel.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <time.h>
#include <dirent.h>
#include <ctype.h>

#include "../lib/file.h"
#include "../lib/err.h"
#include "../lib/order.h"
#include "../lib/item.h"
#include "../lib/report.h"

struct stat overflow;
time_t rawtime;
struct tm * mytime;
char buffer[80];

DIR *dirp;
struct dirent *orders;

/*
 * Validate user configuration (.conf file and order directory)
 */
void check_conf(void)
{
	/*
	 * If there is no .conf -- create it.
	 */
	if(stat(".conf",&overflow) == -1)
	{
		export_default();
		check_folders();
	}
	/*
	 * Check to see if the directory (dir=) field is not blank
	 */
	else{
		if(check_conf_val("dir=") == -1)
		{
			change_conf_line("dir","dir=orders/");
		}
	check_folders();
	}
}

/*
 * Check if line from .conf file has any data before \n 
 */
int check_conf_val(char conf_arg[])
{
	/*
	 * Open .conf in read only mode
	 */
	FILE *data;
	data = fopen(".conf","r");
	if(data == NULL)
	{
		return false;
	}
	
	/*
	 * Loop through .conf file until EOF
	 */
	char compare[50],check[100],c;
	int isempty;
	while(!feof(data))
	{
		/*
		 * Check for conf_arg in .conf
		 */
		fgets(compare,strlen(conf_arg) + 1,data);
		fgets(check,100,data);
		if(strcmp(conf_arg,compare) == 0)
		{
			/*
			 * Loop through .conf and check if it contains anything
			 * other than empty spaces (or \n or \0)
			 * 
			 * -1 = empty 
			 * 1 = not empty
			 */
			for(int i=0;i < 100; i++)
			{
				c = check[i];
				if(c == ' ')
				{
					continue;
				}
				else if(c == '\n')
				{
					isempty = -1;
					break;
				}
				else if(c == '\0')
				{
					isempty = -1;
					break;
				}
				else{
					isempty = 1;
					break;
				}
			}
		}
	}
	fclose(data);
	return isempty;
}

/*
 * Using touch/echo, create a default .conf file
 */
void export_default(void)
{
	system("touch .conf");
	system("echo \"#This config file was generated by JustPOS. If it is deleted\" >> .conf");
	system("echo \"#it will be regenerated with default configurations upon\" >> .conf");
	system("echo \"#program startup.\n\" >> .conf");
	system("echo \"#Default directory for storing orders.\" >> .conf");
	system("echo \"dir=orders/\n\" >> .conf");
	system("echo \"#A short message to display on top of receipts\" >> .conf");
	system("echo \"msg=Thank you and please come again!!\n\" >> .conf");
	system("echo \"#A phone number or social media handle\" >> .conf");
	system("echo \"contact=1-800-555-2335\n\" >> .conf");
	system("echo \"#Name for reciept printer (via CUPS)\" >> .conf");
	system("echo \"#Leave empty for default printer\" >> .conf");
	system("echo \"printer1=\n\" >> .conf");
	system("echo \"#Number of copies for printer 1\" >> .conf");
	system("echo \"printer1-copies=1\n\" >> .conf");
	system("echo \"#Name for grill printer (via CUPS)\" >> .conf");
	system("echo \"#Leave empty for default printer\" >> .conf");
	system("echo \"printer2=\n\" >> .conf");
	system("echo \"#Number of copies for printer 2\" >> .conf");
	system("echo \"printer2-copies=1\n\" >> .conf");
}

/*
 * Use the sed utility to change a desired line in .conf
 */
void change_conf_line(char arg1[],char arg2[])
{
	char complete_command[150];
	strncpy(complete_command,"sed -i \"/",10);
	strncat(complete_command,arg1,strlen(arg1));
	strncat(complete_command,"/c\\",5);
	strncat(complete_command,arg2,strlen(arg2));
	strncat(complete_command,"\" .conf",10);
	system(complete_command);
}

/*
 * Here we check to see that the folders exist for the current year, 
 * month, and day. We also check for the parent directory orders. If 
 * they do not exist, they are created. This function should be run when
 * the program initializes. 
 */
void check_folders(void)
{
	char command_string[100] = "mkdir -p ";
	char temp[100];
	get_dir_date(temp);
	strncat(command_string,temp,strlen(temp) + 1);
	system(command_string);
}


/*
 * This function simply aims to check the order directory for how many 
 * files (orders) it contains. 
 */
int find_orderno()
{
	
	char folder[100];
	get_dir_date(folder);
	/*
	 * Find number of files (orders) in the order directory corresponding
	 * to the current day. We explicitly check to see that the file name
	 * begins with 'order-'
	 */
	int num=0;
	if((dirp = opendir(folder)) != NULL)
	{
		while((orders = readdir(dirp)) != NULL)
		{
			if(orders->d_type == DT_DIR)
			{
				continue;
			}
			else if(strncmp(orders->d_name,"order-",6) == 0)
			{
				num++;
			}
		}
		closedir(dirp);
	}
	else{
		err_dialog("COULD NOT OPEN ORDER DIRECTORY ORDERS WILL NOT BE SAVED");
	}
	return num;
}

/*
 * Create an order from a receipt (text file) in the orders directory
 */
void recall(char order[])
{
	int i=0;
	int itm_num,drink_num;
	
	/*
	 * Attempt to open the specified file
	 */
	FILE *target_order;
	target_order = fopen(order,"r");
	/*
	 * Fail if file does not exist
	 */
	if(target_order == NULL)
	{
		err_dialog("ORDER NOT FOUND!!");
		return;
	}
	
	char line[100];
	/*
	 * Loop through every line of the receipt file. 
	 */
	while(!feof(target_order))
	{
		/*
		 * Get one line from the receipt file, remove the newline, and
		 * check the line against food and drink menus
		 */
		float charges=0;
		fgets(line,50,target_order);
		line[strcspn(line,"\n")] = 0;
		itm_num = check_menu_line(line,1);
		drink_num = check_menu_line(line,2);
		/*
		 * Check for a match and add corresponding item.
		 */
		if(itm_num > 0)
		{
			add_itm(itm_num,1);
			i++;
		}
		else if(drink_num > 0)
		{
			add_itm(drink_num,2);
			i++; 
		}
		else if(strncmp(line,"OPEN FOOD",9) == 0)
		{
			add_open_food(0);
			i++;
		}
		else if(strncmp(line,"   ",3) == 0)
		{
			charges = recall_mods(line,i-1);
		}
		else if(strncmp(line,"x",1) == 0)
		{
			recall_details(line,i-1,charges);
		}
	}
	fclose(target_order);
}

/*
 * Change quantity and price of recalled items. 
 */
void recall_details(char line2[],int itm_num,float charges)
{
	char qty[100],cost[100],c;
	int i,pos=1,val=0;
	float value,value1;
	/*
	 * Loop through every character in line2 looking for $
	 */
	for(i = 0; i < strlen(line2) + 1; i++)
	{
		c = line2[i];
		if((c == 'x') | (c == ' '))
		{
			pos++;
			continue;
		}
		else if(c == '$')
		{
			pos++;
			val = 1;
			continue;
		}
		/*
		 * Copy value after x until whitespace to 'qty'
		 */
		if(val == 0)
		{
			pos++;
			qty[i-1] = c;
			qty[i] = '\0';
		}
		/*
		 * Copy value after $ to 'cost'
		 */
		else if(val == 1)
		{
			cost[i-pos+1] = c;
			cost[i + 1] = '\0';
		}
	}
	/*
	 * Convert strings to floats and modify qty/price
	 */
	value = strtof(qty,NULL);
	modify_qty(itm_num,value);
	value1 = strtof(cost,NULL);
	if(which_menu(itm_num) == 5)
	{
		char stuff[20];
		sprintf(stuff,"%.2f",get_of_val(itm_num));
		//err_dialog(stuff);
		modify_price(itm_num,((get_of_val(itm_num)*value)+(charges*value)));
		return;
	}
	modify_price(itm_num,((value1/value)-(charges*value)));
	
}

/*
 * Read any lines from receipt that starts with three blank spaces. 
 */
float recall_mods(char line2[],int itm_num)
{
	float extra_charge=0;
	int j=0,mod=0,charge=0;
	char temp[100];
	
	/*
	 * Remove the three blank spaces by copying to temporary string
	 */
	for(j = strlen(line2)+1; j > 2; j--)
	{
		temp[j-3] = line2[j];
	}
	/*
	 * Check temp string value against the value passed to this function
	 * (from receipt/text file)
	 */
	mod = check_menu_line(temp,3);
	if(mod > 0)
	{
		add_mod(itm_num,mod,3);
		return 0;
	}
	charge = check_menu_line(temp,4);
	if(charge > 0)
	{
		add_mod(itm_num,charge,4);
		extra_charge = get_itm(4,"COST",charge);
		return extra_charge;
	}
	else{
		add_msg(itm_num,temp);
	}
	return 0;
}

/*
 * Check for a match on a line in a given file. Used by recall
 * returns a line number if a match is found
 */
int check_menu_line(char val[],int menu)
{
	char target[100];
	int itm_num=0,i=0,j;
	
	/*
	 * Associate with the correct data file
	 */
	if(menu == 1)
	{
		strncpy(target,"dat/food.dat\0",14);
	}
	else if(menu == 2)
	{
		strncpy(target,"dat/drinks.dat\0",16);
	}
	else if(menu == 3)
	{
		strncpy(target,"dat/mods.dat\0",15);
	}
	else if(menu == 4)
	{
		strncpy(target,"dat/charge.dat\0",17);
	}
	
	/*
	 * Open data stream
	 */
	FILE *data;
	data = fopen(target,"r");
	
	/*
	 * Fail if file does not exist
	 */
	if(data == NULL)
	{
		err_dialog("SYSTEM FILE NOT FOUND!!");
		fclose(data);
		return false;
	}
	/*
	 * Convert val to all uppercase letters (allows for search)
	 */
	 
	int len = strlen(val) + 1;
	char caps[len];
	for(int k = 0; k < len; k++)
	{
		caps[k] = toupper(val[k]);
	}
	char line[100],temp[100];
	/*
	 * Get text from desired data file -- stopping at the first comma
	 * or newline
	 */
	while(!feof(data))
	{
		fgets(temp,75,data);
		for(j = 0; j < 75; j++)
		{
			if((temp[j] == ',') | (temp[j] == '\n') | (temp[j] == '\0'))
			{
				break;
			} 
			line[j] = toupper(temp[j]);
			line[j + 1] = '\0';
		}
		/*
		 * If a match is found, return line number from specified 
		 * data file.
		 */
		if(strcmp(val,line) == 0)
		{
			itm_num = i; 
			fclose(data);
			return itm_num;	
		}
		char *ret;
		ret = strstr(line,caps);
		if(ret)
		{
			itm_num = i;
			fclose(data);
			return itm_num;
		}
		i++;
	}
	fclose(data);
	return 0;
}

/*
 * Get current day/date directory according to the dir= value in
 * .conf and local time. 
 */
void get_dir_date(char dir_date[])
{
	time(&rawtime);
	mytime = localtime(&rawtime);
	char test[100];
	get_file_data(".conf","dir=",test);
	strftime(buffer,80,"%Y/%b/%d/",mytime);
	strncat(test,buffer,strlen(buffer) + 1);
	strncpy(dir_date,test,strlen(test) + 1);
}

/*
 * Copy local time for saving order
 */
void get_time(char cur_time[])
{
	time(&rawtime);
	mytime = localtime(&rawtime);
	strftime(buffer,80,"%H:%M %d-%b-%Y",mytime);
	strncpy(cur_time,buffer,strlen(buffer) + 1);
}

/*
 * Get currrent date value
 */
void get_current(char cur[],char *current)
{
	time(&rawtime);
	if(strncmp(cur,"YEAR",4) == 0)
	{
		strftime(buffer,80,"%Y",mytime);
		strncpy(current,buffer,strlen(buffer) + 1);
	}
	else if(strncmp(cur,"MONTH",5) == 0)
	{
		strftime(buffer,80,"%b",mytime);
		strncpy(current,buffer,strlen(buffer) + 1);
	}
	else if(strncmp(cur,"DAY",3) == 0)
	{
		strftime(buffer,80,"%d",mytime);
		strncpy(current,buffer,strlen(buffer) + 1);
	}
}
