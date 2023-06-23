#ifndef ZOBRIST
#define ZOBRIST

#include "base.h"

class Hasher {
	Hash hash = 0;
public:
	Hasher();
	void invert_piece(Piece p, Side s, Square sq);
	void invert_castle(CastleType t);
	void invert_player();

	inline Hash get() { return hash; }
};

#endif
