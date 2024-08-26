#ifndef _KEYPAD
#define _KEYPAD

void draw_keypad(char pos[]);
void delete_keypad(void);
void set_keypad_state(char ref[],int val);
void set_keypad_val(char val[1]);
void copy_to_keypad(char val[]);
void clear_keypad(void);
void write_keypad_val(void);
void keypad_back(void);
int get_keypad_state(char ref[]);
int has_decimal(void);
int decimal_check(char target[],char ref); 
float get_keypad_data(void);

#endif
