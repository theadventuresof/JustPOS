#ifndef _RECALL
#define _RECALL

void populate_recall_list(char order[]);
void find_recall_list(char date[],int func);
void del_recall_list(void);
void write_recall(void);
void append_order_recall(char *order);
void sort_recall(void);
int count_recall(void);
int find_recall_order(char val[]);
int find_reacall_lines(int y);

#endif
