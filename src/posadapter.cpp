#include "moves.h"

#include "posadapter.h"


PosAdapter::PosAdapter(Position& p) : pos(p) {
	generate_moves();
}

std::vector<Move> PosAdapter::moves_from(Square sq) const {
	std::vector<Move> res;
	for (Move m : moves)
		if (formal_from(m) == sq)
			res.push_back(m);
	return res;
}
void PosAdapter::do_move(Move m) {
	pos.do_move(m);
	generate_moves();
}
SquareContent PosAdapter::at(Square sq) const {
	if (sq == NONE_SQUARE)
		return EMPTY;

	if ( getbit(pos.get_position(PAWN, WHITE), sq) ) return WHITE_PAWN;
	if ( getbit(pos.get_position(KNIGHT, WHITE), sq) ) return WHITE_KNIGHT;
	if ( getbit(pos.get_position(BISHOP, WHITE), sq) ) return WHITE_BISHOP;
	if ( getbit(pos.get_position(ROOK, WHITE), sq) ) return WHITE_ROOK;
	if ( getbit(pos.get_position(QUEEN, WHITE), sq) ) return WHITE_QUEEN;
	if ( getbit(pos.get_position(KING, WHITE), sq) ) return WHITE_KING;

	if ( getbit(pos.get_position(PAWN, BLACK), sq) ) return BLACK_PAWN;
	if ( getbit(pos.get_position(KNIGHT, BLACK), sq) ) return BLACK_KNIGHT;
	if ( getbit(pos.get_position(BISHOP, BLACK), sq) ) return BLACK_BISHOP;
	if ( getbit(pos.get_position(ROOK, BLACK), sq) ) return BLACK_ROOK;
	if ( getbit(pos.get_position(QUEEN, BLACK), sq) ) return BLACK_QUEEN;
	if ( getbit(pos.get_position(KING, BLACK), sq) ) return BLACK_KING;

	return EMPTY;
}
void PosAdapter::generate_moves() {
	std::vector<Move> pseudolegal;
	pos.generate_pseudolegal_moves(pseudolegal);
	moves.clear();

	for (Move m : pseudolegal)
		if (pos.is_legal(m))
			moves.push_back(m);

	if (moves.empty())
		pos.report_lack_of_legal_moves();
}
