#ifndef BITBOARD
#define BITBOARD

#include <iostream>
#include <immintrin.h>

#include "base.h"


constexpr void set_1(Bitboard& bb, Square s) {
	bb |= 1ull << s;
}
constexpr void set_0(Bitboard& bb, Square s) {
	bb &= ~(1ull << s);
}
constexpr bool getbit(Bitboard bb, Square s) {
	return bb & (1ull << s);
}
constexpr void mv(Bitboard& bb, Square from, Square to) {
	set_0(bb, to);
	bb |= (Bitboard)getbit(bb, from) << to;
	set_0(bb, from);
}


constexpr uint8_t popcount(Bitboard bb) {
	return __builtin_popcountll(bb);
}
inline Bitboard pext(Bitboard blockers, Bitboard mask) {
	return _pext_u64(blockers, mask);
}
inline Bitboard pdep(Bitboard p, Bitboard mask) {
	return _pdep_u64(p, mask);
}
constexpr Bitboard consteval_pdep(Bitboard p, Bitboard mask) {
	Bitboard dest = 0;
	int k = 0;
	for (int m = 0; m < 64; m++) {
		if (not getbit(mask, m)) continue;
		if (getbit(p, k)) set_1(dest, m);
		k++;
	}
	return dest;
}

constexpr Square bitscantable[64] = {
	0,  47, 1,  56, 48, 27, 2,  60,
	57, 49, 41, 37, 28, 16, 3,  61,
	54, 58, 35, 52, 50, 42, 21, 44,
	38, 32, 29, 23, 17, 11, 4,  62,
	46, 55, 26, 59, 40, 36, 15, 53,
	34, 51, 20, 43, 31, 22, 10, 45,
	25, 39, 14, 33, 19, 30, 9,  24,
	13, 18, 8,  12, 7,  6,  5,  63
};
constexpr Square bsf(Bitboard bb) { // bit scan forward, return first pop number
	return bitscantable[((bb ^ (bb - 1)) * 0x03f79d71b4cb0a89) >> 58];
}
constexpr Square bsr(Bitboard bb) { // bit scan reversed, returns last pop number
	bb |= bb >> 1;
	bb |= bb >> 2;
	bb |= bb >> 4;
	bb |= bb >> 8;
	bb |= bb >> 16;
	bb |= bb >> 32;

	return bitscantable[(bb * 0x03f79d71b4cb0a89) >> 58];
}

class Bb_iterator {
protected:
	Bitboard bb;
	Square processing;
public:
	Bb_iterator(Bitboard);
	Square operator*() const;
	void operator++();
	bool not_ended() const;
};

inline Bb_iterator::Bb_iterator(Bitboard b) {
	bb = b;
}
inline void Bb_iterator::operator++() {
	set_0(bb, bsf(bb));
}
inline Square Bb_iterator::operator*() const {
	return bsf(bb);
}
inline bool Bb_iterator::not_ended() const {
	return bb != 0;
}

#endif // ifndef BITBOARD
