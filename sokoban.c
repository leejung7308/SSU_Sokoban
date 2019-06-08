#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
#include<termios.h>
void load_map(void);			//맵 받아오기                     
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
void check_$(void);			//보관장소에 상자 몇 개 들어갔는지 확인
void level_clear(void);			//클리어
void save_file(void);			//세이브
void load_file(void);			//로드
void input_name(void);			//닉네임 입력
void rank_save(void);			//랭킹 저장
void bubble_sort(void);			//버블 정렬(랭킹 정렬용)
void rank_view(int n);			//랭킹출력(t커맨드)
void rank_load(void);			//랭킹 로드

char name[100]={0};			//이름 배열
char rank_name[10][6][5]={0};		//랭킹에 들어가는 이름들 배열
char undo_arr[30][30][5]={0};		//undo하기 위해 이전행동들 넣어놓는 배열
char save_map[30][30]={0};		//저장파일에 저장하고 로드할 맵 진행상황
char map[30][30][6]={0};		//맵 배열
char c_map[30][30]={0};			//current_map(현재 맵)
int rank_cnt[6][5]={0};			//랭킹에 들어가는 이동횟수 배열
int cnt = 0;				//이동횟수
int level=1;				//레벨
int pos_x, pos_y, cnt_O=0, cnt_$=0;	//@ 좌표 및 상자, 보관장소 개수
int stor_x[20]={0},stor_y[20]={0};	//보관장소 좌표 배열
int key;				//입력 받는 키
int undo_cnt=5;				//undo 개수 제한
int left_$;				//남은 상자 개수

