#ifndef __SCROLL
#define __SCROLL

void scroll_order(int lines);
void scroll_recall(int lines);
void set_scrolldex(char type[],int val);
void set_recalldex(char type[],int val);
void scroll_to_end(void);
void scroll_to_top(void);
void reset_recall_date(void);
void set_recall_date(char date[]);
void get_recall_date(char *date);
int get_scrolldex(char type[]);
int get_recalldex(char type[]);

#endif
