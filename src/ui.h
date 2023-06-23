#ifndef UI_H
#define UI_H

#include "config.h"
#include "drawer.h"


class UI {
	Drawer& drw;
	Config cfg;
	Position& pos;
public:
	UI(Drawer& d, Position& p, Config c);
	void player_move();
	void computer_move();
private:
	std::pair<Square, Square> pick_move();
	Square pick_move(Square from);
};

#endif
