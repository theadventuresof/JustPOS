#ifndef _ERR
#define _ERR
/*
 * This header includes function descriptions for error handling. Also 
 * includes system messages (non-error)
 */
void print_err(char err[]);
void print_msg(char msg[]);
void center_error(char msg[]);
void err_dialog(char error[]);
void del_err_dialog(void);

#endif
