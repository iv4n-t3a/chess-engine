#include "moves.h"

#include "engine.h"


std::pair<Move, Evaluation> search(Position p, Depth soft_limit, Depth hard_limit, Depth left, AB ab) {
	if (p.get_state() == CHECK and left == soft_limit and soft_limit + 1 != hard_limit) soft_limit++; // check reinwall
	if (left == soft_limit) return {UNINITIALIZED, evaluate(p)};

	std::vector<Move> moves;
	moves.reserve(218);
	p.generate_pseudolegal_moves(moves);

	std::pair<Move, Evaluation> best_found = {UNINITIALIZED, -best_ev[p.get_active()]};

	Position copy;
	for (Move m : moves) {
		if (not p.is_legal(m)) continue;
		copy = p;
		copy.do_move(m);

		Evaluation e = search(copy, soft_limit, hard_limit, left+1, ab).second;
		if ( e > best_found.second and p.get_active() == WHITE ) {
			best_found = {m, e};
			ab.alpha = e;
		} else if ( e < best_found.second and p.get_active() == BLACK ) {
			best_found = {m, e};
			ab.beta = e;
		}
		if (ab.alpha > ab.beta)
			return best_found;
	}

	if (best_found.first == UNINITIALIZED) p.report_lack_of_legal_moves();
	if (p.get_state() == DRAW) return {UNINITIALIZED, 0};
	if (p.get_state() == WIN) return {UNINITIALIZED, -best_ev[p.get_active()]};
	return best_found;
}
