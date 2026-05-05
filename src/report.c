#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>
#include <curses.h>

#include "../lib/report.h"
#include "../lib/file.h"
#include "../lib/item.h"
#include "../lib/err.h"

DIR *dir;
DIR *subdir;
DIR *month_dir;
DIR *month_dir_void;
DIR *subdir_void;
DIR *year_dir;
struct dirent *order;
struct dirent *month_order;
struct dirent *month_order_void;
struct dirent *year_order;
struct dirent *suborder;
struct dirent *suborder_void;
struct stat buf;

/*
 * Enter a given folder (day of buisiness) and read all receipts (text 
 * files) and calculate the total net sales for that day.
 */
void gen_daily(char folder[])
{
	float daily_total=0;
	float void_total=0;
	float food_cost=0;
	/*
	 * If folder[] is a valid path, loop through every file/folder
	 */
	if((dir = opendir(folder)) != NULL)
	{
		while((order = readdir(dir)) != NULL)
		{
			/*
			 * If file name begins with 'order-', assume it is a receipt
			 * and try to extract total price.
			 */
			if(strncmp(order->d_name,"order-",6) == 0)
			{
				char target_file[100];
				strncpy(target_file,folder,strlen(folder) + 1);
				strncat(target_file,order->d_name,strlen(order->d_name) + 1);
				daily_total += scrape_order(target_file);
				food_cost += scrape_voids(target_file);
			} 
			/*
			 * If file name begins with 'void-', assume it is a voided
			 * receipt, and try to extract lost food cost
			 */
			else if(strncmp(order->d_name,"void-",5) == 0)
			{
				char target_file[100];
				strncpy(target_file,folder,strlen(folder) + 1);
				strncat(target_file,order->d_name,strlen(order->d_name) + 1);
				void_total += scrape_voids(target_file);
			}
		}
	}
	closedir(dir);
	food_cost += void_total;
	char target_file[100];
	strncpy(target_file,folder,strlen(folder) + 1);
	strncat(target_file,"daily.rpt",10);
	FILE *rpt;
	rpt = fopen(target_file,"w");
	if(rpt == NULL)
	{
		return;
	}
	fprintf(rpt,"net_sales=%.2f\n",daily_total);
	fprintf(rpt,"total_voids=%.2f\n",void_total);
	fprintf(rpt,"total_fc=%.2f\n",food_cost);
	fprintf(rpt,"profits=%.2f\n",daily_total - food_cost);
	fclose(rpt);
}	

/*
 * Retrieve total sales from a receipt (text file).
 */
float scrape_order(char order_path[])
{
	char line[100];
	char target[50];
	int i=0,j=0;
	float total=0;
	
	FILE *op;
	op = fopen(order_path,"r");
	if(op == NULL)
	{
		return false;
	}
	/*
	 * If order_path is a valid file path, loop through until EOF
	 */
	while(!feof(op))
	{
		/*
		 * Retrieve every line from order_path and check for 'Total  Due'
		 */
		fgets(line,99,op);
		if(strncmp(line,"Total  Due",10) == 0)
		{
			/*
			 * If 'Total  Due' is found, remove the newline character and
			 * loop until '$' is found.
			 */
			line[strcspn(line,"\n")] = 0;
			for(i = 0;i < strlen(line) + 1; i++)
			{
				j++;
				if(line[i] == '$')
				{
					break;
				}
			} 
			/*
			 * Starting from the position after '$' copy character by 
			 * character from text file to target string
			 */
			for(i = 0; i < strlen(line) + 1; i++)
			{
				target[i] = line[j];
				target[i + 1] = '\0';
				j++;
			}
		}
	}
	/*
	 * Convert string to float and return it.
	 */
	total = strtof(target,NULL);
	fclose(op);
	return total;
}

/*
 * Get voided/food cost date from specified file
 */
float scrape_voids(char file[])
{
	/*
	 * Establish a float value to hold food cost and open specified
	 */
	float val = 0;
	FILE *target = fopen(file,"r");
	/*
	 * Stop here if specified file does not exist
	 */
	if(file == NULL)
	{
		return false;
	}
	/*
	 * Define a string to hold each line, and begin looping through 
	 * specified file until EOF
	 */
	char line[100];
	while(!feof(target))
	{
		/*
		 * Copy value of line in file to string 'line' and remove newline
		 * character
		 */
		int item;
		fgets(line,99,target);
		line[strcspn(line,"\n")] = 0;
		/*
		 * Loop through the four menus (Food, Drink, Mods, Extra Charge)
		 * to look for a matching item
		 */
		for(int i = 1; i <= 4; i++)
		{
			/*
			 * Mods do not have a food cost value, skip this item
			 */
			if(i == 3)
			{
				continue;
			}
			item = check_menu_line(line,i);
			/*
			 * If file line is a menu item, try to extract food cost and
			 * add it to val
			 */
			if(item > 0)
			{
				val += get_itm(i,"FC",item); 
			}
		}
	}
	/*
	 * Close file and return floating value 'val'
	 */
	fclose(target);
	return val;
}

