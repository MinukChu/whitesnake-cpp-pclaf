
#include "pile.h"

deckPile theDeck(50,50);
discardPile theDiscard(140,50);

suitPile suit1(290,50);
suitPile suit2(370,50);
suitPile suit3(450,50);
suitPile suit4(530,50);

tablePile table1(50,180);
tablePile table2(130,180);
tablePile table3(210,180);
tablePile table4(290,180);
tablePile table5(370,180);
tablePile table6(450,180);
tablePile table7(530,180);

cardPile* allPiles[13];

// cardPile class.

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

card* cardPile::topCard()
{
	if(count > 0) 
		return theCards[count-1];
	return NULL;
}

card* cardPile::drawTopCard()
{
	if(count > 0)
		return theCards[--count];
	return NULL;
}

int cardPile::canTake(card* c)
{
	return 0;
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

void cardPile::select(int n)
{
	// do not anything.
}

void cardPile::display(window* win)
{
	if(count > 0) 
		theCards[count-1]->draw(win);
	else 
	{
		// ī�尡 ��� �׵θ��� �׷��ش�.
		win->setPen(blue);
		win->rectangle(posX,posY,card::WIDTH,card::HEIGHT);
		win->setPen(black);
	}
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

int cardPile::getCount()
{
	return count;
}

void cardPile::drawAll()
{
	for(int i=0; i<MAX_CARD; i++)
		theCards[i] = NULL;
	count = 0;
}

// deckPile class.

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

void deckPile::select()
{
	if(count > 0) 
	{
		for(int i=0; i<3 && count>0; i++)
		{
			//if(count == 0) break;
			card* c = drawTopCard();
			c->flip();
			theDiscard.addCard(c);
		}
	}
	else 
	{
		int cnt = theDiscard.getCount();
		for(int i=0; i<cnt; i++)
		{
			card* c = theDiscard.drawTopCard();
			c->flip();
			// ī�尡 ������ �ʱ⸦ ���Ұ��, 
			// �θ�Ŭ������ addCard()�Լ��� ����ȴ�.
			addCard(c);
		}
	}
}

int deckPile::includes(int x, int y)
{
	// ī�尡 ��� �����Ǿ ���õ� �� �ֵ��� �Ѵ�.
	if(count == 0)
	{
		if(posX < x && x < posX+card::WIDTH)
			if(posY < y && y < posY+card::HEIGHT)
				return 1;
		return 0;
	}
	else
		return cardPile::includes(x, y);
}

// discardPile class.

discardPile::discardPile(int x, int y)
: cardPile(x, y)
{
}

void discardPile::select()
{
	if(count > 0)
	{
		card* c = drawTopCard();

		for(int i=2; i<=12; i++)
		{
			if(allPiles[i]->canTake(c)) 
			{
				allPiles[i]->addCard(c);
				return;
			}
		}

		this->addCard(c);
	}
}

void discardPile::display( window* win )
{
	if(count > 0) 
	{
		int gabX = 15;
		int tx = 0;

		int start = count-3;
		if (start < 0) start = 0;

		for(int i=start; i<count; i++)
		{
			theCards[i]->moveTo(posX+tx, posY);
			theCards[i]->draw(win);
			tx = tx + gabX;
		}
	}
	else
	{
		cardPile::display(win);
	}
}

// suitPile class.

suitPile::suitPile(int x, int y)
: cardPile(x, y)
{
}

int suitPile::canTake(card* c)
{
	if((count==0) && (c->getRank()==card::ACE))
		return 1;

	if(count > 0) 
	{
		int s = topCard()->getSuit();
		int r = topCard()->getRank();
		if((s == c->getSuit()) && ((c->getRank() - r) == 1))
			return 1;
	}

	return 0;
}

// tablePile class.

tablePile::tablePile(int x, int y)
: cardPile(x, y)
{
}

int tablePile::canTake(card* c)
{
	if((count==0) && (c->getRank()==card::KING))
		return 1;

	if(count > 0) 
	{
		// �ֻ��� ī�尡 �޸��̸� ���� �ʴ´�.
		if(topCard()->isfront() == false) return 0;

		int s = topCard()->getSuit();
		int r = topCard()->getRank();
		if((r - c->getRank()) == 1)
			if((s + c->getSuit()) != 5 && s != c->getSuit()) return 1;
	}

	return 0;
}

void tablePile::display(window* win)
{
	if(count > 0) 
	{
		int gabY = 30;
		int ty = 0;

		for(int i=0; i<count; i++)
		{
			theCards[i]->moveTo(posX, posY+ty);
			theCards[i]->draw(win);
			ty = ty + gabY;
		}
	}
	else
	{
		cardPile::display(win);
	}
}

void tablePile::select()
{
	if(count == 0) return;			// ī�尡 ���ٸ� �����Ѵ�.

	// �ֻ��� ī�尡 �޸��̸� �����⸸ �ϰ� �Լ��� �����Ѵ�.
	if(topCard()->isfront() == false) topCard()->flip();
	else
	{
		// ��ũ ���ϰ� ��ī�� ������ �����ϰ� �˻��Ѵ�.
		for(int i=2; i<=12; i++)
		{
			// �ڱ��ڽŰ��� �񱳿��� canTake�� false�� ��ȯ�ϵ��� �ϱ� ���� ī�带 ������ �ʰ� �׳� ���Ѵ�. 
			// �׷���, �̹� ������ �ִ� �����̹Ƿ� �ڱ��ڽ��� ������ false�� ��ȯ�ϰ� �ȴ�.
			card* c = this->topCard();
			if(allPiles[i]->canTake(c))
			{
				allPiles[i]->addCard(this->drawTopCard());
				break;
			}
		}
	}
}

void tablePile::select(int selectedCardNum)
{
	if(count == 0) return;			// ī�尡 ���ٸ� �����Ѵ�.
	int idx = selectedCardNum - 1;	// ���õ� ī���� �迭 �ε����� ���Ѵ�.
	card* selectedCard = theCards[idx];

	if(selectedCard == topCard())
	{
		this->select();
	}

	// ���õ� ī�尡 �ո��� ��쿡�� ī�� �׷��̵��� �����Ѵ�.
	else if(selectedCard->isfront())
	{
		for(int i=6; i<=12; i++)
		{
			if(allPiles[i]->canTake(selectedCard))
			{
				cardPile stack(0, 0);

				while(true)
				{
					card* top = this->drawTopCard();
					stack.addCard(top);
					// ���ÿ� ���� ī�尡 ���콺�� ������ ī����, �׸� �ִ´�.
					if(top == selectedCard) break;
				}

				int stackCount = stack.getCount();
				for (int m=0; m<stackCount; m++)
				{
					allPiles[i]->addCard(stack.drawTopCard());
				}

				break;
			}
		}
	}
}
