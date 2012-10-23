
//////////////////////////////////////////////////////////////////////////
// 1. cardPile 클래스(모든 카드묶음 클래스의 부모)를 작성한다.
// 2. deckPile 클래스(랜덤하게 섞은 후 엎어놓은 카드묶음)를 작성한다.
// 3. discardPile 클래스(화면에 펼쳐진 카드묶음)를 작성한다.
// 4. deckPile객체와 discardPile객체를 생성 후 화면에 표시한다.
// 5. deckPile객체를 클릭하면 최상위 카드를 한장 꺼내와서 discardPile객체에게 넘긴다.
// 6. 오른쪽 버튼을 이용하여 discardPile에 있는 카드를 펼쳐서 보이는 기능을 추가한다.
// 7. 종료 메뉴를 추가한다.
//////////////////////////////////////////////////////////////////////////

#include "../pclaf.h"
#include <cmath>

#define MAX_CARD 52

// figure class.
class figure
{
protected:
	int x;
	int y;

	color col;
	int thick;

public:
	void moveTo(int,int);
	virtual void draw(window*) = 0;
	virtual int includes(int,int) = 0;

	int getPosX() const {return x;}
	int getPosY() const {return y;}

	void setPen(color c, int t)
	{
		col = c;
		thick = t;
	}

public:
	figure(int,int);
};

figure::figure(int x, int y)
{
	moveTo(x,y);
	col = black;
	thick = 1;
}

void figure::moveTo(int x, int y)
{
	this->x = x;
	this->y = y;
}

// myCircle class.
class myCircle : public figure
{
protected:	
	int radius;

public:
	void draw(window*);
	int includes(int,int);

public:
	myCircle(int,int,int);
};

myCircle::myCircle(int x, int y, int r)
: figure(x,y), radius(r)
{
}

int myCircle::includes(int px, int py)
{
	int distanceSq = 0;
	distanceSq = (px-x)*(px-x) + (py-y)*(py-y);
	if(distanceSq <= radius*radius)
		return 1;
	else
		return 0;
}

void myCircle::draw(window* win)
{
	win->setPen(col, solidLine, thick);
	win->circle(x, y, radius);
}

// myTriangle class.
class myTriangle : public figure
{
protected:
	int length;

public:
	void draw(window*);
	int includes(int,int);

public:
	myTriangle(int,int,int);
};

myTriangle::myTriangle(int x, int y, int l)
: figure(x,y), length(l)
{
}

void myTriangle::draw(window* win)
{
	win->setPen(col, solidLine, thick);
	int dx = int(length*cos(60*3.141592/180));
	int dy = int(length*sin(60*3.141592/180));
	point trianglePoints[] = {{x, y}, {x-dx, y+dy}, {x+dx, y+dy}};
	win->polygon(trianglePoints, 3);		// 다각형을 그리는 함수.
}

int myTriangle::includes(int px, int py)
{
	int dx = int(length*cos(60*3.141592/180));
	int dy = int(length*sin(60*3.141592/180));

	int left = x-dx;
	int right = x+dx;
	int top = y;
	int bottom = y+dy;

	if(px > left && px < right)
	{
		if(py > top && py < bottom)
		{
			double yOfx = (double)dy/dx;

			double xLength;
			if(px < x) xLength = px - left;
			else xLength = right - px;
			double yLength = bottom - py;

			if(yLength/xLength <= yOfx)
				return 1;
		}
	}

	return 0;
}

// myRect class.
class myRect : public figure
{
protected:
	int width;
	int height;

public:
	void draw(window*);
	int  includes(int,int);
	int  area() const;

public:
	myRect& operator = (const myRect&);
	bool operator > (const myRect&);
	bool operator < (const myRect&);

public:
	myRect(int,int,int,int);
};

myRect::myRect(int x, int y, int w, int h)
:figure(x,y), width(w), height(h)
{
}

void myRect::draw(window* win)
{
	win->setPen(col, solidLine, thick);
	win->rectangle(x, y, width, height);
}

int myRect::includes(int px, int py)
{
	if((x<px) && (x+width>px))
		if((y<py) && (y+height>py))
			return 1;

	return 0;
}

int myRect::area() const
{
	return(width*height);
}

