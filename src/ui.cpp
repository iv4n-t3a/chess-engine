#include "config.h"
#include "drawer.h"
#include "moves.h"
#include "engine.h"

#include "ui.h"


UI::UI(Drawer& d, Position& p, Config c) : drw(d), pos(p), cfg(c) {
}

void UI::player_move() {
	Square from, to;
	Event e;
	from = NONE_SQUARE;

	std::vector<Move> pseudolegal_moves, moves;
	pos.generate_pseudolegal_moves(pseudolegal_moves);
	for (Move m : pseudolegal_moves)
		if (pos.is_legal(m)) moves.push_back(m);

	if (moves.empty()) {
		pos.report_lack_of_legal_moves();
		return;
	}

	for (;;) {
		for (Move m : moves)
			if (formal_from(m) == from) drw.border(formal_to(m));
		drw.redraw();

		e = drw.wait_event();
		switch (e.type) {
			case UNDOMOVE:
				if (history.size()) {
					pos = history[history.size() - 1];
					history.pop_back();
				}
				player_move();
				return;
			case PICKSQ:
				to = e.picked;
				break;
			case NONE_EVENT:
				break;
		}

		for (Move m : moves) {
			if (formal_from(m) == from and formal_to(m) == to) {
				history.push_back(pos);
				pos.do_move(m);
				drw.redraw();
				return;
			}
		}
		if (to != NONE_SQUARE) from = to;
	}
}
void UI::computer_move() {
	drw.unborder_all();
	Move m = search(pos, 7);
	pos.do_move(m);
	drw.redraw();
}
