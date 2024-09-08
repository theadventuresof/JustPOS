#ifndef __REPORT
#define __REPORT

void gen_daily(char folder[]);
void gen_monthly(char month[]);
void gen_yearly(char year[]);
void get_file_data(char file[],char val[],char *dest);
void gen_today(void);
float scrape_order(char order_path[]);
float scrape_daily(char month[]);
int validate_month(char month[]);

#endif
