#ifndef _DRAW
#define _DRAW

void draw_menu_buttons(void);
void draw_sys_buttons(void);
void draw_page_status(void);
void draw_menu_tabs(void);
void draw_order(void);
void draw_order_stat(void);
void draw_logo(void);
void del_order_win(void);
void del_menu_buttons(void);
void del_sys_buttons(void);
void del_page_status(void);
void del_menu_tabs(void);
void del_order_stat(void);
void clear_until(int starty,int startx,int endx);
void clear_order_win(void);
void clear_menu_buttons(void);
void clear_sys_buttons(void);
void clear_order_stat(void);
void write_to_menu_buttons(void);
void write_to_order_win(char details[],int format,int highlight);
void write_pages(void);
void index_init(void);
void set_index(char type[],int val);
void set_max_pages(void);
void shuffle_tabs(void);
void update_order_stat(void);
void reset_menu_button(void);
void concat_blanks(int num,char *dest);
void select_menu_button(int button);
int get_max_pages(char type[]);
int get_index(char type[]);

#endif
