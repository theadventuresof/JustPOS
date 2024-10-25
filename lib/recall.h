#ifndef _RECALL
#define _RECALL

typedef struct recall_ord{
	char order[50];
	struct recall_ord *next;
	struct recall_ord *top;
}recall_ord; 	

void populate_recall_list(char order[]);
void find_recall_list(char date[],int func);
void del_recall_list(void);
void write_recall(void);
void append_order_recall(char *order);
struct recall_ord *sort_recall(void);
int get_length(struct recall_ord *top);
int get_orderno(char order[]);
int count_recall(void);
int find_recall_order(char val[]);
int find_reacall_lines(int y);

#endif
