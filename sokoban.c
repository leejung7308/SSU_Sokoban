#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
//#include<conio.h>
#include<termios.h>
void load_map();			//맵 받아오기                     
int getch(void); 			//엔터 없이 입력
void left(int x, int y, int level); 	//왼
void right(int x, int y, int level); 	//오
void up(int x, int y, int level); 	//위
void down(int x, int y, int level); 	//아래
void count_check(void); 		//상자, 보관장소
void position(void); 			//@ 좌표잡기
void set_storage(void); 		//보관장소 놓기
void pos_storage(void); 		//보관장소 좌표
void print_map(void); 			//맵 출력
void push_key(void); 			// 키 입력
void display_help(void); 		//도움말

char map[30][30][6] = { 0 };		//맵 배열
int cnt = 0;				//이동횟수
int level=1;				//레벨
int pos_x, pos_y, cnt_O=0, cnt_$=0;	//@ 좌표 및 상자, 보관장소 개수
int stor_x[20]={0},stor_y[20]={0};	//보관장소 좌표 배열
int key;				//입력 받는 키

int main(){
	int i, j;
	load_map();
	while(1){  			//게임시작
	    system("clear");
		count_check();
		if(cnt_O!=cnt_$){
			printf("Mismatch between box and storage count.\n"); 
			return 0;
		}
		pos_storage();
		while(1){  		//게임 진행
			position();
			print_map();
			
			printf("COUNT : %d\n", cnt);
			//printf("%d %d\n%d %d\n",cnt_O,cnt_$,pos_x,pos_y);  
            push_key();
            set_storage();
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
void position(void){
	int i,j;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(map[i][j][level]=='@'){
				pos_x=j;
				pos_y=i;
			}
		}
	}
}
void count_check(void){
	int i,j;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(map[i][j][level]=='O') cnt_O++;
			if(map[i][j][level]=='$') cnt_$++;
		}
	}
}
void pos_storage(void){
	int i,j,k=0;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(map[i][j][level]=='O'){
				stor_x[k]=j;
				stor_y[k]=i;
				k++;
			} 
		}
	}
}
void set_storage(void){
	int i;
	for(i=0; i<cnt_O; i++){
		//printf("%d %d\n",stor_x[i],stor_y[i]);
		if(map[stor_y[i]][stor_x[i]][level]=='.') map[stor_y[i]][stor_x[i]][level]='O';
	}
}
void print_map(void){
	int i,j;
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
}
void push_key(void){
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
		display_help();
                break;
            case 116:             //top
                break;
            default:
                break;
        }
}
void display_help(void){
	system("clear");

        printf("============도움말============\n");
	printf("h : 왼쪽, j : 아래, k : 위, l : 오른쪽\n");
	printf("u(undo) : 되돌리기(5번 가능)\n");
  	printf("r(replay) : 현재 맵을 처음부터 다시 시작\n");
	printf("n(new) : 첫 번째 맵부터 다시 시작\n");
	printf("e(exit) : 현재 게임 저장 후 종료\n");
	printf("s(save) : 현재 게임 저장\n");
	printf("f(file load) : 저장 시점부터 이어서 게임 시작\n");
	printf("t(top) : 게임 순위, t만 입력시 전체 순위, t + 맵숫자 입력시 해당 맵의 순위\n\n\n\n");
	printf("d를 한번 더 누르시면 게임 화면으로 돌아갑니다.\n");

	while(1){
	    key = getch();

	    switch (key)
	    {
            case 100:
		    	return;
			default:
		    	break;
	    }
	}
}




