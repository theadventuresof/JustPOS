#ifndef _STATE
#define _STATE

void set_state(char type[],int val);
int get_state(char type[]);
int get_item_max(void);
int get_page(char type[]);
void init(void);
void draw_state(int state);
void del_state(int state);
void find_max_items(void);

#endif
