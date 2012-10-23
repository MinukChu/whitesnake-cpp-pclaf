
#pragma once
#include "card.h"

// cardPile class.
// 1. �ֻ��� ī�带 ������ �ʰ� ���⸸ �ϴ� �Լ� topCard()�� �߰��Ѵ�.
// 2. ī�带 ���� �� �ִ��� Ȯ���ϴ� �����Լ� canTake()�� �߰��Ѵ�.(������ 0�� ����.)
// 3. Ư�� ī�带 ������ �� �ִ� �����Լ� select(int)�� �߰��Ѵ�.
// 4. display()�Լ��� ī�尡 �������, �� �׵θ��� �׸��� ����� �߰��Ѵ�.
// 5. �����ϰ� �ִ� ī���� ���� �˷��ִ� getCount()�Լ��� �߰��Ѵ�.
// 6. �����ϰ� �ִ� ��� ī�带 ������ drawAll()�Լ��� �߰��Ѵ�.
// 7. includes()�Լ��� �����Լ��� �����Ѵ�.
class cardPile
{
protected:
	card* theCards[MAX_CARD];
	int count;
	int posX;
	int posY;

public:
	card* topCard();				// �ֻ���ī�带 ������ �ʰ� ���⸸ �ϴ� �Լ�.
	card* drawTopCard();
	virtual int canTake(card*);		// ���� �� �ִ� ī���ΰ�??
	virtual void addCard(card*);
	virtual void select();
	virtual void select(int);		// Ư�� ī�带 �������� �� ������ ����.
	virtual void display(window*);
	virtual int includes(int,int);	// deckPile���� ������.
	void flip(int);
	bool isfront(int);
	int getCount();					// ���� ������ �ִ� ī���� ��.
	void drawAll();					// ���� ������ �ִ� ī�带 ��� ������.

public:
	cardPile(int,int);	
};

// deckPile class
// 1. includes()�Լ��� �������Ѵ�. 
//    : ī�尡 ��� ���õ� �� �ֵ��� �Ѵ�.
// 2. select()�Լ��� �������Ѵ�. 
//    : Ŭ���ϸ� theDiscard�� �ֻ��� ī�带 ����� ����, ī�尡 ������� �ٽ� ��� ī�带 ����´�.
class deckPile : public cardPile
{
public:
	void addCard(card*);
	void select();					// discard��ü�� ī�带 3�徿 �Ѱ��ش�.
	int includes(int,int);			// ī�尡 ��� ������ �Ǿ�� �Ѵ�.
public:
	deckPile(int,int);
};

// discardPile class
// 1. select()�Լ��� �������Ѵ�. 
//    : Ŭ���ϸ� this�� deck�� ������ �ٸ� pile�鿡�� �ֻ��� ī�带 �ѱ��.(���� �� ���� ��쿡 ���ؼ�)
// 2. display()�Լ��� �������Ѵ�.
//    : �ִ� 3���� ���������� ��ġ�� ����Ѵ�.(������ 15)
//    : ��)15���� ���� ���, 13/14/15������ ����ϰ� 15�� ���� ���� �ö󰣴�.
class discardPile : public cardPile
{
public:
	void display(window*);
	void select();

public:
	discardPile(int,int);
};

// suitPile class
// 1. canTake()�Լ��� �������Ѵ�.
//    : 1��ī����� �����ؼ� ���� ����� ī�带 ������� ���� �� �ְ� ����� �ۼ��Ѵ�.
class suitPile : public cardPile 
{
public:
	int canTake(card*);
public:
	suitPile(int,int);
};

// tablePile class
// 1. canTake()�Լ��� �������Ѵ�.
//    : ī�尡 ���嵵 ������ KING�� �޴´�.
//    : ī�尡 ������ �ֻ��� ī�尡 �ո��� ��츸 �޴´�.
//    : �ֻ��� ī�尡 �ո��̸�, �ٸ� ������ rank-1 ī�常 �޴´�.
// 2. display()�Լ��� �������Ѵ�.
//    : ī�带 �Ʒ��������� ��ġ�� ����Ѵ�.(���� 30)
// 3. selelct()�Լ��� �������Ѵ�.
//    : �ֻ��� ī�尡 �޸��̸� �ո����� �����´�.
//    : �ֻ��� ī�尡 �ո��̸� �ڽŰ� deck, discard�� ������ ��� pile�鿡�� �ֻ��� ī�带 �ѱ��.(���� �� ���� ��쿡 ���ؼ�)
//    : �ڽ��� �����ϱ� ���� canTake()�� ��� �� topCard()�� �̿��Ѵ�.
// 4. select(int)�Լ��� �������Ѵ�.
//    : ������ ī�尡 �ո��̸� �׷��̵��� �Ѵ�. (�̵��� ������ ��쿡 ���ؼ�)

class tablePile : public cardPile 
{
public:
	int canTake(card *);
	void display(window *);
	void select();				// �ֻ��� ī�尡 ���õǾ��� �� ����� ����.
	void select(int);			// �߰� ī�尡 ���õǾ��� �� ����� ����.
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