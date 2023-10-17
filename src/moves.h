#ifndef MOVES
#define MOVES

#include <limits>

#include "attack.h"
#include "base.h"

const Move UNINITIALIZED = 0xFFFF;

PosDelta posdelta(Move m);

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
constexpr Piece new_piece(Move m) {
  return Piece((m >> 9) & 0b111);
}
constexpr CastleType castle_type(Move m) {
  return CastleType(m >> 2);
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

inline Square formal_from(Move m) {  // not optimised
  switch (move_type(m)) {
    case NORMAL:
      return from(m);
    case EN_PASSANT:
      return square(file_from(m), en_passant_rank[side(m)]);
    case PROMOTION:
      return square(file_from(m), promotion_rank[side(m)]);
    case CASTLE:
      switch (castle_type(m)) {
        case WHITE_OO:
          return E1;
        case WHITE_OOO:
          return E1;
        case BLACK_OO:
          return E8;
        case BLACK_OOO:
          return E8;
      }
  };
  return NONE_SQUARE;
}
inline Square formal_to(Move m) {  // not optimised
  switch (move_type(m)) {
    case NORMAL:
      return to(m);
    case EN_PASSANT:
      return square(file_to(m), en_passant_rank[side(m)]) + pawn_direction[side(m)];
    case PROMOTION:
      return square(file_to(m), promotion_rank[side(m)]) + pawn_direction[side(m)];
    case CASTLE:
      switch (castle_type(m)) {
        case WHITE_OO:
          return G1;
        case WHITE_OOO:
          return C1;
        case BLACK_OO:
          return G8;
        case BLACK_OOO:
          return C8;
      }
  };
  return NONE_SQUARE;
}

#endif
