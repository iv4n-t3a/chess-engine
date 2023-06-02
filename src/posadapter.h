#ifndef POSADAPTER
#define POSADAPTER

#include <vector>

#include "position.h"


enum SquareContent {
	WHITE_PAWN,
	WHITE_BISHOP,
	WHITE_KNIGHT,
	WHITE_ROOK,
	WHITE_QUEEN,
	WHITE_KING,
	BLACK_PAWN,
	BLACK_BISHOP,
	BLACK_KNIGHT,
	BLACK_ROOK,
	BLACK_QUEEN,
	BLACK_KING,
	EMPTY,
	SQ_COND_COUNT
};

class PosAdapter {
	Position& pos;
	std::vector<Move> moves;
public:
	PosAdapter(Position& p);
	std::vector<Move> moves_from(Square sq) const;
	SquareContent at(Square sq) const;
	void do_move(Move m);
	void undo_move();

	inline Bitboard get_position() const { return pos.get_position(); }
	inline State get_state() const { return pos.get_state(); }
private:
	void generate_moves();
};

#endif