/*
 * Given an abbreviated month name, use dir= value from .conf -- check
 * if path is valid, and attempt to calculate sales from every day in 
 * said month. Also output value to text file
 */
void gen_monthly(char month[])
{
	float vals[4];
	char path[100],temp[50];
	
	/*
	 * Build a file path from given month. Only for current year
	 */
	get_file_data(".conf","dir=",temp);
	strncpy(path,temp,strlen(temp) + 1);
	get_current("YEAR",temp);
	strncat(path,temp,strlen(temp) + 1);
	strncat(path,"/",2);
	strncat(path,month,strlen(month) + 1);
	strncat(path,"/",2);
	
	/*
	 * If path exists, scrape all the orders for the total sales value
	 */
	if(stat(path,&buf) == 0)
	{
		scrape_daily(path,vals);
	}
	
	/*
	 * Write total sales value to text file in month folder
	 */
	strncat(path,"monthly.rpt",12);
	FILE *rpt;
	rpt = fopen(path,"w");
	if(rpt == NULL)
	{
		return;
	}
	fprintf(rpt,"net_sales=%.2f\n",vals[0]);
	fprintf(rpt,"total_voids=%.2f\n",vals[1]);
	fprintf(rpt,"total_fc=%.2f\n",vals[2]);
	fprintf(rpt,"profits=%.2f\n",vals[0]-vals[2]);
	fclose(rpt);
}

/*
 * Collect daily report information from every day in a given month
 */
float scrape_daily(char month[],float *vals)
{
	char net[50];
	char voids[50];
	char fc[50];
	char profit[50];
	float totals=0,voided=0,food=0,profits=0;
	if((month_dir = opendir(month)) != NULL)
	{
		while((month_order = readdir(month_dir)) != NULL)
		{
			/*
			 * Go through month folder and check every directory
			 */
			if(month_order->d_type == DT_DIR)
			{
				/*
				 * Don't search parent folder '..'
				 */
				if((strncmp(month_order->d_name,"..",3) == 0) | (strncmp(month_order->d_name,".",2) == 0))
				{
					continue;
				}
				if(strlen(month_order->d_name) >2)
				{
					continue;
				}
				/*
				 * Create two strings, one to hold the file path of 
				 * the current day, and the other to hold the file
				 * path of "daily.rpt"
				 */
				char temp[100];
				char daily[100];
				strncpy(temp,month,strlen(month) + 1);
				strncat(temp,month_order->d_name,strlen(month_order->d_name) + 1);
				strncat(temp,"/",2);
				strncpy(daily,temp,strlen(temp)+1);
				strncat(daily,"daily.rpt",10);
				/*
				 * If 'daily.rpt' cannot be found, generate it
				 */
				if(stat(daily,&buf) != 0)
				{
					gen_daily(temp);
				}
				/*
				 * Open 'daily.rpt' in read-only mode
				 */
				FILE *daily_report;
				daily_report = fopen(daily,"r");
				/*
				 * Continue in loop if daily report cannot be found
				 */
				if(daily_report == NULL)
				{
					continue;
				}
				/*
				 * Copy the values from the daily report to some strings
				 * and convert the strings to 
				 */
				get_file_data(daily,"net_sales=",net);
				totals += strtof(net,NULL);
				get_file_data(daily,"total_voids=",voids);
				voided += strtof(voids,NULL);
				get_file_data(daily,"total_fc=",fc);
				food += strtof(fc,NULL);
				get_file_data(daily,"profits=",profit);
				profits += strtof(profit,NULL);
				fclose(daily_report);
			}
		}
	}
	vals[0] = totals;
	vals[1] = voided;
	vals[2] = food;
	vals[3] = profits;
	closedir(month_dir);
	return 0;
}

/*
 * 
 */
