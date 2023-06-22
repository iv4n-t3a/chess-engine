#include "moves.h"


constexpr PosDelta normal_delta(Move m) {
	PosDelta res;
	res.add = 1ull << to(m);
	res.rm = 1ull << from(m);
	return res;
};
constexpr PosDelta castle_delta(Move m) {
	PosDelta res = {0, 0};
	switch (castle_type(m)) {
		case WHITE_OO:
			res.add |= (1ull << F1) | (1ull << G1);
			res.rm |= (1ull << E1) | (1ull << H1);
			break;
		case WHITE_OOO:
			res.add |= (1ull << B1) | (1ull << C1) | (1ull << D1);
			res.rm |= (1ull << E1) | (1ull << A1);
			break;
		case BLACK_OO:
			res.add |= (1ull << F8) | (1ull << G8);
			res.rm |= (1ull << E8) | (1ull << H8);
			break;
		case BLACK_OOO:
			res.add |= (1ull << B8) | (1ull << C8) | (1ull << D8);
			res.rm |= (1ull << E8) | (1ull << A8);
			break;
	}
	return res;
};
constexpr PosDelta en_passant_delta(Move m) {
	PosDelta res;
	res.rm = (1ull << square(file_from(m), en_passant_rank[side(m)]));
	res.add = 1ull << (square(file_to(m), en_passant_rank[side(m)]) + pawn_direction[side(m)]);
	return res;
}
constexpr PosDelta promotion_delta(Move m) {
	PosDelta res;
	res.rm = 1ull << square(file_from(m), promotion_rank[side(m)]);
	res.add = 1ull << (square(file_to(m), promotion_rank[side(m)]) + pawn_direction[side(m)]);
	return res;
}
constexpr std::array<PosDelta, std::numeric_limits<Move>::max()> init_pd() {
	std::array<PosDelta, std::numeric_limits<Move>::max()> res;
	for (Move m = 0; m < std::numeric_limits<Move>::max(); m++) {
		switch (move_type(m)) {
			case NORMAL: res[m] = normal_delta(m); break;
			case CASTLE: res[m] = castle_delta(m); break;
			case EN_PASSANT: res[m] = en_passant_delta(m); break;
			case PROMOTION: res[m] = promotion_delta(m); break;
		}
	}
	return res;
}
constexpr std::array<PosDelta, std::numeric_limits<Move>::max()> pd = init_pd();

PosDelta posdelta(Move m) {
	return pd[m];
}
