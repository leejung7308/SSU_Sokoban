#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
//#include<conio.h>
#include<termios.h>
void load_map();						//맵 받기 
int getch(void);						//엔터없이 입력 
void left(int x, int y, int level);		//왼쪽 
void right(int x, int y, int level);	//오른쪽 
void up(int x, int y, int level);		//위 
void down(int x, int y, int level);		//아래 
void count_check(void);					//상자, 보관장소 개수 세기 
void position(void);					//@ 좌표 잡기 
void set_storage(void);					//보관장소 설정 
void pos_storage(void);					//보관장소 좌표 잡기 
void print_map(void);					//맵 출력 
void push_key(void);					//키 입력 
void display_help(void);				//도움말 

char map[30][30][6] = { 0 };			
int cnt = 0;
int level=1;
int pos_x, pos_y, cnt_O=0, cnt_$=0;
int stor_x[20]={0},stor_y[20]={0};
int key;

int main(){
	int i, j;
	load_map();
	while(1){						//게임 시작 
		system("clear");
		count_check();
		if(cnt_O!=cnt_$){										
			printf("Mismatch between box and storage count.\n");	//박스, 보관장소 개수 불일치 시 종료 
			return 0;
		}
		pos_storage();
		while(1){ 					//게임 진행 
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

        printf("============�룄���留�============\n");
	printf("h : �쇊履�, j : �븘�옒, k : �쐞, l : �삤瑜몄そ\n");
	printf("u(undo) : �릺�룎由ш린(5踰� 媛��뒫)\n");
  	printf("r(replay) : �쁽�옱 留듭쓣 泥섏쓬遺��꽣 �떎�떆 �떆�옉\n");
	printf("n(new) : 泥� 踰덉㎏ 留듬���꽣 �떎�떆 �떆�옉\n");
	printf("e(exit) : �쁽�옱 寃뚯엫 ����옣 �썑 醫낅즺\n");
	printf("s(save) : �쁽�옱 寃뚯엫 ����옣\n");
	printf("f(file load) : ����옣 �떆�젏遺��꽣 �씠�뼱�꽌 寃뚯엫 �떆�옉\n");
	printf("t(top) : 寃뚯엫 �닚�쐞, t留� �엯�젰�떆 �쟾泥� �닚�쐞, t + 留듭닽�옄 �엯�젰�떆 �빐�떦 留듭쓽 �닚�쐞\n\n\n\n");
	printf("d瑜� �븳踰� �뜑 �늻瑜댁떆硫� 寃뚯엫 �솕硫댁쑝濡� �룎�븘媛묐땲�떎.\n");

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




