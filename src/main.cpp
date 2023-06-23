#include "attack.h"
#include "position.h"
#include "config.h"
#include "drawer.h"
#include "ui.h"

void print_help(char* name) {
	std::cout << "Usage: " << name << " <options> {white player} {black player}\n" <<
	"Player types:\n" <<
	"	h - human\n" <<
	"	b - bot\n" <<
	"Options:\n" <<
	"	-d or --depth - engine analise depth\n" <<
	"	-p or --prgram-dir - program files directory\n" <<
	"	-w or --wigth - window wigth\n" <<
	"	-h or --height - window height\n" <<
	"	-? or --help - show this message\n" <<
	"Keys:\n" <<
	"	U - undo move\n" <<
	"	Q - quit\n";
}

int main(int argc, char *argv[]) {
	Config cfg;
	try { 
		cfg = generate_config(argc, argv);
	} catch (ParsingException e) {
		print_help(argv[0]);
		if (e == HELPEXEPT)
			return 0;
		return -1;
	}
	sf::RenderWindow win(sf::VideoMode(cfg.wight, cfg.height), "CHESS");

	Position p;
	Drawer drw(win, p, cfg);
	UI ui(drw, p, cfg);

	for (;;) {
		if(cfg.players[p.get_active()] == HUMAN) ui.player_move();
		else ui.computer_move();

		if (p.get_state() == DRAW) {
			std::cout << "Draw!" << std::endl;
			break;
		}
		if (p.get_state() == WIN) {
			std::cout << (p.get_active() == WHITE ? "White " : "Black ") << "win!" << std::endl;
			break;
		}
	}
	drw.wait_event();
	return 0;
}
