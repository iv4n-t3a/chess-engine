#include "config.h"
#include "drawer.h"
#include "posadapter.h"
#include "moves.h"
#include "engine.h"

#include "ui.h"


UI::UI(Drawer& d, Config c, PosAdapter p) : drw(d), cfg(c), pos(p) {
}

void UI::player_move() {
	Square from, to;
	from = drw.pick_square();
	for (;;) {
		std::vector<Move> moves = pos.moves_from(from);
		for (Move m : moves)
			drw.border(formal_to(m));
		drw.redraw();

		to = drw.pick_square();
		for (Move m : moves) {
			if (formal_to(m) == to) {
				pos.do_move(m);
				return;
			}
		}
		if (to != NONE_SQUARE)
			from = to;
	}
}
void UI::computer_move() {
	drw.unborder_all();
	drw.redraw();
	Move m = search(pos.get_position(), 8, 20).first;
	pos.do_move(m);
}
