#ifndef __REPORT
#define __REPORT

void gen_daily(char folder[]);
void gen_monthly(char month[]);
void gen_yearly(char year[]);
void get_file_data(char file[],char val[],char *dest);
float scrape_daily(char month[],float *vals);
float scrape_monthly(char path[],char month[],float *vals);
float scrape_order(char order_path[]);
float scrape_voids(char file[]);
int validate_month(char month[]);

#endif
