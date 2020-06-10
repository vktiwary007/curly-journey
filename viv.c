/*************************\
 * @author : Vivek Tiwary *
 * Jun 8 2020		  *
 *			  *
 * Text Editor using C    *
\*************************/

/*** include ***/

#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>

/*** data ***/

struct termios orig_termios;

/*** terminal ***/

void die(const char *s){
	perror(s);
	exit(1);
}
/*
 * Method to disable Raw Mod on exiting the editor
 */
void disableRawMode(){
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &orig_termios) == -1)
		die("tcsetattr");
}

/*
 * Method to enable raw mode in editor 
 */
void enableRawMode(){	
	/* get terminal attributes and set it to orig_termios
	 * termios will get all the terminal attributes
	 */
	if (tcgetattr(STDIN_FILENO, &orig_termios) == -1)
		die("tcgetattr");
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
	raw.c_cc[VMIN] = 0;
	raw.c_cc[VTIME] = 1;
	if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &raw) == -1 )
		die("tcsetattr");
}

int main(){
	enableRawMode();
	//reading input from standard keypress
       	while(1){
		char c = '\0';
		if (read(STDIN_FILENO, &c, 1) == -1 && errno != EAGAIN )
			die("read");
		if(iscntrl(c)){

			printf("%d\r\n", c);
		}
		else{
			printf("%d ('%c')\r\n", c, c);
		}
		if (c == 'q') break;
	};	
	return 0;
}

