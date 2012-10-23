
#include "card.h"

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
