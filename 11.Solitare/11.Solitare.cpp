
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////

#include "../pclaf.h"
//#include "figure.h"
//#include "card.h"
#include "pile.h"

// 전역변수.
char* title = "Solitare Game";

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	~myApplication();
	void create();
	void paint();
	void mouseDown(int,int);
	void mouseRDown(int,int);
	void mouseMDown(int, int);
	void doubleClick(int,int);

	void timer();

	void init();
	void reset();

	card* allCards[MAX_CARD];
	int aniCount;
};

void myApplication::init()
{
	// 카드 생성 후 데크 파일에 삽입
	for(int i=1; i<=4; i++)
	{
		for(int j=1; j<=13; j++)
		{
			int idx = (i-1)*13 + (j-1);
			allCards[idx] = new card(0,0,i,j);
			//theDeck.addCard(allCards[idx]);
		}
	}

	// 파일 객체 포인터를 allPiles 배열에 저장
	allPiles[0] = &theDeck;
	allPiles[1] = &theDiscard;
	allPiles[2] = &suit1;
	allPiles[3] = &suit2;
	allPiles[4] = &suit3;
	allPiles[5] = &suit4;
	allPiles[6] = &table1;
	allPiles[7] = &table2;
	allPiles[8] = &table3;
	allPiles[9] = &table4;
	allPiles[10] = &table5;
	allPiles[11] = &table6;
	allPiles[12] = &table7;

	reset();
}

void myApplication::reset()
{
	stopTimer();
	aniCount = 0;

	// 모든 파일 초기화.
	for(int i=0; i<13; i++)
	{
		allPiles[i]->drawAll();
	}

#define Normal

#ifdef Normal

	// 데크파일 초기화.
 	for(int i=0; i<MAX_CARD; i++)
 	{
 		allCards[i]->flip(card::BACK);
 		theDeck.addCard(allCards[i]);
 	}
 
 	// 테이블 파일의 초기화
 	for(int m=1; m<=7; m++)
 	{
 		for(int n=1; n<=m; n++)
 		{
 			allPiles[m+5]->addCard(theDeck.drawTopCard());
 		}
 		// 테이블 파일의 제일 위 카드를 앞면인 상태로
 		allPiles[m+5]->select();
 	}

#else

	// 게임종료 테스트.
 	for(int i=0; i<13; i++)
 	{
 		suit1.addCard(allCards[i]);
 		suit2.addCard(allCards[i+13]);
 		suit3.addCard(allCards[i+26]);
 		suit4.addCard(allCards[i+39]);
 	}
 
 	theDeck.addCard(suit4.drawTopCard());

#endif
}

myApplication::myApplication(char* apName)
: application(apName)
{
	init();
}

myApplication::myApplication(char* apName, int width, int height, int x, int y)
: application(apName, width, height, x, y)
{
	init();
}

myApplication::~myApplication()
{
	for(int i=0; i<MAX_CARD; i++)
		delete allCards[i];
}

void myApplication::create()
{
	//startTimer(1000);
}

void myApplication::timer()
{
	static int dx=10, dy=10;

	card* c = suit4.topCard();
	c->draw(this);
	c->moveTo(c->getPosX()+dx, c->getPosY()+dy);
	if((c->getPosX()<0) || (c->getPosX()+card::WIDTH>width())) dx = -dx;
	if((c->getPosY()<0) || (c->getPosY()+card::HEIGHT>height())) dy = -dy;

	aniCount += 1;
	if(aniCount == 800) 
		stopTimer();
}

void myApplication::paint()
{
	for(int i=0; i<=12; i++)
		allPiles[i]->display(this);
}


void myApplication::mouseDown(int x,int y)
{
	for (int i=0; i<=12; i++)
	{
		int selectedCardNum = allPiles[i]->includes(x,y);
		if (selectedCardNum)
		{
			if (i < 6) allPiles[i]->select();
			else allPiles[i]->select(selectedCardNum);
			break;
		}
	}

	if((suit1.getCount() == 13) && (suit2.getCount() == 13) && 
		(suit3.getCount() == 13) && (suit4.getCount() == 13))
	{
		startTimer(10);
	}

	clearAndUpdate();
}

void myApplication::mouseRDown(int x,int y)
{
	clearAndUpdate();
}

void myApplication::mouseMDown(int x, int y)
{
	clearAndUpdate();
}

void myApplication::doubleClick( int x,int y)
{
	clearAndUpdate();
}

// menu.
class exitMenuItem : public basicMenuItem 
{
public:
	exitMenuItem(window&,char*,char,menu&);
	void select();
};

exitMenuItem::exitMenuItem(window& win, char* txt, char c, menu& owner)
: basicMenuItem(win, txt, c, owner)
{
	// win	 : 메뉴항목이 표시될 윈도우 객체
	// txt	 : 메뉴항목의 문자열
	// c	 : 문자열중에 alt키와 함께 사용될 단축키
	// owner : 주 메뉴 객체.
	/* no action */ 
}

void exitMenuItem::select()
{
	this->theWindow.quit();
}

class resetMenuItem : public basicMenuItem 
{
public:
	resetMenuItem(window&,char*,char,menu&);
	void select();
};

resetMenuItem::resetMenuItem(window& win, char* txt, char c, menu& owner)
: basicMenuItem(win, txt, c, owner)
{
}

void resetMenuItem::select()
{
	((myApplication*)&theWindow)->reset();
	this->theWindow.clearAndUpdate();
}

int mainLAF(void)
{
	myApplication theApp(title, 800, 620, 10, 10);
	menu fileMenu(theApp,"File",'F');
	resetMenuItem reset(theApp, "Reset", 'R', fileMenu);
	exitMenuItem exit(theApp, "Exit", 'x', fileMenu);

	card::load_image(&theApp);

	theApp.run();

	return 0;
}