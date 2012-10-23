
#pragma once
#include "figure.h"

#define MAX_CARD 52

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
