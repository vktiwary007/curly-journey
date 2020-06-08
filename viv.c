/*************************\
 * @author : Vivek Tiwary *
 * Jun 8 2020		  *
 *			  *
 * Text Editor using C    *
\* ***********************/

#include <termios.h>
#include <unistd.h>


/*
 * Method to enable raw mode in editor 
 */
void enableRawMode(){
	
	//termios will get all the terminal attributes
	struct termios raw;
	//get terminal attributes and set it to raw
	tcgetattr(STDIN_FILENO, &raw);
	/* flipped the bits of ECHO by applying bitwise NOT operator(~) 
	 *  & updated the local flag.
	 */
	raw.c_lflag &= ~(ECHO);
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

