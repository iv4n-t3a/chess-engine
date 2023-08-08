#include "moves.h"

#include "engine.h"

#include <map>
#include <unordered_map>
#include <algorithm>


void sort_moves(std::vector<Move>&, Position const&);
Evaluation evaluate_move(Move, Position const&);
std::pair<Move, Evaluation> search(Position, Depth, AB);

Move search(Position p, Depth d) {
	return search(p, d, AB()).first; 
}

std::pair<Move, Evaluation> search(Position p, Depth d, AB ab) {
	if (p.get_state() == CHECK and d == 0) d++; // check reinwall
	if (d == 0) return {UNINITIALIZED, evaluate(p)};

	std::vector<Move> moves;
	moves.reserve(218);
	p.generate_pseudolegal_moves(moves);
	sort_moves(moves, p);

	bool no_possible_moves = true;
	std::pair<Move, Evaluation> best_found = {UNINITIALIZED, -best_ev[p.get_active()]};

	Position copy;

	for (Move m : moves) {
		if (not p.is_legal(m)) continue;
		copy = p;
		copy.do_move(m);
		no_possible_moves = false;

		Evaluation e = search(copy, d-1, ab).second;
		if ( e >= best_found.second and p.get_active() == WHITE ) {
			best_found = {m, e};
			ab.alpha = e;
		} else if ( e <= best_found.second and p.get_active() == BLACK ) {
			best_found = {m, e};
			ab.beta = e;
		}
		if (ab.alpha > ab.beta)
			return best_found;
	}

	if (no_possible_moves) p.report_lack_of_legal_moves();
	if (p.get_state() == DRAW) return {UNINITIALIZED, 0};
	if (p.get_state() == WIN) return {UNINITIALIZED, best_ev[p.get_active()]};
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
		e = -getbit(pawn_attack, to(m)) * piece_cost[from(m)];
	}
	return e;
}
