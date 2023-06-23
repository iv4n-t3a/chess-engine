#include "zobrist.h"


constexpr Hash seed = 0xAB0BA;
constexpr Hash multiplier = 0x1984;
constexpr Hash summand = 0xACDC;

constexpr Hash next_random(Hash previous) {
	return previous*multiplier + summand;
}
constexpr std::array< std::array<Hash, 64>, 12 > init_piece_constants(Hash seed) {
	std::array< std::array<Hash, 64>, 12 > r;
	Hash h = seed;
	for (Side s : { WHITE, BLACK })
		for (Piece p : { PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING })
			for (Square sq = 0; sq < 64; sq++) {
				r[p + 6*s][sq] = next_random(h);
				h = r[p + 6*s][sq];
			}
	return r;
}
constexpr std::array<Hash, 4> init_castle_constants(Hash seed) {
	std::array<Hash, 4> r;
	Hash h = seed;
	for ( CastleType t : {WHITE_OO, WHITE_OOO, BLACK_OO, BLACK_OOO} ) {
		r[t] = next_random(h);
		h = r[t];
	}
	return r;
}

constexpr std::array< std::array<Hash, 64>, 12 > piece_constants = init_piece_constants(seed);
constexpr std::array<Hash, 4> castle_constants = init_castle_constants(piece_constants[11][63]);
constexpr Hash player_const = next_random(castle_constants[3]);

void Hasher::invert_piece(Piece p, Side s, Square sq) {
	hash ^= piece_constants[p * 6*s][sq];
}
void Hasher::invert_castle(CastleType t) {
	hash ^= castle_constants[t];
}
void Hasher::invert_player() {
	hash ^= player_const;
}
