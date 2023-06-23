#include "moves.h"

#include "engine.h"


void sort_moves(std::vector<Move>&, Position const&);
Evaluation evaluate_move(Move, Position const&);

std::pair<Move, Evaluation> search(Position p, Depth soft_limit, Depth hard_limit, Depth left, AB ab, bool only_capture) {
	if ((p.get_state() == CHECK or only_capture) and left == soft_limit and soft_limit + 1 != hard_limit) soft_limit++; // check and capture reinwall
	if (left == soft_limit) return {UNINITIALIZED, evaluate(p)};

	std::vector<Move> moves;
	moves.reserve(218);
	p.generate_pseudolegal_moves(moves);
	sort_moves(moves, p);

	std::pair<Move, Evaluation> best_found = {UNINITIALIZED, -best_ev[p.get_active()]};

	Position copy;
	for (Move m : moves) {
		if (not p.is_legal(m)) continue;
		copy = p;
		copy.do_move(m);

		if (only_capture and popcount(copy.get_position()) == popcount(p.get_position())) continue;

		Evaluation e = search(copy, soft_limit, hard_limit, left+1, ab, popcount(copy.get_position()) != popcount(p.get_position()) and left == soft_limit or only_capture).second;
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

void sort_moves(std::vector<Move>& m, Position const& p) {
	std::sort(m.begin(), m.end(), [&p](Move a, Move b){ return evaluate_move(a, p) > evaluate_move(b, p); });
}
Evaluation evaluate_move(Move m, Position const& p) {
	Evaluation e;
	if (move_type(m) == PROMOTION)
		e = 2500 + piece_cost[p.piece_at(square(file_to(m), promotion_rank[p.get_active()]) + pawn_direction[p.get_active()])];
	else if (move_type(m) == NORMAL and getbit(p.get_position(), to(m)))
		e = 2000 + 10*piece_cost[p.piece_at(to(m))] - piece_cost[p.piece_at(from(m))];
	else if (move_type(m) == EN_PASSANT)
		e = 1500;
	else if (move_type(m) == CASTLE)
		e = 1000;
	else if (p.piece_at(to(m)) == PAWN)
		e = 500;
	else {
		Side opponent = invert(p.get_active());
		Bitboard pawn_position = p.get_position(PAWN, opponent);
		Bitboard pawn_attack = 0;
		pawn_attack |= (pawn_position << (pawn_direction[opponent] + WEST)) & ~FILE_H;
		pawn_attack |= (pawn_position << (pawn_direction[opponent] + EAST)) & ~FILE_A;
		e = not getbit(pawn_attack, to(m));
	}
	return e;
}
