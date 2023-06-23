#include "zobrist.h"


constexpr Hash seed = 43214213132;
constexpr Hash multiplier = 890523987724;
constexpr Hash summand = 435235234524352;

constexpr Hash next_random(Hash previous) {
	return previous*multiplier + summand;
}
constexpr std::array< std::array<Hash, 64>, 12 > init_piece_constants() {
	std::array< std::array<Hash, 64>, 12 > r;
	Hash h = seed;
	for (Square sq = 0; sq < 64; sq++)
		for (Piece p : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING })
			for (Side s : { WHITE, BLACK }) {
				r[p + 6*s][sq] = next_random(h);
				h = r[p + 6*s][sq];
			}
	return r;
}

constexpr Hash player_const = seed;
constexpr std::array< std::array<Hash, 64>, 12 > piece_constants = init_piece_constants();

void Hasher::invert_piece(Square sq, Piece p, Side s) {
	hash ^= piece_constants[p * 6*s][sq];
}
void Hasher::invert_player() {
	hash ^= player_const;
}
