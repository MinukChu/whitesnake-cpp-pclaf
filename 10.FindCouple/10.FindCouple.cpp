
//////////////////////////////////////////////////////////////////////////
// 1. 52장의 카드 모두를 화면에 펼쳐보인다.
// 2. 카드를 클릭하여 뒤집을 수 있도록 한다.
// 3. 한번에 최대 2장까지만 선택할 수 있도록 한다.
// 4. 카드의 짝을 찾으면 그 정보를 유지하도록 한다.
// 5. 카드게임의 규칙을 완성한다.
// 6. 타이머를 이용해서 제목 표시줄에 시간의 흐름을 출력한다.
// 7. 카드의 짝을 모두 찾았을 경우, 경과한 시간을 출력한다.
// 8. 메뉴를 이용하여 게임을 리셋하는 기능을 추가한다.
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
	bool isfront();

	int getSuit() { return suit; }
	int getRank() { return rank; }

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

bool card::isfront()
{
	return (faceUp == FRONT);
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
	void flip(int);
	bool isfront(int);

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
			return i;
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

void cardPile::flip(int num)
{
	if(0 < num && num <= count) theCards[num-1]->flip();
}

bool cardPile::isfront(int num)
{
	if(0 < num && num <= count)
		return theCards[num-1]->isfront();
	else 
		return false;
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
			// 랜덤하게 섞는다.
			int n = randomInt(0, count-1);
			theCards[count] = theCards[n];
			theCards[n] = newCard;
			newCard->moveTo(posX, posY);
			count++;
		}
	}
}

//==========================================
// discardPile class.
//==========================================
class discardPile : public cardPile
{
public:
	int findCount;
	int selectedCard[2];
	int findList[MAX_CARD];

public:
	void init();
	void display(window*);
	void addSelectedCard(int);
	void delSelectedCard(int);
	void checkCouple();
	bool isFoundCard(int);
	bool isRemainLastCouple();
	bool isFindAll();

private:
	bool isCouple(int,int);

public:
	discardPile(int,int);
};

discardPile::discardPile(int x, int y)
: cardPile(x, y)
{
	init();
}

void discardPile::init()
{
	findCount = 0;
	selectedCard[0] = selectedCard[1] = 0;

	for(int i=0; i<MAX_CARD; i++)
		findList[i] = 0;
}

void discardPile::display(window* win)
{
	if(count > 0) 
	{
		int gabX = card::WIDTH+5;
		int gabY = card::HEIGHT+5;

		int tx = 0;
		int ty = 0;

		for(int i=0; i<count; i++)
		{
			theCards[i]->moveTo(posX+tx, posY+ty);
			theCards[i]->draw(win);
			tx = tx + gabX;

			if(tx == gabX*13)
			{
				tx = 0;
				ty += gabY;
			}
		}
	}
}

void discardPile::addSelectedCard(int num)
{
	for(int i=0; i<2; i++)
	{
		if(selectedCard[i] == 0)
		{
			selectedCard[i] = num;
			break;
		}
	}
}

void discardPile::delSelectedCard(int num)
{
	for(int i=0; i<2; i++)
	{
		if(selectedCard[i] == num)
		{
			selectedCard[i] = 0;
			break;
		}
	}
}

bool discardPile::isCouple(int c1_of_array, int c2_of_array)
{
	if(theCards[c1_of_array]->getRank() == theCards[c2_of_array]->getRank())
	{
		if(theCards[c1_of_array]->getSuit()+theCards[c2_of_array]->getSuit() == 5)
			return true;
	}
	return false;
}

void discardPile::checkCouple()
{
	if(selectedCard[0] && selectedCard[1])
	{
		int c1 = selectedCard[0]-1;
		int c2 = selectedCard[1]-1;

		if(isCouple(c1, c2))
		{
			findList[c1] = 1;
			findList[c2] = 1;
			findCount += 2;
		}
		else
		{
			theCards[c1]->flip();
			theCards[c2]->flip();
		}

		selectedCard[0] = selectedCard[1] = 0;
	}
}

bool discardPile::isFoundCard(int num)
{
	return (findList[num-1] != 0);
}

bool discardPile::isRemainLastCouple()
{
	return (findCount+2 == MAX_CARD);
}

bool discardPile::isFindAll()
{
	return (findCount == MAX_CARD);
}