float scrape_monthly(char path[],char month[],float *vals)
{
	char net[50];
	char voids[50];
	char fc[50];
	char profit[50];
	float totals=0,voided=0,food=0,profits=0;
	if(stat(path,&buf) == 0)
	{
		char monthly[100];
		strncpy(monthly,path,strlen(path)+1);
		strncat(monthly,"monthly.rpt",12);
		/*
		 * If monthly.rpt cannot be found, generate it
		 */
		if(stat(monthly,&buf) != 0)
		{
			gen_monthly(month);
		}
		/*
		 * Open monthly.rpt in read only mode
		 */
		FILE *monthly_report;
		monthly_report = fopen(monthly,"r");
		if(monthly_report == NULL)
		{
			err_dialog("Something went wrong!");
			return false;
		}
		/*
		 * Copy values from monthly_report.rpt to some strings and 
		 * convert to floats
		 */
		 get_file_data(monthly,"net_sales=",net);
		 totals += strtof(net,NULL);
		 get_file_data(monthly,"total_voids",voids);
		 voided += strtof(voids,NULL);
		 get_file_data(monthly,"total_fc=",fc);
		 food += strtof(fc,NULL);
		 get_file_data(monthly,"profits=",profit);
		 profits += strtof(profit,NULL);
		 fclose(monthly_report);
	}
	
	vals[0] = totals;
	vals[1] = voided;
	vals[2] = food;
	vals[3] = profits;
	return 0;
}

/*
 * Generate a file named yearly.rpt with the current sales from the
 * current year
 */
void gen_yearly(char year[])
{
	char path[100];
	float vals[4];
	float totals[4];
	
	/*
	 * Build a string to contain file path
	 */
	get_file_data(".conf","dir=",path);
	strncat(path,year,strlen(year) + 1);
	strncat(path,"/",2);
	/*
	 * If path does not exist, display an error and return
	 */
	if((year_dir = opendir(path)) == NULL)
	{
		err_dialog("No orders exist for specified year");
		return;
	}
	/*
	 * Check every file/folder in path string
	 */
	if((year_dir = opendir(path)) != NULL)
	{
		while((year_order = readdir(year_dir)) != NULL)
		{
			if(year_order->d_type == DT_DIR)
			{
				/*
				 * If the folder is not a correct (English) month 
				 * abbreviation, continue
				 */
				if(validate_month(year_order->d_name) == 0)
				{
					continue;
				}
				/*
				 * Build a string with the file path that needs to be 
				 * checked
				 */
				char line[100];
				get_file_data(".conf","dir=",line);
				strncat(line,year,strlen(year) + 1);
				strncat(line,"/",2);
				strncat(line,year_order->d_name,strlen(year_order->d_name) + 1);
				strncat(line,"/",2);
				/*
				 * Get data from monthly.rpt files
				 */
				scrape_monthly(line,year_order->d_name,vals);
				/*
				 * Make totals equal the totals of all monthly.rpt files
				 */
				totals[0] += vals[0];
				totals[1] += vals[1];
				totals[2] += vals[2];
				totals[3] += vals[3];
			}
		}
	}
	closedir(year_dir);
	strncat(path,"yearly.rpt",11);
	/*
	 * Output total to 'yearly.rpt'
	 */
	FILE *rpt;
	rpt = fopen(path,"w");
	
	if(rpt == NULL)
	{
		return;
	}
	fprintf(rpt,"net_sales=%.2f\n",totals[0]);
	fprintf(rpt,"total_voids=%.2f\n",totals[1]);
	fprintf(rpt,"total_fc=%.2f\n",totals[2]);
	fprintf(rpt,"total_profit=%.2f\n",totals[3]);
	fclose(rpt);
}

/*
 * Return 1 if folder name is an abbreviated month...limited to English only
 */
int validate_month(char month[])
{
	int valid = 0;
	
	if(strncmp(month,"Jan",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Feb",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Mar",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Apr",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"May",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Jun",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Jul",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Aug",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Sep",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Oct",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Nov",3) == 0)
	{
		valid = 1;
	}
	else if(strncmp(month,"Dec",3) == 0)
	{
		valid = 1;
	}
	return valid;
}

/*
 * Search for match in given file for val[] and if found, copy to *dest
 */
void get_file_data(char file[],char val[],char *dest)
{
	/*
	 * Attempt to open specified file in read-only mode
	 */
	FILE *data;
	data = fopen(file,"r");
	/*
	 * Fail if file does not exist
	 */
	if(data == NULL)
	{
		return;
	}
	/*
	 * Go through every line of file checking for match to desired string
	 */
	char line[100],target[100];
	while(!feof(data))
	{
		fgets(line,strlen(val) + 1,data);
		/*
		 * If match is found -- copy to *dest.
		 */
		if(strncmp(line,val,strlen(val) + 1) == 0)
		{
			fgets(target,99,data);
			target[strcspn(target,"\n")] = 0;
			strncpy(dest,target,strlen(target) + 1);
		}
	}
	
	fclose(data);
}

