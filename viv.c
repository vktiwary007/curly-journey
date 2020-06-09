/*************************\
 * @author : Vivek Tiwary *
 * Jun 8 2020		  *
 *			  *
 * Text Editor using C    *
\*************************/

#include <ctype.h>
#include <stdio.h>
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
	/* IXON flag will prevent ctrl-S (XOFF) & ctrl-Q (XON) to pause data transmission
	 * ICRNL will fix ctrl-M issue. CR: Carriage return, NL: New line
	 */
	raw.c_iflag &= ~(BRKINT | ICRNL |INPCK | ISTRIP | IXON);
	// OPOST will turn off output processing features like "\n" to "\r\n"
	raw.c_oflag &= ~(OPOST);
	/* flipped the bits of ECHO by applying bitwise NOT operator(~) 
	 *  & updated the local flag.
	 *  Turned off canonical mode by applying BITWISE NOT operator on ICANON flag.
	 *  Now we'll be reading input byte-by-byte instead of line-by-line by updating
	 *  ICANON flag. ~ISIG flag will prevent ctrl-c to send SIGINT & 
	 *  ctrl-z to send SIGSTP signal. ~IEXTEN will fix ctrl-V, ctrl-C, ctril-O behavior
	 */
	raw.c_cflag |=(CS8);
	raw.c_lflag &= ~(ECHO | ICANON | IEXTEN | ISIG);
	/* set the updated flag & TCSAFLUSH will apply the changes 
	 * when all pending outputs are written to the terminal
	 */
	tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw);
}

int main(){
	enableRawMode();
   	char c;
	//reading input from standard keypress
       	while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q'){
		if(iscntrl(c)){
			printf("%d\r\n", c);
		}
		else{
			printf("%d ('%c')\r\n", c, c);
		}
	};	
	return 0;
}

