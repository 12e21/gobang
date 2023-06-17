#include <graphics.h>       
#include <conio.h>

//global params
IMAGE img,whiteChessPic,blackChessPic;
int windowWide=640,windowHeight=480;
int tableStart=30,tableEnd=450,tableGap=30;
int tableGapCount=15;
int chessRadius=10;
int chessSituation[15][15]; // 0 represent blank; 1 represent white; 2 represent black
int cursorPosition[2]={0,0};
int cursorRadius=5;
int chessingSide=1; // 1 represent white side , -1 represent black side
char keyWord;

void loadImgs();
void drawChessTable();
void drawChess();
void drawCursor();
void draw();
void init();
void getKey();
void moveCursor();
void placeChess();
void processData();

void main()
{

	init();
	while(1){
		draw();
		getKey();
		processData();
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
void drawCursor(){
	setfillcolor(GREEN);
	fillcircle(tableStart+cursorPosition[0]*tableGap,tableStart+cursorPosition[1]*tableGap,cursorRadius);
}
void draw(){
	BeginBatchDraw();
	drawChessTable();
	drawChess();
	drawCursor();
	FlushBatchDraw();
}

void getKey(){
	keyWord=_getch();
}
void moveCursor(){
	switch(keyWord){
		case 'W':
		case 'w':
			if(cursorPosition[1]>=1){
				cursorPosition[1]--;
			}
			break;
		case 'S':
		case 's':
			if(cursorPosition[1]<=13){
				cursorPosition[1]++;
			}
			break;
		case 'A':
		case 'a':
			if(cursorPosition[0]>=1){
				cursorPosition[0]--;
			}
			break;
		case 'D':
		case 'd':
			if(cursorPosition[0]<=13){
				cursorPosition[0]++;
			}
			break;

	}
}
void placeChess(){
	if(keyWord==' '){
		if(chessSituation[cursorPosition[0]][cursorPosition[1]]==0){
			if(chessingSide==1){
				chessSituation[cursorPosition[0]][cursorPosition[1]]=1;
			}else if(chessingSide==-1){
				chessSituation[cursorPosition[0]][cursorPosition[1]]=2;
			}
			chessingSide=-chessingSide;
		}
	}
}

void processData(){
		moveCursor();
		placeChess();
}
