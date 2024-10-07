#ifndef _ORDER
#define _ORDER

void add_itm(int itm_num, int menu);
void insert_head(int itm_num, int menu, char name[]);
void insert_tail(int itm_num, int menu, char name[]);
void del_itm(int itm_num);
void add_mod(int itm_num, int mod_num,int menu);
void add_msg(int itm_num, char msg[]);
void remove_mods(int itm_num);
void add_open_food(float total);
void modify_qty(int itm_num,int qty);
void modify_price(int itm_num,float price);
void save_order(float paid,float change);
void del_order(void);
void print_list(void);
void highlight(int itm_num);
void write_list(void);
int check_duplicate(int itm_num,int menu);
int which_menu(int itm_num);
int total_items(void);
int total_cells(void);
int total_lines(void);
int find_item_lines(int line);
int find_item_max_line(void);
float get_of_val(int itm_num);
float calc_total();


#endif
