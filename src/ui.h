#ifndef UI_H
#define UI_H

#include "config.h"
#include "drawer.h"
#include "posadapter.h"


class UI {
	Drawer& drw;
	Config cfg;
	PosAdapter pos;
public:
	UI(Drawer& d, Config c, PosAdapter p);

	void player_move();
	void computer_move();

private:
	std::pair<Square, Square> pick_move();
	Square pick_move(Square from);
};

#endif
