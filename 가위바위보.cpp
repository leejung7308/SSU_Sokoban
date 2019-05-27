 #include <stdio.h>
 #include <stdlib.h>
 #include <termios.h>
 int getch(void){
    int ch;

    struct termios buf;
    struct termios save;

    tcgetattr(0, &save);
    buf = save;

    buf.c_lflag&=~(ICANON|ECHO);
    buf.c_cc[VMIN] = 1;
    buf.c_cc[VTIME] = 0;

    tcsetattr(0, TCSAFLUSH, &buf);

    ch = getchar();
    tcsetattr(0, TCSAFLUSH, &save);

    return ch;
 }
 int main(void){
    int input_char, check = 1, computer = 0;
    system("clear");
    printf("==============================\n");
    printf("  Choose one  \n");
    printf("==============================\n");
    while (check){
       printf("**********************************\n");
       printf("*            M E N U             *\n");
       printf("* 1. Rock  2. Paper  3. Scissors *\n");
       printf("*          0. End                *\n");
       printf("**********************************\n");
       printf("Select a menu : ");
       computer = rand() % 3 + 1;
       input_char = getch();
       system("clear");
       printf("==============================\n");
       switch (input_char){
       case '1':
          if (computer == 1)
             printf("  Draw\n");
          else if (computer == 2)
             printf("  You lose.\n");
          else
             printf("  You Win.\n");
          break;
       case '2':
          if (computer == 2)
             printf("  Draw\n");
          else if (computer == 3)
             printf("  You lose.\n");
          else
             printf("  You Win.\n");
          break;
       case '3':
          if (computer == 3)
             printf("  Draw\n");
          else if (computer == 1)
             printf("  You lose.\n");
          else
             printf("  You Win.\n");
          break;
       case '0':
          check = 0;
          break;
       default:
          printf("  Wrong input\n");
          break;
       }
       printf("==============================\n");
    }
    system("clear");
    return 0;
}
