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
int placedchess[3]={0,0,0}; // form 2 means row and column ,last num means chess(1 white ,2 black)
char keyWord;


//status params
int winner=0; // 0 means none, 1 means white wins, 2 means black wins

void loadImgs();
void drawChessTable();
void drawChess();
void drawCursor();
void draw();
void init();
void getKey();
void moveCursor();
void placeChess();
void checkWin();
void processData();


void main()
{

	init();
	while(1){
		draw();
		getKey();
		processData();
		if(winner!=0){
			return;
		}
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
				placedchess[0]=cursorPosition[0];
				placedchess[1]=cursorPosition[1];
				placedchess[2]=1;
			}else if(chessingSide==-1){
				chessSituation[cursorPosition[0]][cursorPosition[1]]=2;
				placedchess[0]=cursorPosition[0];
				placedchess[1]=cursorPosition[1];
				placedchess[2]=2;
			}
			chessingSide=-chessingSide;
		}
	}
}

void checkWin(){
	
	int continueCount=1;
	int ifEnd=false;
	int ifReverseEnd=false;
	int sequentIndex=1;
	int ReverseIndex=1;
	
	//row check
	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if((placedchess[0]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[0]-sequentIndex>=0)&&(chessSituation[placedchess[0]-sequentIndex][placedchess[1]]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}

	//column check
	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if((placedchess[1]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]][placedchess[1]+sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[1]-sequentIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}

	//slope check
	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if(((placedchess[0]+sequentIndex<tableGapCount)&&(placedchess[1]+sequentIndex<tableGapCount))&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]+sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[0]-sequentIndex>=0)&&(placedchess[1]-sequentIndex>=0))&&(chessSituation[placedchess[0]-sequentIndex][placedchess[1]-sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}

	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if(((placedchess[1]+sequentIndex<tableGapCount)&&(placedchess[0]-sequentIndex>=0))&&(chessSituation[placedchess[0]-sequentIndex][placedchess[1]+sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[1]-sequentIndex>=0)&&(placedchess[0]+sequentIndex<tableGapCount))&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]-sequentIndex]==placedchess[2])){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					winner=placedchess[2];
					return;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}
}

void processData(){
		moveCursor();
		placeChess();
		checkWin();
}
