JustPOS is a Point of Sale client for concession/food truck style vending.
This program is not meant to be useful to anyone and is in a prepetual
beta stage. JustPOS was written entirely in C using ncurses/panels. 

The goal of this project was not to be a useful or popular POS, but 
rather a way to learn how to use C and make TUI applications. 

This program was developed entirely on a Microsoft Surface Go 2. (128GB
Model) Using GCC and Geany IDE on Fedora Linux with GNOME/Wayland. 
This program is developed for Linux(64) only with no plans to support 
other OSes in the future. (Although an ARM version is not out of the 
question)

Building and Running----------------------------------------------

Building can be accomplished simply by running

$ make

In the JustPOS directory. 

To run JustPOS, simply enter the justpos directory and use

$ ./justpos

Scrolling----------------------------------------------------------

The scrolling functions -- as mentioned above -- were tested
on a Microsoft Surface Go 2 running Fedora with GNOME/Wayland using 
gnome-terminal. When items are placed in an order (by touching the item
button) they fill the order window. When the order window cannot display
more items, it begins to show the items that fit in the window. Scrolling
in this application is not exactly the same as on a smartphone for example.
To succcessfully scroll the program expects that the user's finger is 
placed inside the order window and dragged either up or down, but not both.
And then released before attempting to drag in the opposite direction.

For the sake of giving extra information I tried to use the ncurses 
scroll(int) with scrollok(win,TRUE) idlok(win,TRUE) and wsetscrreg(win,0,TRUE).
But I couldn't get it to work for lines that were not printed in the window.
So I had to write my own.   

States----------------------------------------------------------------

Developers reference:

States are virtual placeholders for what is visible on the screen. This
is so that the correct mouse events are captured at the correct times.

get/set_state() states

STATE = 0

Error dialog window is visible

STATE = 1

The main menu is visible 

STATE = 2

Keypad is visible on top of the main menu

STATE = 3

Keyboard is visible on top of the main menu

STATE = 4

Recall menu is visible

STATE = 5

Cash payment screen is visible

BSTATE = 1

Food menu items are drawn to the main menu

BSTATE = 2

Drink menu items are drawn to the main menu

BSTATE = 3

Mod menu items are drawn to the main menu

BSTATE = 4 

Extra charge items are drawn to the main menu 
