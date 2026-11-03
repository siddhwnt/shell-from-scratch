#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <termios.h>

int main()
{
    // Clear stdout after printf
    setbuf(stdout, NULL);

    // setUpPath();

    char text[4096];

    struct termios oldt, newt;
    // Save original terminal settings
    tcgetattr(STDIN_FILENO, &oldt);

    newt = oldt;                          
    newt.c_lflag &= ~(ICANON);   
    
    
    while(1)
    {
        
        int pos = 0;

        tcsetattr(STDIN_FILENO, TCSANOW, &newt);

        printf("$ ");
        while (1)
        {
            int ch = getchar();
            if (ch == EOF)
            {
                tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
                putchar('\n');
                return 0;
            }
            if(ch == '\n')
            {
                break;
            }
            else if(ch == '\t')
            {
                printf("TAB HIT");
                
            }
            else if (ch == 127 || ch == 8)
            {
                if (pos > 0)
                {
                    pos--;
                    printf("\b \b");
                }
            }
            else
            {
                text[pos++] = ch;
            }
        }

        tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
        
        text[pos] = '\0';
        printf("%s\n", text);


    }
}