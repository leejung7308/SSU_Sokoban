#include<stdio.h>
#include<conio.h>
#include<stdlib.h>
#include<windows.h>
char map[13][30]={0};
int pos_x,pos_y,pos_0x[6]={0},pos_0y[6]={0};
int cnt=0;
void right(int x, int y);
void left(int x, int y);
void down(int x, int y);
void up(int x, int y);
int main(){
	char a,key;
	int x=0, y=0, i, j, t=0;
	FILE *f=fopen("C:\\Users\\user\\Desktop\\이정꺼\\작업환경\\C\\map","r");
	while(fscanf(f,"%c",&a)!=EOF){
		if(a=='2') break;
		if(a=='\n'){
			y++;
			x=0;
		}
		else{
			map[y][x]=a;
			x++;
		}
	}
	while(1){
		for(int k=0; k<t; k++){
			//printf("\n%d %d",pos_0x[k],pos_0y[k]);
			if(map[pos_0y[k]][pos_0x[k]]=='.')
				map[pos_0y[k]][pos_0x[k]]='O';
		}
		t=0;
		for(i=0; i<13; i++){
			for(j=0; j<30; j++){
				printf("%c",map[i][j]);
				if(map[i][j]=='@') pos_x=j, pos_y=i;
				if(map[i][j]=='O'){
					pos_0x[t]=j;
					pos_0y[t]=i;
					t++;
				}
			}
			printf("\n");
		}
		
		printf("\nX : %d Y : %d COUNT : %d\n",pos_x,pos_y,cnt);
		while(1){
			scanf("%c",&key);
			if(key=='l'){ 
				right(pos_x,pos_y);
				break;
			}
			else if(key=='h'){ 
				left(pos_x,pos_y);
				break;
			}
			else if(key=='j'){ 
				down(pos_x,pos_y);
				break;
			}
			else if(key=='k'){ 
				up(pos_x,pos_y);
				break;
			}
			else ;
		}
		system("cls");
		
	}
	return 0;
}
void right(int x, int y){
	if(map[y][x+1]=='#') ;
	else{
		if(map[y][x+1]=='$'){
			if(map[y][x+2]=='#'||map[y][x+2]=='$') ;
			else{
				map[y][x+2]='$';
				map[y][x+1]='@';
				map[y][x]='.';
				cnt++;
			}
		}
		else{
			map[y][x+1]='@';
			map[y][x]='.';
			cnt++;
		}
	}
}
void left(int x, int y){
	if(map[y][x-1]=='#') ;
	else{
		if(map[y][x-1]=='$'){
			if(map[y][x-2]=='#'||map[y][x-2]=='$') ;
			else{
				map[y][x-2]='$';
				map[y][x-1]='@';
				map[y][x]='.';
				cnt++;
			}
		}
		else{
			map[y][x-1]='@';
			map[y][x]='.';
			cnt++;
		}
	}
}
void down(int x, int y){
	if(map[y+1][x]=='#') ;
	else{
		if(map[y+1][x]=='$'){
			if(map[y+2][x]=='#'||map[y+2][x]=='$') ;
			else{
				map[y+2][x]='$';
				map[y+1][x]='@';
				map[y][x]='.';
				cnt++;
			}
		}
		else{
			map[y+1][x]='@';
			map[y][x]='.';
			cnt++;
		}
	}
}
void up(int x, int y){
	if(map[y-1][x]=='#') ;
	else{
		if(map[y-1][x]=='$'){
			if(map[y-2][x]=='#'||map[y-2][x]=='$') ;
			else{
				map[y-2][x]='$';
				map[y-1][x]='@';
				map[y][x]='.';
				cnt++;
			}
		}
		else{
			map[y-1][x]='@';
			map[y][x]='.';
			cnt++;
		}
	}
}

