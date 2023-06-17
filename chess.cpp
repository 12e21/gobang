#include <graphics.h>       
#include <conio.h>

//global params
IMAGE img,whiteChessPic,blackChessPic;
int windowWide=640,windowHeight=480;
int tableStart=30,tableEnd=450,tableGap=30;
int tableGapCount=15;
int chessRadius=10;
int chessSituation[15][15]; // 0 represent blank; 1 represent white; 2 represent black

void loadImgs();
void drawChessTable();
void drawChess();
void draw();
void init();


void main()
{

	init();
	while(1){
		draw();
		Sleep(0.1);
	}

	getch();

}

void loadImgs(){

	//load the  chess table pic and chess pic
	loadimage(&img,"img\\back.jpg",windowWide,windowHeight);

}

void drawChessTable(){
	//put table pic
	putimage(0, 0, &img);

	//draw chess line
	for(int x=tableStart;x<=tableEnd;x+=tableGap){
		line(x,tableStart,x,tableEnd);
		line(tableStart,x,tableEnd,x);
	}
}

void init(){
    initgraph(windowWide, windowHeight);
	loadImgs();

}

void drawChess(){
	for(int i=0;i<tableGapCount;i++){
		for(int j=0;j<tableGapCount;j++){

			if(chessSituation[i][j]==1){
				setfillcolor(WHITE);
				fillcircle(tableStart+i*tableGap,tableStart+j*tableGap,chessRadius);
			}else if(chessSituation[i][j]==2){
				setfillcolor(BLACK);
				fillcircle(tableStart+i*tableGap,tableStart+j*tableGap,chessRadius);
			}
		}
	}
}

void draw(){
	BeginBatchDraw();
	drawChessTable();
	drawChess();
	FlushBatchDraw();
}