// 전역변수.
deckPile theDeck(100, 100);
discardPile theDiscard(10, 10);

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
	bool startGame;
	DWORD startTime;
	int clickCount;
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
			theDeck.addCard(allCards[idx]); // 랜덤하게 섞는 기능이 포함되어 있다.
		}
	}

	// 디스카드 파일에 모두 넘긴다.
	for(int i=0; i<MAX_CARD; i++)
		//theDiscard.addCard(allCards[i]);
		theDiscard.addCard(theDeck.drawTopCard());

	startGame = false;
	startTime = 0;
	clickCount = 0;
}

void myApplication::reset()
{
	// 카드를 섞기위해 데크파일에 카드를 모두 넘긴다.
	for(int i=0; i<MAX_CARD; i++)
	{
		theDeck.addCard(theDiscard.drawTopCard());
	}

	// 뒤집어서 다시 디스카드파일에 넣는다.
	for(int i=0; i<MAX_CARD; i++)
	{
		card* top = theDeck.drawTopCard();
		top->flip(card::BACK);
		theDiscard.addCard(top);
	}

	theDiscard.init();
	startGame = false;
	startTime = 0;
	clickCount = 0;

	this->stopTimer();
	SetWindowText(this->handle(), title);
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

void myApplication::create()
{
	//startTimer(1000);
}

void myApplication::timer()
{
	DWORD elapsedTime = GetTickCount() - startTime;
	elapsedTime /= 1000;

	char str[64];
	sprintf_s(str, _countof(str), "%s (걸린 시간 : %d 초, 클릭 횟수 : %d 회)", title, elapsedTime, clickCount);
	SetWindowText(this->handle(), str);
}

void myApplication::paint()
{
	theDiscard.display(this);
}


void myApplication::mouseDown(int x,int y)
{
	int n = theDiscard.includes(x,y);
	
	if(n)
	{
		// 클릭카운트 증가.
		clickCount++;

		// 2. 선택카드에 2장이 등록되면, 커플인지 체크.
		// 커플이면...찾은카드에 등록 -> 더이상 선택되지 않게 한다.
		// 커플이아니면...다시 뒤집는다.
		theDiscard.checkCouple();

		// 3. 이미 찾은 카드인가?
		if(theDiscard.isFoundCard(n))
			return;

		// 1. 클릭한 카드를 뒤집는다.
		theDiscard.flip(n);

		// 1-1. 클릭에 의해 뒤집힌 카드가 앞면이라면, 선택 카드에 등록한다.
		if(theDiscard.isfront(n))
		{
			// 카드 선택.
			theDiscard.addSelectedCard(n);

			// 게임 시작 시간을 저장한다.
			if(startGame == false)
			{
				startGame = true;
				startTime = GetTickCount();
				startTimer(1000);
				timer();
			}
		}
		// 1-2. 클릭에 의해 뒤집힌 카드가 뒷면이라면, 선택 카드에서 제거한다.
		else
		{
			// 카드 선택에서 제외.
			theDiscard.delSelectedCard(n);
		}

		// 4. 마지막 한쌍은 선택하자마자 커플인지를 체크한다.
		if(theDiscard.isRemainLastCouple())
			theDiscard.checkCouple();
	}

	clearAndUpdate();

	// 모두 찾는데 소요된 시간을 출력한다.(단 1회만 수행되게 작성한다.)
	// 모두 찾지 못했을 경우,
	//   게임을 진행중인 경우에만 모두 다 찾았는지 검사한다.
	// 모두 찾았을 경우,
	//   처음에는 아래조건을 모두 만족하게 된다. 하지만, 다음부터는...
	//   카드를 클릭하면 이미 찾은카드이므로 여기까지 코드가 오지 않는다.
	//   여백을 클릭하면 n이 0이므로 코드가 수행되지 않는다.
	if(startGame && theDiscard.isFindAll())
	{
		startGame = false;
		stopTimer();
		SetWindowText(this->handle(), title);

		DWORD elapsedTime = GetTickCount() - startTime;
		elapsedTime /= 1000;

		char str[64];
		sprintf_s(str, _countof(str), "걸린 시간 : %d 초, 클릭 횟수 : %d 회", elapsedTime, clickCount);
		//okBox(str, "클리어");
		MessageBox(NULL, str, "클리어", MB_OK);
	}
}

void myApplication::mouseRDown(int x,int y)
{
	//reset();
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
	myApplication theApp("FindCouple Game", 1004, 440, 10, 10);
	menu fileMenu(theApp,"File",'F');
	resetMenuItem reset(theApp, "Reset", 'R', fileMenu);
	exitMenuItem exit(theApp, "Exit", 'x', fileMenu);

	card::load_image(&theApp);

	theApp.run();

	return 0;
}