int main(){
	int i, j, k;
	if(name[0]==0){
		input_name();
		rank_load();
	}
	load_map();
	for(k=0;k<5;k++){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				undo_arr[i][j][k]=map[i][j][level];
			}
		}
	}	
	system("clear");
	current_map();
	count_check();
	if(cnt_O!=cnt_$){	//상자와 창고 개수 불일치 시 오류메세지 출력 후 종료
		printf("Mismatch between box and storage count.\n"); 
		return 0;
	}	
	pos_storage();
	while(1){  		//게임 진행
		if(level==6){	
			printf("-----------CLEAR!!!!!!------------\n");
			return 0;
		}
		position();
		print_map();
		check_$();
		printf("COUNT : %d\nUndo : %d\nBox left : %d\n", cnt, undo_cnt,(cnt_$-left_$));
        push_key();
        if(key=='e'){
        	system("clear");
        	printf("SEE YOU ....\n");
        	return 0;
		}
        set_storage();
		system("clear");
	}
	return 0;	
}
void load_map(void){
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
		if(c_map[stor_y[i]][stor_x[i]]=='.') c_map[stor_y[i]][stor_x[i]]='O';
	}
}
void print_map(void){
	int i,j;
	printf("HELLO ");
	for(i=0;name[i]!=0;i++)
		printf("%c",name[i]);
	printf("!!\n");
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
        if (key == 't'){
            key = getch();
            if (key == '\n'){
                rank_view(0);                
            }
            else if (key == '1'){
                key = getch();
                if (key == '\n'){
                    rank_view(1);                    
                }
                else ;
            }
            else if (key == '2'){
                key = getch();
                if (key == '\n'){
                    rank_view(2);                    
                }
                else ;
            }
            else if (key == '3'){
                key = getch();
                if (key == '\n'){
                    rank_view(3);                    
                }
                else ;
            }
            else if (key == '4'){
                key = getch();
                if (key == '\n'){
                    rank_view(4);                    
                }
                else ;
            }
            else if (key == '5'){
                key = getch();
                if (key == '\n'){
                    rank_view(5);                    
                }
                else ;
            }
            else ;
        }
        else if (key == 'h'){
            move(pos_x,pos_y);           //left            
        }
        else if (key == 'l'){
            move(pos_x,pos_y);           //right            
        }
        else if (key == 'k'){
            move(pos_x,pos_y);           //up            
        }
        else if (key == 'j'){
            move(pos_x,pos_y);           //down            
        }
        else if (key == 'u'){            //undo
            if(undo_cnt>0) undo();
            else ;            
        }
        else if (key == 'r'){            //replay
            restart();  
        }
        else if (key == 'n'){            //new
            new_start();
        }
        else if (key == 'e'){            //exit
            save_file(); 
        }
        else if (key == 's'){            //save
            save_file(); 
        }
        else if (key == 'f'){            //file load
            load_file();   
        }
        else if (key == 'd'){            //display help
            display_help();
        }
        else if (key == '1'){
            level_clear();  
        }
        else ;     
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
	printf("t(top) : 게임 순위, t만 입력시 전체 순위, t + 맵숫자 입력시 해당 맵의 순위\n");
	while(1){
	    key = getch();
		if(key=='\n') return ;
		else ;
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
	cnt++;
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
}
void check_$(void){
	int i; 
	left_$=0;
	for(i=0; i<cnt_O; i++){
		if(c_map[stor_y[i]][stor_x[i]]=='$') left_$++;
	}
	if(left_$==cnt_$) level_clear();
}
void level_clear(void){
	int i, j, k;
	cnt_O=0,cnt_$=0;
	undo_cnt=5;
	for(i=0; i<20; i++){
		stor_x[i]=0;
		stor_y[i]=0;
	}
	rank_cnt[5][level-1]=cnt;
	for(i=0; i<10; i++)
		rank_name[i][5][level-1]=name[i];
	cnt=0;
	rank_save();
	level++;
	main();
}
void save_file(void){
	FILE*f;
	int i,j;
	f = fopen("sokoban.txt", "wt");
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			save_map[i][j]=c_map[i][j];
		}
	}
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			fprintf(f,"%c",save_map[i][j]);
		}
	}
	fprintf(f, "\n%d\n%d\n%d\n%d\n%d", level,cnt,undo_cnt,cnt_$,cnt_O);
	for(i=0;name[i]!=0;i++)
		fprintf(f,"%c",name[i]);
	fclose(f);
}
void load_file(void){
	FILE*f;
	int i, j, k;
	int a[5]={0};
	char trash;
	f=fopen("sokoban.txt","r");
	for(i=0; i<30; i++){
		for(j=0; j<30; j++){
			fscanf(f,"%c",&save_map[i][j]);
		}
	}
	for(i=0; i<5; i++){
		fscanf(f,"%d",&a[i]);
	}
	level=a[0];
	cnt=a[1];
	undo_cnt=a[2];
	cnt_$=a[3];
	cnt_O=a[4];
	for(i=0; name[i]!=0; i++)
		name[i]=0;
	for(i=0;fscanf(f,"%c",&name[i])!=EOF;i++) ;
	for(i=0;i<30;i++){
		for(j=0;j<30;j++){
			c_map[i][j]=save_map[i][j];
		}
	}
	for(k=0;k<5;k++){
		for(i=0;i<30;i++){
			for(j=0;j<30;j++){
				undo_arr[i][j][k]=c_map[i][j];
			}
		}
	}
	pos_storage();
}
void input_name(void)
{
	int i,j;
	while(1){
		system("clear");
		printf("Input name : ");
		scanf("%s", &name);
		for(i=0; name[i]!=0; i++) ;
		if(i<=10) break;
		else {
			printf("Enter your name under 10 letters.\n");
			Sleep(500);
		}
	}
}
void rank_save(void){
	int i, j, k;
	FILE*f=fopen("ranking.txt","wt");
	bubble_sort();
	for(k=1; k<6; k++){	
		fprintf(f,"map%d\n",k);
		for(i=0; i<5; i++){
			for(j=0; j<10; j++){
				if(rank_name[0][i][k-1]==0) break;
				fprintf(f,"%c",rank_name[j][i][k-1]);
			}
			if(rank_name[0][i][k-1]!=0)
				fprintf(f,"\t%d\n",rank_cnt[i][k-1]);
		}
	}
	fclose(f);
}
void bubble_sort(void){
	int i, j, k, l, temp, tname[10];
	for(i=0;i<5;i++){
		for(j=0;j<6;j++){
			if(rank_name[0][j][i]==0) rank_cnt[j][i]=9999999;
		}
	}
	for(k=0; k<5; k++){	
		for(i=0; i<5; i++){
			for(j=0; j<5; j++){
				if(rank_cnt[j][k]>rank_cnt[j+1][k]){
					temp=rank_cnt[j][k];
					rank_cnt[j][k]=rank_cnt[j+1][k];
					rank_cnt[j+1][k]=temp;
					for(l=0; l<10; l++){
						tname[l]=rank_name[l][j][k];
					}
					for(l=0; l<10; l++){
						rank_name[l][j][k]=rank_name[l][j+1][k];
					}
					for(l=0; l<10; l++){
						rank_name[l][j+1][k]=tname[l];
					}
				}
			}
		}
	}
}
void rank_view(int n){
	int i,j,k;
	system("clear");
	FILE*f;
	switch(n){
		case 0:
			for(k=1; k<6; k++){	
				printf("map%d\n",k);
				for(i=0; i<5; i++){
					for(j=0; j<10; j++){
						if(rank_name[0][i][k-1]==0) break;
						printf("%c",rank_name[j][i][k-1]);
					}
					if(rank_name[0][i][k-1]!=0)
						printf("\t%d\n",rank_cnt[i][k-1]);
				}
			}
			break;
		case 1:
			printf("map1\n");
			for(i=0; i<5; i++){
				for(j=0; j<10; j++){
					if(rank_name[0][i][0]==0) break;
					printf("%c",rank_name[j][i][0]);
				}
				if(rank_name[0][i][0]!=0)
					printf("\t%d\n",rank_cnt[i][0]);
			}
			break;
		case 2:
			printf("map2\n");
			for(i=0; i<5; i++){
				for(j=0; j<10; j++){
					if(rank_name[0][i][1]==0) break;
					printf("%c",rank_name[j][i][1]);
				}
				if(rank_name[0][i][1]!=0)
					printf("\t%d\n",rank_cnt[i][1]);
			}
			break;
		case 3:
			printf("map3\n");
			for(i=0; i<5; i++){
				for(j=0; j<10; j++){
					if(rank_name[0][i][2]==0) break;
					printf("%c",rank_name[j][i][2]);
				}
				if(rank_name[0][i][2]!=0)
					printf("\t%d\n",rank_cnt[i][2]);
			}
			break;
		case 4:
			printf("map4\n");
			for(i=0; i<5; i++){
				for(j=0; j<10; j++){
					if(rank_name[0][i][3]==0) break;
					printf("%c",rank_name[j][i][3]);
				}
				if(rank_name[0][i][3]!=0)
					printf("\t%d\n",rank_cnt[i][3]);
			}
			break;
		case 5:
			printf("map5\n");
			for(i=0; i<5; i++){
				for(j=0; j<10; j++){
					if(rank_name[0][i][4]==0) break;
					printf("%c",rank_name[j][i][4]);
				}
				if(rank_name[0][i][4]!=0)
					printf("\t%d\n",rank_cnt[i][4]);
			}
			break;
		default:
			break;
	}
	while(1){
	    key = getch();
		if (key == '\n') return;
        else ;
	}
}
void rank_load(){
    FILE *f=fopen("ranking.txt","r");
    int i=0, j=0, k=0, pre_cnt;
    char pre_name[10]={0},a;
    while(!feof(f)){
    	fscanf(f,"%s",&pre_name);
    	if(pre_name[0]=='m' && pre_name[1]=='a' && pre_name[2]=='p'){
    		if(pre_name[3]=='1') continue;
    		else if(pre_name[3]=='2' || pre_name[3]=='3' || pre_name[3]=='4' || pre_name[3]=='5'){
    			k++;
    			j=0;
    			continue;
			}
		}
        else ;
    	fscanf(f,"%d",&pre_cnt);
    	for(i=0; i<10; i++){				    
            rank_name[i][j][k]=pre_name[i];
		}
		rank_cnt[j][k]=pre_cnt;
		j++;
	}
	rank_cnt[j-1][4]=9999999;
	for(i=0; i<10; i++){
		rank_name[i][j-1][4]=0;
	}
}
