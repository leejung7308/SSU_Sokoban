#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<conio.h>
void load_map();
//int getch(void);
void left(int x, int y, int level);
void right(int x, int y, int level);
void up(int x, int y, int level);
void down(int x, int y, int level);

char map[30][30][6] = { 0 };
int cnt = 0;
int level=1;

int main(){
	int pos_x, pos_y, cnt_O=0, cnt_$=0, i, j, key;
	load_map();
	while(1){
		system("clear");
		for(i=0; i<30; i++){
			for(j=0; j<30; j++){
				if(map[i][j][level]=='@'){
					pos_x=j;
					pos_y=i;
				}
				if(map[i][j][level]=='O') cnt_O++;
				if(map[i][j][level]=='$') cnt_$++;
			}
		}

                printf("%d %d\n%d %d\n",cnt_O,cnt_$,pos_x,pos_y);

		if(cnt_O!=cnt_$){
			printf("Mismatch between box and storage count.\n");
			return 0;
		}
		while(1){
			printf("stage");
			for(i=0; i<30; i++){
				char end[30]={0};
				for(j=0; j<30; j++){
					printf("%c",map[i][j][level]);
					end[j]=map[i][j][level];
				}
				printf("\n");
				if(end[0]==0) break;
			}
			printf("COUNT : %d\n", cnt);
            key = getch();

            switch (key)
            {
	            case 104:
					left(pos_x,pos_y,level);            //left
	                break;
	            case 108:
					right(pos_x,pos_y,level);           //right
	                break;
	            case 107:
					up(pos_x,pos_y,level);              //up
	                break;
	            case 106:
					down(pos_x,pos_y,level);            //down
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
			map[y][x][z]=a;
			x++;
		}
	}
	for(i=1; i<=5; i++){
		map[0][0][i]=i+48;
	}
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			printf("%c",map[i][j][level]);
		}
		printf("\n");
	}
}
/*int getch(void){
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
}*/
void left(int x, int y, int level){
	cnt++;
	if(map[y][x-1][level]=='#') ;
	else{
		if(map[y][x-1][level]=='$'){
			if(map[y][x-2][level]=='#'||map[y][x-2][level]=='$') ;
			else{
				map[y][x-2][level]='$';
				map[y][x-1][level]='@';
				map[y][x][level]='.';
			}
		}
		else{
			map[y][x-1][level]='@';
			map[y][x][level]='.';
		}
	}
}
void right(int x, int y, int level){
	cnt++;
	if(map[y][x+1][level]=='#') ;
	else{
		if(map[y][x+1][level]=='$'){
			if(map[y][x+2][level]=='#'||map[y][x+2][level]=='$') ;
			else{
				map[y][x+2][level]='$';
				map[y][x+1][level]='@';
				map[y][x][level]='.';
			}
		}
		else{
			map[y][x+1][level]='@';
			map[y][x][level]='.';
		}
	}
}
void up(int x, int y, int level){
	cnt++;
	if(map[y-1][x][level]=='#') ;
	else{
		if(map[y-1][x][level]=='$'){
			if(map[y-2][x][level]=='#'||map[y-2][x][level]=='$') ;
			else{
				map[y-2][x][level]='$';
				map[y-1][x][level]='@';
				map[y][x][level]='.';
			}
		}
		else{
			map[y-1][x][level]='@';
			map[y][x][level]='.';
		}
	}
}
void down(int x, int y, int level){
	cnt++;
	if(map[y+1][x][level]=='#') ;
	else{
		if(map[y+1][x][level]=='$'){
			if(map[y+2][x][level]=='#'||map[y+2][x][level]=='$') ;
			else{
				map[y+2][x][level]='$';
				map[y+1][x][level]='@';
				map[y][x][level]='.';
			}
		}
		else{
			map[y+1][x][level]='@';
			map[y][x][level]='.';
		}
	}
}





