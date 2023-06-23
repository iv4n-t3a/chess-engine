#ifndef ZOBRIST
#define ZOBRIST

#include "base.h"

class Hasher {
	Hash hash = 0;
public:
	void invert_piece(Square sq, Piece p, Side s);
	void invert_player();

	inline Hash get() const { return hash; }
};

#endif
