#ifndef TYPES
#define TYPES

#include <iostream>
#include <array>


static const int PIECECOUNT = 6;

typedef uint64_t Bitboard;
typedef uint8_t Square;
typedef uint16_t Hash;
typedef uint8_t File;
typedef uint8_t Rank;
typedef uint16_t Move;
typedef uint8_t CastleRights;

enum EnumSquare: Square {
	A1, B1, C1, D1, E1, F1, G1, H1,
	A2, B2, C2, D2, E2, F2, G2, H2,
	A3, B3, C3, D3, E3, F3, G3, H3,
	A4, B4, C4, D4, E4, F4, G4, H4,
	A5, B5, C5, D5, E5, F5, G5, H5,
	A6, B6, C6, D6, E6, F6, G6, H6,
	A7, B7, C7, D7, E7, F7, G7, H7,
	A8, B8, C8, D8, E8, F8, G8, H8,
	NONE_SQUARE
};
enum FileMask : Bitboard {
	FILE_A = 0x8080'8080'8080'8080,
	FILE_B = FILE_A >> 1,
	FILE_C = FILE_A >> 2,
	FILE_D = FILE_A >> 3,
	FILE_E = FILE_A >> 4,
	FILE_F = FILE_A >> 5,
	FILE_G = FILE_A >> 6,
	FILE_H = FILE_A >> 7,
};
enum RankMask : Bitboard {
	RANK_1 = 0xff,
	RANK_2 = RANK_1 << 8*1,
	RANK_3 = RANK_1 << 8*2,
	RANK_4 = RANK_1 << 8*3,
	RANK_5 = RANK_1 << 8*4,
	RANK_6 = RANK_1 << 8*5,
	RANK_7 = RANK_1 << 8*6,
	RANK_8 = RANK_1 << 8*7,
};
const std::array<Bitboard, 8> filemasks = { FILE_A, FILE_B, FILE_C, FILE_D, FILE_E, FILE_F, FILE_G, FILE_H };
const std::array<Bitboard, 8> rankmasks = { RANK_1, RANK_2, RANK_3, RANK_4, RANK_5, RANK_6, RANK_7, RANK_8 };

enum Side {
	WHITE,
	BLACK,
};
enum Piece {
	PAWN,
	BISHOP,
	KNIGHT,
	ROOK,
	QUEEN,
	KING,
};

enum MoveType {
	NORMAL,
	EN_PASSANT,
	CASTLE,
	PROMOTION,
};
enum CastleType {
	WHITE_OO,
	WHITE_OOO,
	BLACK_OO,
	BLACK_OOO,
};
enum State {
	PLAYING,
	CHECK,
	WIN,
	DRAW,
};
enum Direction {
	NORTH = 8,
	SOUTH = -NORTH,
	WEST = 1,
	EAST = -WEST,
	NORTH_EAST = NORTH + EAST,
	NORTH_WEST = NORTH + WEST,
	SOUTH_EAST = SOUTH + EAST,
	SOUTH_WEST = SOUTH + WEST,
};

struct PosDelta {
	Bitboard add;
	Bitboard rm;
};

constexpr Side invert(Side s) {
	return Side(1 - s);
}
constexpr Rank rank(Square s) {
	return s / 8;
}
constexpr File file(Square s) {
	return s % 8;
}
constexpr Square square(File f, Rank r) {
	return f + r*8;
}
constexpr bool can_castle(CastleRights r, CastleType t) {
	return r & (1 << t);
}

#endif // #ifndef TYPES
