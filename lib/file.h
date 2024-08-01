#ifndef __SAVE
#define __SAVE

void export_default(void);
void change_conf_line(char arg1[],char arg2[]);
void check_folders(void);
void check_conf(void);
void get_conf_data(char target[],char *dataval);
void get_dir_date(char dir_date[]);
void get_time(char cur_time[]);
void get_current(char cur[],char *current);
void recall(char order[]);
void recall_mods(char line2[],int itm_num);
void recall_details(char line2[],int itm_num);
int check_menu_line(char val[],int menu);
int check_conf_val(char conf_arg[]);
int find_orderno(void);

#endif
