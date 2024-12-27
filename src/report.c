#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <dirent.h>

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
		 * Retrieve every line from order_path and check for 'Total Due'
		 */
		fgets(line,99,op);
		if(strncmp(line,"Total  Due",10) == 0)
		{
			/*
			 * If 'Total Due' is found, remove the newline character and
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
			 * Starting from the position of '$' copy character by 
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
 * 
 */
float scrape_voids(char file[])
{
	float val = 0;
	FILE *target = fopen(file,"r");
	if(file == NULL)
	{
		return false;
	}
	char total[100];
	get_file_data(file,"Total  Due:     $",total);
	val += strtof(total,NULL);
	center_error(total);
	/*while(!feof(target))
	{
		int item;
		fgets(line,99,target);
		line[strcspn(line,"\n")] = 0;
		for(int i = 1; i <= 4; i++)
		{
			if(i == 3)
			{
				continue;
			}
			item = check_menu_line(line,i);
			if(item > 0)
			{
				val += get_itm(i,"FC",item); 
			}
		}
	}*/
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
	float total=0;
	float voids=0;
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
		total = scrape_daily(path);
		voids = scrape_daily_voids(path);
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
	fprintf(rpt,"net_sales=%.2f",total);
	fprintf(rpt,"total_voids=%.2f",voids);
	fclose(rpt);
}

/*
 * Collect total sales from every order in a given month
 */
float scrape_daily(char month[])
{
	float day=0,val=0;
	char *err;
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
				 * Convert DIR name to float and check if it is less than
				 * 31 (A day of the month)
				 * 
				 * If *err points to the same location as order->d_name, 
				 * no conversion occured and day = 0.00 
				 */
				day = strtof(month_order->d_name,&err);
				if(err == month_order->d_name)
				{
					continue;
				}
				if((day <= 31) & (day > 0))
				{
					char temp[100];
					char line[100];
					char daily[100];
					strncpy(temp,month,strlen(month) + 1);
					strncat(temp,month_order->d_name,strlen(month_order->d_name) + 1);
					strncat(temp,"/",2);
					/*
					 * If our DT_DIR name is between 01 and 31, open each day
					 * and check for regular files starting with 'order-'
					 */
					strncpy(daily,temp,strlen(temp)+1);
					strncat(daily,"daily.rpt",10);
					/*
					 * If daily report exists
					 */
					if(stat(daily,&buf) == 0)
					{
						FILE *daily_report;
						daily_report = fopen(daily,"r");
						if(daily_report == NULL)
						{
							return false;
						}
						char total[50];
						get_file_data(daily,"net_sales=",total);
						val += strtof(total,NULL);
					}
					/*
					 * If daily report does not exist
					 */
					else{
						if((subdir = opendir(temp)) != NULL)
						{
							while((suborder = readdir(subdir)) != NULL)
							{
								/*
								 * If our DT_REG files begin with 'order-', scrape them
								 * for their total sales value. 
								 */
								if(strncmp(suborder->d_name,"order-",6) == 0)
								{
									strncpy(line,temp,strlen(temp) + 1);
									strncat(line,suborder->d_name,strlen(suborder->d_name) + 1);
									val += scrape_order(line);
								}
							}
						}
						closedir(subdir);
					}
				}
			}
		}
	}
	closedir(month_dir);
	/*
	 * Return total sales value (if found)
	 */
	return val;
}

/*
 * 
 */
float scrape_daily_voids(char month[])
{
	float day=0;
	float val=0;
	char *err;
	if((month_dir_void = opendir(month)) != NULL)
	{
		while((month_order_void = readdir(month_dir_void)) != NULL)
		{
			if(month_order_void->d_type == DT_DIR)
			{
				day = strtof(month_order_void->d_name,&err);
				if(err == month_order_void->d_name)
				{
					continue;
				}
				if((day >= 0) & (day <= 31))
				{
					char temp[100];
					char line[100];
					char daily[100];
					strncpy(temp,month,strlen(month) + 1);
					strncat(temp,month_order_void->d_name,strlen(month_order_void->d_name) + 1);
					strncat(temp,"/",2);
					
					strncat(daily,temp,strlen(temp)+1);
					strncat(daily,"daily.rpt",10);
					
					if(stat(daily,&buf) == 0)
					{
						FILE *daily_report;
						daily_report = fopen(daily,"r");
						if(daily_report == NULL)
						{
							return false;
						}
						char total[50];
						get_file_data(daily,"total_voids=",total);
						val += strtof(total,NULL);
					}
					else{
						if((subdir_void = opendir(temp)) != NULL)
						{
							while((suborder_void = readdir(subdir_void)) != NULL)
							{
								if(strncmp(suborder_void->d_name,"void-",5) == 0)
								{
									strncpy(line,temp,strlen(temp)+1);
									strncat(line,suborder_void->d_name,strlen(suborder_void->d_name)+1);
									val += scrape_voids(line);
								}
							}
						}
					}
					closedir(subdir_void);
				}
			}
		}
	}
	closedir(month_dir_void);
	return val;
}

/*
 * Generate a file named yearly.rpt with the current sales from the
 * current year
 */
void gen_yearly(char year[])
{
	char path[100];
	float total=0;
	
	/*
	 * Build a string to contain file path
	 */
	get_file_data(".conf","dir=",path);
	strncat(path,year,strlen(year) + 1);
	strncat(path,"/",2);
	/*
	 * Check every file/folder in path string
	 */
	if((year_dir = opendir(path)) != NULL)
	{
		while((year_order = readdir(year_dir)) != NULL)
		{
			/*
			 * If first three characters from abbreviated month name are 
			 * matched, assume they are folders and check inside for orders
			 */
			if(validate_month(year_order->d_name) == 1)
			{
				char line[100];
				get_file_data(".conf","dir=",line);
				strncat(line,year,strlen(year) + 1);
				strncat(line,"/",2);
				strncat(line,year_order->d_name,strlen(year_order->d_name) + 1);
				strncat(line,"/",2);
				total += scrape_daily(line);
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
	fprintf(rpt,"net_sales=%.2f",total);
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
 * Generate all reports 
 */
void gen_today(void)
{
	char date[100],temp[50];
	get_file_data(".conf","dir=",date);
	get_current("YEAR",temp);
	strncat(date,temp,strlen(temp) + 1);
	strncat(date,"/",2);
	gen_yearly(temp);
	get_current("MONTH",temp);
	gen_monthly(temp);
	strncat(date,temp,strlen(temp) + 1);
	strncat(date,"/",2);
	get_current("DAY",temp);
	strncat(date,temp,strlen(temp) + 1);
	strncat(date,"/",2);
	gen_daily(date);
	
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

