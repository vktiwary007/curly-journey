/*************************\
 * @author : Vivek Tiwary *
 * Jun 8 2020		  *
 *			  *
 * Text Editor using C    *
\*************************/

#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

struct termios orig_termios;

/*
 * Method to disable Raw Mod on exiting the editor
 */
void disableRawMode(){
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios);
}

/*
 * Method to enable raw mode in editor 
 */
void enableRawMode(){	
	/* get terminal attributes and set it to orig_termios
	 * termios will get all the terminal attributes
	 */
	tcgetattr(STDIN_FILENO, &orig_termios);
	//called while exiting the editor & disables Raw mode
	atexit(disableRawMode);
	struct termios raw = orig_termios;

	/* flipped the bits of ECHO by applying bitwise NOT operator(~) 
	 *  & updated the local flag.
	 *  Turned off canonical mode by applying BITWISE NOT operator on ICANON flag.
	 *  Now we'll be reading input byte-by-byte instead of line-by-line by updating
	 *  ICANON flag
	 */
	raw.c_lflag &= ~(ECHO | ICANON);
	/* set the updated flag & TCSAFLUSH will apply the changes 
	 * when all pending outputs are written to the terminal
	 */
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	enableRawMode();
   	char c;
	//reading input from standard keypress
       	while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');	
	return 0;
}

