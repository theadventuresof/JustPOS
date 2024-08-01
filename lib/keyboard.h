#ifndef _KEYBOARD
#define _KEYBOARD

void draw_keyboard(void);
void write_keys(void);
void del_keyboard(void);
void add_key(char key[]);
void clear_keyboard(void);
void copy_keyboard_val(char *val);
void backspace_keyboard(void);
void set_keyboard(char ref[],int val);
int get_keyboard(char ref[]);

#endif
