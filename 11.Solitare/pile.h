
#pragma once
#include "card.h"

// cardPile class.
// 1. 최상위 카드를 꺼내지 않고 보기만 하는 함수 topCard()를 추가한다.
// 2. 카드를 받을 수 있는지 확인하는 가상함수 canTake()를 추가한다.(무조건 0을 리턴.)
// 3. 특정 카드를 선택할 수 있는 가상함수 select(int)를 추가한다.
// 4. display()함수에 카드가 없을경우, 빈 테두리를 그리는 기능을 추가한다.
// 5. 소지하고 있는 카드의 수를 알려주는 getCount()함수를 추가한다.
// 6. 소지하고 있는 모든 카드를 버리는 drawAll()함수를 추가한다.
// 7. includes()함수를 가상함수로 선언한다.
class cardPile
{
protected:
	card* theCards[MAX_CARD];
	int count;
	int posX;
	int posY;

public:
	card* topCard();				// 최상위카드를 꺼내지 않고 보기만 하는 함수.
	card* drawTopCard();
	virtual int canTake(card*);		// 받을 수 있는 카드인가??
	virtual void addCard(card*);
	virtual void select();
	virtual void select(int);		// 특정 카드를 선택했을 때 수행할 동작.
	virtual void display(window*);
	virtual int includes(int,int);	// deckPile에서 재정의.
	void flip(int);
	bool isfront(int);
	int getCount();					// 현재 가지고 있는 카드의 수.
	void drawAll();					// 현재 가지고 있는 카드를 모두 버린다.

public:
	cardPile(int,int);	
};

// deckPile class
// 1. includes()함수를 재정의한다. 
//    : 카드가 없어도 선택될 수 있도록 한다.
// 2. select()함수를 재정의한다. 
//    : 클릭하면 theDiscard로 최상위 카드를 뒤집어서 전달, 카드가 없을경우 다시 모든 카드를 뺏어온다.
class deckPile : public cardPile
{
public:
	void addCard(card*);
	void select();					// discard객체에 카드를 3장씩 넘겨준다.
	int includes(int,int);			// 카드가 없어도 선택이 되어야 한다.
public:
	deckPile(int,int);
};

// discardPile class
// 1. select()함수를 재정의한다. 
//    : 클릭하면 this와 deck를 제외한 다른 pile들에게 최상위 카드를 넘긴다.(받을 수 있을 경우에 한해서)
// 2. display()함수를 재정의한다.
//    : 최대 3장을 오른쪽으로 겹치게 출력한다.(간격은 15)
//    : 예)15장이 있을 경우, 13/14/15순으로 출력하고 15가 가장 위에 올라간다.
class discardPile : public cardPile
{
public:
	void display(window*);
	void select();

public:
	discardPile(int,int);
};

// suitPile class
// 1. canTake()함수를 재정의한다.
//    : 1번카드부터 시작해서 같은 모양의 카드를 순서대로 받을 수 있게 기능을 작성한다.
class suitPile : public cardPile 
{
public:
	int canTake(card*);
public:
	suitPile(int,int);
};

// tablePile class
// 1. canTake()함수를 재정의한다.
//    : 카드가 한장도 없으면 KING만 받는다.
//    : 카드가 있으면 최상위 카드가 앞면일 경우만 받는다.
//    : 최상위 카드가 앞면이면, 다른 색상의 rank-1 카드만 받는다.
// 2. display()함수를 재정의한다.
//    : 카드를 아래방향으로 겹치게 출력한다.(간격 30)
// 3. selelct()함수를 재정의한다.
//    : 최상위 카드가 뒷면이면 앞면으로 뒤집는다.
//    : 최상위 카드가 앞면이면 자신과 deck, discard를 제외한 모든 pile들에게 최상위 카드를 넘긴다.(받을 수 있을 경우에 한해서)
//    : 자신을 제외하기 위해 canTake()로 물어볼 때 topCard()를 이용한다.
// 4. select(int)함수를 재정의한다.
//    : 선택한 카드가 앞면이면 그룹이동을 한다. (이동이 가능한 경우에 한해서)

class tablePile : public cardPile 
{
public:
	int canTake(card *);
	void display(window *);
	void select();				// 최상위 카드가 선택되었을 때 수행될 동작.
	void select(int);			// 중간 카드가 선택되었을 때 수행될 동작.
public:
	tablePile(int,int);
};

extern deckPile theDeck;
extern discardPile theDiscard;

extern suitPile suit1;
extern suitPile suit2;
extern suitPile suit3;
extern suitPile suit4;

extern tablePile table1;
extern tablePile table2;
extern tablePile table3;
extern tablePile table4;
extern tablePile table5;
extern tablePile table6;
extern tablePile table7;

extern cardPile* allPiles[];