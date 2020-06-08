#include <unistd.h>
int main(){
   	char c;
	//reading input from standard keypress
       	while(read(STDIN_FILENO, &c, 1) == 1 && c != 'q');	
	return 0;
}

