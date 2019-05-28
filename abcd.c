#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<termios.h>
char all_map[30][30][6] = { 0 };
char map[65][30]={0};
void load_map();
int getch(void);
int cnt = 0;
int main(){
	int pos_x, pos_y, cnt_O=0, cnt_$=0, i, j, key;
	int level=1;
	load_map();
	while(1){
		system("clear");
		for(i=0; i<30; i++){
			for(j=0; j<30; j++){
				if(all_map[i][j][level]=='@'){
					pos_x=j;
					pos_y=i;
				}
				if(all_map[i][j][level]=='O') cnt_O++;
				if(all_map[i][j][level]=='$') cnt_$++;
			}
		}

                printf("COUNT : %d\n", cnt);

                //printf("%d %d\n%d %d\n",cnt_O,cnt_$,pos_x,pos_y);
           
                key = getch();

                switch (key)
                {
                case 104:             //left
                    
                    break;
                case 108:             //right
                    break;
                case 107:             //up
                    break;
                case 106:             //down
                    break;
                case 117:             //undo
                    break;
                case 114:             //replay
                    break;
                case 110:             //new
                    break;
                case 101:             //exit
                    break;
                case 115:             //save
                    break;
                case 102:             //file load
                    break;
                case 100:             //display help
                    break;
                case 116:             //top
                    break;
                default:
                    break;
                }
		if(cnt_O!=cnt_$){
			printf("박스와 보관장소의 개수가 맞지 않습니다.\n");
			return 0;
		}
		while(1){
			printf("stage");
			for(i=0; i<30; i++){
				char end[30]={0};
				for(j=0; j<30; j++){
					printf("%c",all_map[i][j][level]);
					end[j]=all_map[i][j][level];
				}
				printf("\n");
				if(end[0]==0) break;
			}
			system("clear");
		}
	}
	return 0;	
}

void load_map(){
	int i,j;
	FILE *f=fopen("map","r");
	char a;
	int x=0, y=0, z=0;
	while(fscanf(f,"%c",&a)!=EOF){
		if(a=='\n'){
			y++;
			x=0;
		}
		else if(a=='1'||a=='2'||a=='3'||a=='4'||a=='5'||a=='e'){
			z++;
			y=0;
			x=0;
		}
		else{
			all_map[y][x][z]=a;
			x++;
		}
	}
	for(i=1; i<=5; i++){
		all_map[0][0][i]=i+48;
	}
}
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
