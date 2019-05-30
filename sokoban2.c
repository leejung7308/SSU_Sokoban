#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
//#include<conio.h>
#include<termios.h>
void load_map();			//맵 받아오기                     
int getch(void); 			//엔터 없이 입력
void count_check(void); 		//상자, 보관장소
void position(void); 			//@ 좌표잡기
void set_storage(void); 		//보관장소 놓기
void pos_storage(void); 		//보관장소 좌표
void print_map(void); 			//맵 출력
void push_key(void); 			// 키 입력
void display_help(void); 		//도움말'
void current_map(void);			//현재 레벨
void restart(void);			//재시도(현재레벨)
void new_start(void);			//새 게임
void move(int x, int y);		//움직임
void save_undo(void);			//undo 배열 저장
void undo(void);			//undo
void check_$(void);			//보관장소에 상자 몇개 들어갔는지
void level_clear(void);			//클리어
void save_file(void);			//세이브
void save_map(void);			//
void file_load(void);			//로드

char undo_arr[30][30][5]={0};
char map[30][30][6]={0};		//맵 배열
char c_map[30][30]={0};
int cnt = 0;				//이동횟수
int level=2;				//레벨
int pos_x, pos_y, cnt_O=0, cnt_$=0;	//@ 좌표 및 상자, 보관장소 개수
int stor_x[20]={0},stor_y[20]={0};	//보관장소 좌표 배열
int key;				//입력 받는 키
int undo_cnt=5;			//
int left_$;

int main(){
	int i, j;
	load_map();
	system("clear");
	current_map();
	count_check();
	if(cnt_O!=cnt_$){
		printf("Mismatch between box and storage count.\n"); 
		return 0;
	}
	pos_storage();
	while(1){  		//게임 진행
		position();
		print_map();
		printf("COUNT : %d\nUndo : %d\nBox left : %d\n", cnt, undo_cnt,(cnt_$-left_$));
		check_$();
		//printf("%d %d\n%d %d\n",cnt_O,cnt_$,pos_x,pos_y);  
        push_key();
	if (key == 's')
		return 0;
        set_storage();
		system("clear");
	}
	return 0;	
}

void load_map(){
	int i,j;
	FILE *f=fopen("map.txt","r");
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
void move(int x,int y){
	int dx=0,dy=0;
	if(key=='l') dx=1;
	else if(key=='h') dx=-1;
	else if(key=='k') dy=-1;
	else dy=1;
	cnt++;
	save_undo();
	if(c_map[y+dy][x+dx]=='#') ;
	else{
		if(c_map[y+dy][x+dx]=='$'){
			if(c_map[y+dy*2][x+dx*2]=='#'||c_map[y+dy*2][x+dx*2]=='$') ;
			else{
				c_map[y+dy*2][x+dx*2]='$';
				c_map[y+dy][x+dx]='@';
				c_map[y][x]='.';
			}
		}
		else{
			c_map[y+dy][x+dx]='@';
			c_map[y][x]='.';
		}
	}
}
void position(void){
	int i,j;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(c_map[i][j]=='@'){
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
			if(c_map[i][j]=='O') cnt_O++;
			if(c_map[i][j]=='$') cnt_$++;
		}
	}
}
void pos_storage(void){
	int i,j,k=0;
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			if(c_map[i][j]=='O'){
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
		if(c_map[stor_y[i]][stor_x[i]]=='.') c_map[stor_y[i]][stor_x[i]]='O';
	}
}
void print_map(void){
	int i,j;
	printf("stage");
	for(i=0; i<30; i++){
		char end[30]={0};
		for(j=0; j<30; j++){
			printf("%c",c_map[i][j]);
			end[j]=c_map[i][j];
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
            move(pos_x,pos_y);            //left
            break;
        case 108:
            move(pos_x,pos_y);           //right
            break;
        case 107:
            move(pos_x,pos_y);              //up
            break;
        case 106:
            move(pos_x,pos_y);            //down
            break;
        case 117:             //undo
            if(undo_cnt>0) undo();
            else ;
            break;
        case 114:             //replay
            restart();
            break;
        case 110:             //new
            new_start();
            break;
        case 101:             //exit
            break;
        case 115:             //save
	    save_file();
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
void restart(void){
	current_map();
}
void current_map(void){
	int i,j;
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			c_map[i][j]=map[i][j][level];
		}
	}
}
void new_start(void){
	int i;
	level=1;
	cnt=0;
	undo_cnt=5;
	for(i=0; i<20; i++){
		stor_x[i]=0;
		stor_y[i]=0;
	}
	cnt_O=0;
	cnt_$=0;
	main();
}
void save_undo(void){
	int i,j,k;
	/*for(k=0;k<5;k++){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				undo_arr[i][j][k]=map[i][j][level];
			}
		}
	}*/
	for(k=3;k>-1;k--){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				undo_arr[i][j][k+1]=undo_arr[i][j][k];
			}
		}
	}
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			undo_arr[i][j][0]=c_map[i][j];
		}
	}	
}
void undo(void){
	int i,j,k;
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			c_map[i][j]=undo_arr[i][j][0];
		}
	}
	for(k=0;k<4;k++){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				undo_arr[i][j][k]=undo_arr[i][j][k+1];
			}
		}
	}
	undo_cnt--;
	/*for(k=0;k<5;k++){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				printf("%c",undo_arr[i][j][k]);
			}
			printf("\n");
		}
		printf("\n");
	}
	scanf("%d",&i);*/
}
void check_$(void){
	int i; 
	left_$=0;
	for(i=0; i<cnt_O; i++){
		//printf("%d %d\n",stor_x[i],stor_y[i]);
		if(c_map[stor_y[i]][stor_x[i]]=='$') left_$++;
	}
	printf("%d %d\n",left_$,cnt_$);
	if(left_$==cnt_$) level_clear();
}
void level_clear(void){
	int i;
	cnt_O=0,cnt_$=0,cnt=0;
	undo_cnt=5;
	for(i=0; i<20; i++){
		stor_x[i]=0;
		stor_y[i]=0;
	}
	if(level<5) level++;
	else level=1;
	main();
}
void save_file(void){
	FILE*f;
	f = fopen("sokoban.txt", "wt");
	fprintf(f, "stage%d\n", level);
	fprintf(f, "COUNT : %d\n", cnt);
	fprintf(f, "Undo : %d\n", undo_cnt);
	fprintf(f, "Box left : %d\n", (cnt_$-left_$));
	fprintf(f, "%d %d\n", left_$, cnt_$);
	fclose(f);
	return;
}



