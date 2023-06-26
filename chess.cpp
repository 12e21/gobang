#include <graphics.h>       
#include <conio.h>
#include <iostream>
#include <fstream>
using namespace std;

//global params
IMAGE img,whiteChessPic,blackChessPic;
int windowWide=640,windowHeight=480;
int tableStart=30,tableEnd=450,tableGap=30;
int tableGapCount=15;
int chessRadius=10;
int chessSituation[15][15]; // 0 represent blank; 1 represent white; 2 represent black
int cursorPosition[2]={0,0};
int cursorRadius=5;
int chessingSide=-1; // 1 represent white side , -1 represent black side
int placedchess[3]={0,0,0}; // form 2 means row and column ,last num means chess(1 white ,2 black)
int winnerTextPos[2]={460,50};
char keyWord;
int chessSituationRecord[300][15][15];
int chessSituationRecordIndex=1;
bool ifRegretChess=false;
bool ifAi=true;
int aiSide=1;
bool ifForbid=false;
bool ifSaveEndgame=false;
bool ifLoadEndgame=false;


//status params
int winner=0; // 0 means none, 1 means white wins, 2 means black wins
int forbid=0; //0 means no forbid, 1 means long forbid ,2 means three forbid

void loadImgs();
void drawChessTable();
void drawChess();
void drawCursor();
void draw();
void init();
void getKey();
void dealInput();
void moveCursor();
void placeChess();
int checkLongPossible();
void aiPlaceChess();
void storeChessSituation();
void regretChess();
bool checkForbidLong();
bool checkForbidThree();
void checkForbid();
void checkWin();
void processData();
void drawStatus();
void saveEndgame();
void loadEndgame();
void reinit();

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

