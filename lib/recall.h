#ifndef _RECALL
#define _RECALL

void populate_recall_list(char order[]);
void find_recall_list(char date[],int func);
void del_recall_list(void);
void write_recall(void);
void append_order_recall(char *order);
int find_reacall_lines(int y);

#endif
