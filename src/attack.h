#ifndef ATTACK
#define ATTACK

#include "base.h"
#include "bitboard.h"

static constexpr std::array<Direction, 2> pawn_direction = {
    NORTH,  // WHITE
    SOUTH   // BLACK
};
static constexpr std::array<Rank, 2> en_passant_rank = {
    4,  // WHITE
    3   // BLACK
};
static constexpr std::array<Rank, 2> promotion_rank = {
    6,  // WHITE
    1   // BLACK
};

Bitboard calc_pawn_attack(Square sq, Bitboard blockers, Side p);
Bitboard calc_bishop_attack(Square sq, Bitboard blockers);
Bitboard calc_knight_attack(Square sq);
Bitboard calc_rook_attack(Square sq, Bitboard blockers);
Bitboard calc_king_attack(Square sq);

#endif
