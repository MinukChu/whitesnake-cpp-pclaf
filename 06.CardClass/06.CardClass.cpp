
// 실습1. 카드 클래스 작성 : 4개의 카드를 생성 후 마우스로 뒤집기.
// 실습2. 마우스 클릭 지점에 랜덤카드 생성.(최대 20개).

#include "../pclaf.h"

class myRect
{
protected:
	int upperLeftX;
	int upperLeftY;
	int lowerRightX;
	int lowerRightY;

public:
	virtual void draw(window*);
	void moveTo(int,int,int,int);
	int  includes(int,int);
	int  area() const;

	int getPosX() const
	{
		return upperLeftX;
	}
	int getPosY() const
	{
		return upperLeftY;
	}

public:
	myRect& operator = (const myRect&);
	bool operator > (const myRect&);
	bool operator < (const myRect&);

public:
	myRect(int,int,int,int);
	myRect(){}
};

myRect::myRect(int x1, int y1, int x2, int y2)
{
	moveTo(x1,y1,x2,y2);
}

void myRect::draw(window* win)
{
	win->line(upperLeftX, upperLeftY, lowerRightX,upperLeftY);
	win->line(upperLeftX, upperLeftY, upperLeftX, lowerRightY);
	win->line(upperLeftX, lowerRightY,lowerRightX,lowerRightY);
	win->line(lowerRightX,upperLeftY, lowerRightX,lowerRightY);
}

void myRect::moveTo(int x1, int y1, int x2, int y2)
{
	upperLeftX = min(x1, x2);
	upperLeftY = min(y1, y2);
	lowerRightX = max(x1, x2);
	lowerRightY = max(y1, y2);
}

int myRect::includes(int x, int y)
{
	if((upperLeftX<x) && (lowerRightX>x))
		if((upperLeftY<y) && (lowerRightY>y))
			return 1;

	return 0;
}

int myRect::area() const
{
	return((lowerRightX-upperLeftX) * (lowerRightY-upperLeftY));
}

myRect& myRect::operator = (const myRect& right)
{
	if(this != &right)
	{
		upperLeftX = right.upperLeftX;
		upperLeftY = right.upperLeftY;
		lowerRightX = right.lowerRightX;
		lowerRightY = right.lowerRightY;
	}
	return *this;
}

bool myRect::operator > (const myRect& right)
{
	return area() > right.area();
}

bool myRect::operator < (const myRect& right)
{
	return area() < right.area();
}

class card : public myRect
{
	int suit;		// 모양
	int rank;		// 숫자
	int faceUp;		// 앞면

public:
	void draw(window* );
	void flip();

public:
	card(int,int,int,int);

public:
	static const int WIDTH = 75;
	static const int HEIGHT = 100;

	enum SUIT {HEART=1, CLUB, SPADE, DIAMOND};
	enum RANK {ACE=1, JACK=11, QUEEN=12, KING=13};
	enum FACE {BACK, FRONT};
};

card::card(int x, int y, int suit, int rank)
: myRect(x, y, x+WIDTH, y+HEIGHT)
{
	this->suit = suit;
	this->rank = rank;
	//this->faceUp = BACK;
	this->faceUp = FRONT;
}

void card::draw(window* win)
{
	// 테두리 그리기.
	myRect::draw(win);

	if(faceUp == FRONT)
	{
		if(suit==HEART || suit==DIAMOND) win->setTextColor(brightRed);

		// 그리기코드 추가.
		win->wout << setpos(upperLeftX+5, upperLeftY+5);
		if(suit==HEART) win->wout << "♥";
		else if(suit==CLUB) win->wout << "♣";
		else if(suit==SPADE) win->wout << "♠";
		else if(suit==DIAMOND) win->wout << "◆";

		win->wout << setpos(upperLeftX+23, upperLeftY+5);
		if(rank==ACE) win->wout << "A";
		else if(rank==JACK) win->wout << "J";
		else if(rank==QUEEN) win->wout << "Q";
		else if(rank==KING) win->wout << "K";
		else win->wout << rank;

		if(suit==HEART || suit==DIAMOND) win->setTextColor(black);
	}
	else
	{
		win->line(upperLeftX, upperLeftY, upperLeftX+WIDTH, upperLeftY+HEIGHT);
		win->line(upperLeftX+WIDTH, upperLeftY, upperLeftX, upperLeftY+HEIGHT);
	}
}

void card::flip()
{
// 	if(faceUp == FRONT) faceUp = BACK;
// 	else faceUp = FRONT;
	faceUp = !faceUp;
}

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	~myApplication();
	void paint();
	void mouseDown(int,int);

public:
// 	card* ha;
// 	card* cj;
// 	card* sq;
// 	card* dk;
	static const int MAXCOUNT = 20;
	card* cards[MAXCOUNT];
	int idx;
};

myApplication::myApplication(char * apName)
: application(apName), idx(0)
{
// 	ha = new card(100, 100, card::HEART, card::ACE);
// 	cj = new card(200, 100, card::CLUB, card::JACK);
// 	sq = new card(300, 100, card::SPADE, card::QUEEN);
// 	dk = new card(400, 100, card::DIAMOND, card::KING);

	for(int i=0; i<MAXCOUNT; i++) cards[i] = NULL;
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y), idx(0)
{
// 	ha = new card(100, 100, card::HEART, card::ACE);
// 	cj = new card(200, 100, card::CLUB, card::JACK);
// 	sq = new card(300, 100, card::SPADE, card::QUEEN);
// 	dk = new card(400, 100, card::DIAMOND, card::KING);

	for(int i=0; i<MAXCOUNT; i++) cards[i] = NULL;
}

myApplication::~myApplication()
{
// 	delete ha;
// 	delete cj;
// 	delete sq;
// 	delete dk;

	for(int i=0; i<idx; i++)
		delete cards[i];
}

void myApplication::paint()
{
// 	ha->draw(this);
// 	cj->draw(this);
// 	sq->draw(this);
// 	dk->draw(this);

	for(int i=0; i<idx; i++)
		cards[i]->draw(this);
}


void myApplication::mouseDown(int x,int y)
{
// 	if(ha->includes(x, y)) ha->flip();
// 	if(cj->includes(x, y)) cj->flip();
// 	if(sq->includes(x, y)) sq->flip();
// 	if(dk->includes(x, y)) dk->flip();

	if(idx < MAXCOUNT) 
	{
		cards[idx] = new card(x,y,randomInt(card::HEART, card::DIAMOND),
								randomInt(card::ACE, card::KING));
		idx++;
	};

	clearAndUpdate();
}

int mainLAF(void)
{
	myApplication theApp("CardClass", 800, 600, 50, 50);

	theApp.run();

	return 0;
}