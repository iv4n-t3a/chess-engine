#include "attack.h"
#include "position.h"
#include "config.h"
#include "drawer.h"
#include "ui.h"

#include "unittests.h"

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
	PosAdapter pa(p);

	Drawer drw(win, pa, cfg);
	UI ui(drw, cfg, pa);
	for (;;) {
		ui.player_move();
		ui.computer_move();
	}
	return 0;
}
