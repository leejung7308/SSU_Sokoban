#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
//#include<conio.h>
#include<termios.h>
void load_map();						//¸Ê ¹Ş±â 
int getch(void);						//¿£ÅÍ¾øÀÌ ÀÔ·Â 
void left(int x, int y, int level);		//¿ŞÂÊ 
void right(int x, int y, int level);	//¿À¸¥ÂÊ 
void up(int x, int y, int level);		//À§ 
void down(int x, int y, int level);		//¾Æ·¡ 
void count_check(void);					//»óÀÚ, º¸°üÀå¼Ò °³¼ö ¼¼±â 
void position(void);					//@ ÁÂÇ¥ Àâ±â 
void set_storage(void);					//º¸°üÀå¼Ò ¼³Á¤ 
void pos_storage(void);					//º¸°üÀå¼Ò ÁÂÇ¥ Àâ±â 
void print_map(void);					//¸Ê Ãâ·Â 
void push_key(void);					//Å° ÀÔ·Â 
void display_help(void);				//µµ¿ò¸» 

char map[30][30][6] = { 0 };			
int cnt = 0;
int level=1;
int pos_x, pos_y, cnt_O=0, cnt_$=0;
int stor_x[20]={0},stor_y[20]={0};
int key;

int main(){
	int i, j;
	load_map();
	while(1){						//°ÔÀÓ ½ÃÀÛ 
		system("clear");
		count_check();
		if(cnt_O!=cnt_$){										
			printf("Mismatch between box and storage count.\n");	//¹Ú½º, º¸°üÀå¼Ò °³¼ö ºÒÀÏÄ¡ ½Ã Á¾·á 
			return 0;
		}
		pos_storage();
		while(1){ 					//°ÔÀÓ ÁøÇà 
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

        printf("============ë„ì›€ë§============\n");
	printf("h : ì™¼ìª½, j : ì•„ë˜, k : ìœ„, l : ì˜¤ë¥¸ìª½\n");
	printf("u(undo) : ë˜ëŒë¦¬ê¸°(5ë²ˆ ê°€ëŠ¥)\n");
  	printf("r(replay) : í˜„ì¬ ë§µì„ ì²˜ìŒë¶€í„° ë‹¤ì‹œ ì‹œì‘\n");
	printf("n(new) : ì²« ë²ˆì§¸ ë§µë¶€í„° ë‹¤ì‹œ ì‹œì‘\n");
	printf("e(exit) : í˜„ì¬ ê²Œì„ ì €ì¥ í›„ ì¢…ë£Œ\n");
	printf("s(save) : í˜„ì¬ ê²Œì„ ì €ì¥\n");
	printf("f(file load) : ì €ì¥ ì‹œì ë¶€í„° ì´ì–´ì„œ ê²Œì„ ì‹œì‘\n");
	printf("t(top) : ê²Œì„ ìˆœìœ„, të§Œ ì…ë ¥ì‹œ ì „ì²´ ìˆœìœ„, t + ë§µìˆ«ì ì…ë ¥ì‹œ í•´ë‹¹ ë§µì˜ ìˆœìœ„\n\n\n\n");
	printf("dë¥¼ í•œë²ˆ ë” ëˆ„ë¥´ì‹œë©´ ê²Œì„ í™”ë©´ìœ¼ë¡œ ëŒì•„ê°‘ë‹ˆë‹¤.\n");

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




