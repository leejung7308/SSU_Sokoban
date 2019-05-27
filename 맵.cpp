#include<stdio.h>
#include<stdlib.h>
#include<Windows.h>
char all_map[15][30][6] = { 0 };
char map[65][30]={0};
void load_map(int n){
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
	for(int i=0; i<15; i++){
			for(int j=0; j<30; j++){
				printf("%c",all_map[i][j][n]);
			}
			printf("\n");
		}
}
int main(){
	load_map(5);
	printf("ddd");
	return 0;	
}