myRect& myRect::operator = (const myRect& right)
{
	if(this != &right)
	{
		x = right.x;
		y = right.y;
		width = right.width;
		height = right.height;
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
	int suit;
	int rank;
	int faceUp;

public:
	void draw(window *);
	void flip();
	void flip(int);

public:
	card(int,int,int,int);

public:
	static const int WIDTH = 71;
	static const int HEIGHT = 96;

	static bitmap card_image[53];
	static void load_image(window*);

	enum SUIT {HEART=1, CLUB, SPADE, DIAMOND};
	enum RANK {ACE=1, JACK=11, QUEEN=12, KING=13};
	enum FACE {BACK, FRONT};
};

bitmap card::card_image[53];
void card::load_image(window* win)
{
	for(int i=0; i<53; i++)
	{
		card_image[i] = win->loadBitmap(i+1);
	}
}

card::card(int x, int y, int suit, int rank)
: myRect(x, y, WIDTH, HEIGHT)
{
	this->suit = suit;
	this->rank = rank;
	this->faceUp = BACK;
}

void card::draw(window *win)
{
	int which = faceUp ? ((suit-1)*13 + rank-1) : 52;
	win->drawBitmap(card_image[which], x, y, WIDTH, HEIGHT);
}

void card::flip()
{
	faceUp = !faceUp;
}

void card::flip(int face)
{
	faceUp = face;
}

// carPile class.
class cardPile
{
protected:
	card* theCards[MAX_CARD];
	int count;
	int posX;
	int posY;

public:
	virtual card* drawTopCard();
	virtual void addCard(card*);
	virtual void select();
	virtual void display(window*);
	int includes(int,int);

public:
	cardPile(int,int);	
};

cardPile::cardPile(int x, int y)
: posX(x), posY(y), count(0)
{
	for(int i=0; i<MAX_CARD; i++)
		theCards[i] = NULL;
}

int cardPile::includes(int x, int y)
{
	for(int i=count; i>0; i--)
	{
		if(theCards[i-1]->includes(x,y))
			return 1;
	}
	return 0;
}

card* cardPile::drawTopCard()
{
	if(count > 0)
		return theCards[--count];
	return NULL;
}

void cardPile::addCard(card* newCard)
{
	if(count < MAX_CARD)
	{
		theCards[count] = newCard;
		newCard->moveTo(posX, posY);
		count++;
	}

}

void cardPile::select()
{
	// do not anything.
}

void cardPile::display(window* win)
{
	if(count > 0) theCards[count-1]->draw(win);
}

// deckPile class.
class deckPile : public cardPile
{
public:
	void addCard(card*);
public:
	deckPile(int,int);
};

deckPile::deckPile(int x, int y)
: cardPile(x, y)
{
}

void deckPile::addCard(card* newCard)
{
	if(count < MAX_CARD)
	{
		if(count == 0)
			cardPile::addCard(newCard);
		else
		{
			int n = randomInt(0, count-1);
			theCards[count] = theCards[n];
			theCards[n] = newCard;
			newCard->moveTo(posX, posY);
			count++;
		}
	}
}

// discardPile class.
class discardPile : public cardPile
{
public:
	void display(window*);

public:
	discardPile(int,int);

public:
	bool wideDisplay;
};

discardPile::discardPile(int x, int y)
: cardPile(x, y)
{
	wideDisplay = false;
}

void discardPile::display(window* win)
{
	if(wideDisplay == false)
	{
		cardPile::display(win);
		return;
	}

	if(count > 0) 
	{
		int gabX = card::WIDTH+1;
		int tx = 0;
		for(int i=count; i>0; i--) 
		{
			theCards[i-1]->flip(card::FRONT);
			theCards[i-1]->moveTo(posX+tx, posY);
			theCards[i-1]->draw(win);
			tx = tx + gabX;
		}
	}
}

// 전역변수.
deckPile theDeck(100, 100);
discardPile theDiscard(200, 100);

class myApplication : public application
{
public:
	myApplication(char *);
	myApplication(char *,int,int,int,int);
	~myApplication();
	void paint();
	void mouseDown(int,int);
	void mouseRDown(int,int);
	void mouseMDown(int, int);
	void doubleClick(int,int);

	void init();

	card* allCards[MAX_CARD];
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
			theDeck.addCard(allCards[idx]);
		}
	}
}

myApplication::myApplication(char * apName)
: application(apName)
{
	init();
}

myApplication::myApplication(char * apName, int width, int height, int x, int y)
: application(apName, width, height, x, y)
{
	init();
}

myApplication::~myApplication()
{
	for(int i=0; i<MAX_CARD; i++)
		delete allCards[i];
}

void myApplication::paint()
{
	theDeck.display(this);		// 왼쪽 카드 더미.
	theDiscard.display(this);	// 오른쪽 카드 더미.
}


void myApplication::mouseDown(int x,int y)
{
	if(theDeck.includes(x,y))
	{
		card* deckTop = theDeck.drawTopCard();
		theDiscard.addCard(deckTop);
	}

	if(theDiscard.includes(x,y))
	{
		card* discardTop = theDiscard.drawTopCard();
		discardTop->flip();
		theDiscard.addCard(discardTop);
	}
	clearAndUpdate();
}

void myApplication::mouseRDown(int x,int y)
{
	if(theDiscard.includes(x,y))
		theDiscard.wideDisplay = !theDiscard.wideDisplay;
	// 		if(theDiscard.wideDisplay == false)
	// 			theDiscard.wideDisplay = true;
	// 		else
	// 			theDiscard.wideDisplay = false;

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

int mainLAF(void)
{
	myApplication theApp("CardPile Class", 800, 600, 50, 50);
	menu fileMenu(theApp,"File",'F');
	exitMenuItem exit(theApp, "Exit", 'x', fileMenu);

	card::load_image(&theApp);

	theApp.run();

	return 0;
}