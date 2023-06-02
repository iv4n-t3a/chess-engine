#ifndef MOVES
#define MOVES

#include "base.h"


constexpr MoveType move_type(Move m) {
	return MoveType(m & 0b11);
}
constexpr Side side(Move m) {
	return Side((m >> 15) & 1);
}
constexpr Square from(Move m) {
	return (m >> 2) & 0b111'111;
}
constexpr Square to(Move m) {
	return (m >> 8) & 0b111'111;
}
constexpr File file_from(Move m) {
	return (m >> 2) & 0b111;
}
constexpr File file_to(Move m) {
	return (m >> 5) & 0b111;
}
constexpr Square en_passant_captured(Move m) {
	return square(file_from(m), side(m) == WHITE ? 5 : 4);
}
constexpr Square from_by_file(Move m) {
	return square(file_from(m), side(m) == WHITE ? 5 : 4);
}
constexpr Square to_by_file(Move m) {
	return square(file_to(m), side(m) == WHITE ? 5 : 4);
}
constexpr Piece new_piece(Move m) {
	return Piece((m >> 9) & 0b111);
}
constexpr CastleType castle_type(Move m) {
	return CastleType(m >> 3);
}
constexpr Move build_move(Square from, Square to) {
	return NORMAL | (from << 2) | (to << 8);
}
constexpr Move build_promotion(File from, File to, Piece n, Side s) {
	return PROMOTION | (from << 2) | (to << 5) | (n << 9) | (s << 15);
}
constexpr Move build_en_passant(File from, File to, Side s) {
	return EN_PASSANT | (from << 2) | (to << 5) | (s << 15);
}
constexpr Move build_castle(CastleType c) {
	return CASTLE | (c << 2);
}


inline Square formal_from(Move m) { // not optimised
	switch (move_type(m)) {
		case NORMAL: return from(m);
		case EN_PASSANT: return square(from_by_file(m), side(m) == WHITE ? 4 : 3);
		case PROMOTION: return square(from_by_file(m), side(m) == WHITE ? 6 : 1);
		case CASTLE:
			switch (castle_type(m)) {
				case WHITE_OO: return E1;
				case WHITE_OOO: return E1;
				case BLACK_OO: return E8;
				case BLACK_OOO: return E8;
			}
	};
	return NONE_SQUARE;
}
inline Square formal_to(Move m) { // not optimised
	switch (move_type(m)) {
		case NORMAL: return to(m);
		case EN_PASSANT: return square(from_by_file(m), side(m) == WHITE ? 5 : 2);
		case PROMOTION: return square(from_by_file(m), side(m) == WHITE ? 7 : 0);
		case CASTLE:
			switch (castle_type(m)) {
				case WHITE_OO: return H1;
				case WHITE_OOO: return A1;
				case BLACK_OO: return H8;
				case BLACK_OOO: return A8;
			}
	};
	return NONE_SQUARE;
}

#endif
