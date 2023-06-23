#include "attack.h"
#include "position.h"
#include "config.h"
#include "drawer.h"
#include "ui.h"

void print_bb(Bitboard bb) {
	for (int i = 0; i < 64; i += 8) {
		for (int j = i; j < i + 8; j++)
			std::cout << getbit(bb, j);
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

int main(int argc, char *argv[]) {
	Config cfg = generate_config(argc, argv);
	sf::RenderWindow win(sf::VideoMode(cfg.wight, cfg.height), "CHESS");

	Position p;
	Drawer drw(win, p, cfg);
	UI ui(drw, p, cfg);
	for (int i = 1; true; i = (i + 1) % 2) {
		/* if(i) ui.player_move(); */
		/* else ui.computer_move(); */

		ui.player_move();
		/* ui.computer_move(); */

		if (p.get_state() == DRAW) std::cout << "Draw!" << std::endl;
		else if (p.get_state() == WIN) std::cout << (p.get_active() == WHITE ? "White " : "Black ") << "win!" << std::endl;
		else continue;

		return 0;
	}
	return 0;
}