void reinit(){

	chessingSide=-1;
	chessSituationRecordIndex=1;
	ifRegretChess=false;
	ifAi=true;
	aiSide=1;
	ifForbid=false;
	ifSaveEndgame=false;
	ifLoadEndgame=false;
	winner=0;
	forbid=0;

		
	cursorPosition[0]=0;
	cursorPosition[1]=0;

	placedchess[0]=0;
	placedchess[1]=0;
	placedchess[2]=0;


	for(int i=0;i<15;i++){
		for(int j=0;j<15;j++){
			chessSituation[i][j]=0;
		}
	}

	for(int m=0;m<300;m++){
		for(int n=0;n<15;n++){
			for(int k=0;k<15;k++){
				chessSituationRecord[m][n][k]=0;
			}
		}
	}

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


void drawStatus(){
	if(winner==1){
		outtextxy(winnerTextPos[0],winnerTextPos[1],"winner: white");
	}else if(winner==2){
		outtextxy(winnerTextPos[0],winnerTextPos[1],"winner: black");
	}else{
		outtextxy(winnerTextPos[0],winnerTextPos[1],"winner: none");
	}
	if(ifAi==true){
		outtextxy(winnerTextPos[0],winnerTextPos[1]+20,"AI(Y): TRUE");
	}else{
		outtextxy(winnerTextPos[0],winnerTextPos[1]+20,"AI(Y): FALSE");
	}
	outtextxy(winnerTextPos[0],winnerTextPos[1]+40,"SAVE(R)");
	outtextxy(winnerTextPos[0],winnerTextPos[1]+60,"LOAD(T)");
	outtextxy(winnerTextPos[0],winnerTextPos[1]+80,"REGRET(B) shut down ai");
	outtextxy(winnerTextPos[0],winnerTextPos[1]+100,"RESTART(H)");
}


void draw(){
	BeginBatchDraw();
	drawChessTable();
	drawChess();
	drawCursor();
	drawStatus();
	FlushBatchDraw();
}

void getKey(){
	keyWord=_getch();
}

void dealInput()
{
	if(keyWord=='y'||keyWord=='Y')
	{
		if(ifAi==true){
			ifAi=false;
		}else{
			ifAi=true;
		}
	}
	if(keyWord=='r'||keyWord=='R')
	{
		ifSaveEndgame=true;
	}
	if(keyWord=='t'||keyWord=='T')
	{
		ifLoadEndgame=true;
	}
	if(keyWord=='b'||keyWord=='B'){
		ifRegretChess=true;
	}
	if(keyWord=='h'||keyWord=='H'){
		reinit();
	}
	moveCursor();

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


void aiPlaceChess(){
	int maxRow=0;
	int maxColumn=0;
	int maxLong=0;

	for(int i=0;i<tableGapCount;i++){
		for(int j=0;j<tableGapCount;j++){
			if(chessSituation[i][j]==0){
				placedchess[0]=i;
				placedchess[1]=j;
				if(chessingSide==1){
					placedchess[2]=1;
				}else{
					placedchess[2]=2;			
				}
				if(checkLongPossible()>maxLong){
					maxRow=i;
					maxColumn=j;
					maxLong=checkLongPossible();
				}
			}
		}
	}
	
	placedchess[0]=maxRow;
	placedchess[1]=maxColumn;
	if(chessingSide==1){
		placedchess[2]=1;
	}else{
		placedchess[2]=2;			
	}
	chessSituation[maxRow][maxColumn]=placedchess[2];

	chessingSide=-chessingSide;
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
	}else{
		placedchess[2]=0;
	}
}


void storeChessSituation(){

	if(placedchess[2]!=0){
		memcpy(chessSituationRecord[chessSituationRecordIndex],chessSituation,sizeof(chessSituation));
		chessSituationRecordIndex++;
	}
}

void regretChess(){
	if(ifRegretChess==true&&chessSituationRecordIndex>=2){
		memcpy(chessSituation,chessSituationRecord[chessSituationRecordIndex-2],sizeof(chessSituation));
		chessSituationRecordIndex--;
		chessingSide=-chessingSide;
		winner=0;
		ifRegretChess=false;
	}
}


bool checkForbidLong()
{
	if(placedchess[2]==2){
			
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
				if(continueCount>=6){
					return true;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[0]-ReverseIndex>=0)&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=6){
					return true;
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
				if(continueCount>=6){
					return true;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[1]-ReverseIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=6){
					return true;
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
				if(continueCount>=6){
					return true;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[0]-ReverseIndex>=0)&&(placedchess[1]-ReverseIndex>=0))&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=6){
					return true;
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
				if(continueCount>=6){
					return true;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[1]-ReverseIndex>=0)&&(placedchess[0]+ReverseIndex<tableGapCount))&&(chessSituation[placedchess[0]+ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=6){
					return true;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}
	}
	return false;
}
bool checkForbidThree()
{
	if(placedchess[2]==2){
	int continueThreeCount=0;
			
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
				if(continueCount>=3){
					continueThreeCount++;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[0]-ReverseIndex>=0)&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=3){
					continueThreeCount++;
					break;
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
				if(continueCount>=3){
					continueThreeCount++;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[1]-ReverseIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=3){
					continueThreeCount++;
					break;
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
				if(continueCount>=3){
					continueThreeCount++;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[0]-ReverseIndex>=0)&&(placedchess[1]-ReverseIndex>=0))&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=3){
					continueThreeCount++;
					break;
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
				if(continueCount>=3){
					continueThreeCount++;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[1]-ReverseIndex>=0)&&(placedchess[0]+ReverseIndex<tableGapCount))&&(chessSituation[placedchess[0]+ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=3){
					continueThreeCount++;
					break;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}

	if(continueThreeCount>=2){
		return true;
	}
	}

	return false;
}
void checkForbid(){
	if(checkForbidLong()){
		forbid=1;
	}
	if(checkForbidThree()){
		forbid=2;
	}
	if(forbid!=0){
		winner=1;
	}

}
int checkLongPossible(){
	int longPossibleContinue=0;
	
	bool ifFivePossible=false;
	int continueCount=1;
	int ifEnd=false;
	int ifReverseEnd=false;
	int sequentIndex=1;
	int ReverseIndex=1;
	
	//row check
	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if((placedchess[0]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]]==placedchess[2]||chessSituation[placedchess[0]+sequentIndex][placedchess[1]]==0)){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[0]-ReverseIndex>=0)&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]]==placedchess[2]||chessSituation[placedchess[0]+sequentIndex][placedchess[1]]==0)){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}

	
	if(ifFivePossible==true){
		int continueCount=1;
		int ifEnd=false;
		int ifReverseEnd=false;
		int sequentIndex=1;
		int ReverseIndex=1;

		while(ifEnd==false||ifReverseEnd==false){
			if(ifEnd==false){
				if((placedchess[0]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]]==placedchess[2])){
					sequentIndex++;
					continueCount++;	
				}else{
					ifEnd=true;
				}
			}


			if(ifReverseEnd==false){
				if((placedchess[0]-ReverseIndex>=0)&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]]==placedchess[2])){
					ReverseIndex++;
					continueCount++;
				}else{
					ifReverseEnd=true;
				}
			}
		}

		if(continueCount>longPossibleContinue){
			longPossibleContinue=continueCount;
		}

	}
	


	//column check
	ifFivePossible=false;
	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if((placedchess[1]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]][placedchess[1]+sequentIndex]==placedchess[2]||chessSituation[placedchess[0]][placedchess[1]+sequentIndex]==0)){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if((placedchess[1]-ReverseIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-ReverseIndex]==placedchess[2]||chessSituation[placedchess[0]][placedchess[1]+sequentIndex]==0)){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}


	if(ifFivePossible==true){
		int continueCount=1;
		int ifEnd=false;
		int ifReverseEnd=false;
		int sequentIndex=1;
		int ReverseIndex=1;
		

		while(ifEnd==false||ifReverseEnd==false){
			if(ifEnd==false){
				if((placedchess[1]+sequentIndex<tableGapCount)&&(chessSituation[placedchess[0]][placedchess[1]+sequentIndex]==placedchess[2])){
					sequentIndex++;
					continueCount++;
				}else{
					ifEnd=true;
				}
			}


			if(ifReverseEnd==false){
				if((placedchess[1]-ReverseIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-ReverseIndex]==placedchess[2])){
					ReverseIndex++;
					continueCount++;
				}else{
					ifReverseEnd=true;
				}
			}
		}

		if(continueCount>longPossibleContinue){
			longPossibleContinue=continueCount;
		}

	}



	//slope check
	ifFivePossible=false;
	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if(((placedchess[0]+sequentIndex<tableGapCount)&&(placedchess[1]+sequentIndex<tableGapCount))&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]+sequentIndex]==placedchess[2]||chessSituation[placedchess[0]+sequentIndex][placedchess[1]+sequentIndex]==0)){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[0]-ReverseIndex>=0)&&(placedchess[1]-ReverseIndex>=0))&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2]||chessSituation[placedchess[0]+sequentIndex][placedchess[1]+sequentIndex]==0)){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}


	if(ifFivePossible==true){
		int continueCount=1;
		int ifEnd=false;
		int ifReverseEnd=false;
		int sequentIndex=1;
		int ReverseIndex=1;
		
		while(ifEnd==false||ifReverseEnd==false){
			if(ifEnd==false){
				if(((placedchess[0]+sequentIndex<tableGapCount)&&(placedchess[1]+sequentIndex<tableGapCount))&&(chessSituation[placedchess[0]+sequentIndex][placedchess[1]+sequentIndex]==placedchess[2])){
					sequentIndex++;
					continueCount++;		
				}else{
					ifEnd=true;
				}
			}


			if(ifReverseEnd==false){
				if(((placedchess[0]-ReverseIndex>=0)&&(placedchess[1]-ReverseIndex>=0))&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
					ReverseIndex++;
					continueCount++;
				}else{
					ifReverseEnd=true;
				}
			}
		}

		if(continueCount>longPossibleContinue){
			longPossibleContinue=continueCount;
		}

	}


	ifFivePossible=false;
	continueCount=1;
	ifEnd=false;
	ifReverseEnd=false;
	sequentIndex=1;
	ReverseIndex=1;

	while(ifEnd==false||ifReverseEnd==false){
		if(ifEnd==false){
			if(((placedchess[1]+sequentIndex<tableGapCount)&&(placedchess[0]-sequentIndex>=0))&&(chessSituation[placedchess[0]-sequentIndex][placedchess[1]+sequentIndex]==placedchess[2]||chessSituation[placedchess[0]-sequentIndex][placedchess[1]+sequentIndex]==0)){
				sequentIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
				
			}else{
				ifEnd=true;
			}
		}


		if(ifReverseEnd==false){
			if(((placedchess[1]-ReverseIndex>=0)&&(placedchess[0]+ReverseIndex<tableGapCount))&&(chessSituation[placedchess[0]+ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2]||chessSituation[placedchess[0]-sequentIndex][placedchess[1]+sequentIndex]==0)){
				ReverseIndex++;
				continueCount++;
				if(continueCount>=5){
					ifFivePossible=true;
					break;
				}
			}else{
				ifReverseEnd=true;
			}
		}
	}


	if(ifFivePossible==true){
		int continueCount=1;
		int ifEnd=false;
		int ifReverseEnd=false;
		int sequentIndex=1;
		int ReverseIndex=1;
		
		while(ifEnd==false||ifReverseEnd==false){
			if(ifEnd==false){
				if(((placedchess[1]+sequentIndex<tableGapCount)&&(placedchess[0]-sequentIndex>=0))&&(chessSituation[placedchess[0]-sequentIndex][placedchess[1]+sequentIndex]==placedchess[2])){
					sequentIndex++;
					continueCount++;
				}else{
					ifEnd=true;
				}
			}


			if(ifReverseEnd==false){
				if(((placedchess[1]-ReverseIndex>=0)&&(placedchess[0]+ReverseIndex<tableGapCount))&&(chessSituation[placedchess[0]+ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
					ReverseIndex++;
					continueCount++;
				}else{
					ifReverseEnd=true;
				}
			}
		}

		if(continueCount>longPossibleContinue){
			longPossibleContinue=continueCount;
		}

	}

	return longPossibleContinue;
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
			if((placedchess[0]-ReverseIndex>=0)&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]]==placedchess[2])){
				ReverseIndex++;
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
			if((placedchess[1]-ReverseIndex>=0)&&(chessSituation[placedchess[0]][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
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
			if(((placedchess[0]-ReverseIndex>=0)&&(placedchess[1]-ReverseIndex>=0))&&(chessSituation[placedchess[0]-ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
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
			if(((placedchess[1]-ReverseIndex>=0)&&(placedchess[0]+ReverseIndex<tableGapCount))&&(chessSituation[placedchess[0]+ReverseIndex][placedchess[1]-ReverseIndex]==placedchess[2])){
				ReverseIndex++;
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
void saveEndgame(){
	ofstream outfile;
	outfile.open("endgame.txt");
	outfile << chessingSide <<endl;
	for(int i=0;i<tableGapCount;i++){
		for(int j=0;j<tableGapCount;j++){
			outfile << chessSituation[i][j] << " ";
		}
		outfile<< endl;
	}
	outfile.close();
}

void loadEndgame(){
	ifstream infile;
	infile.open("endgame.txt");
	infile >> chessingSide;
	for(int i=0;i<tableGapCount;i++){
		for(int j=0;j<tableGapCount;j++){
			infile >> chessSituation[i][j];
		}
	}
}
void processData(){
		dealInput();
		regretChess();

		if(winner==0){
			if(ifAi&&chessingSide==aiSide){
				aiPlaceChess();
			}else{
				placeChess();
			}
			if(ifSaveEndgame)
			{
				saveEndgame();
				ifSaveEndgame=false;
			}
			if(ifLoadEndgame)
			{
				loadEndgame();
				ifLoadEndgame=false;
			}
			storeChessSituation();

			if(ifForbid){
				checkForbid();
			}
			if(winner==0){
				checkWin();
			}
		}


}
