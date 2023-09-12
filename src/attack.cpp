#include <iostream>
#include <array>
#include <limits>

#include "attack.h"

#include "base.h"
#include "bitboard.h"
#include "moves.h"


static constexpr size_t PAWN_MASK_SIZE = 4;
static constexpr size_t BISHOP_MASK_SIZE = 9;
static constexpr size_t ROOK_MASK_SIZE = 12;
static constexpr size_t PAWN_BLOCK_COMBS_COUNT = 1 << PAWN_MASK_SIZE;
static constexpr size_t BISHOP_BLOCK_COMBS_COUNT = 1 << BISHOP_MASK_SIZE;
static constexpr size_t ROOK_BLOCK_COMBS_COUNT = 1 << ROOK_MASK_SIZE;

constexpr bool can_add(Square s, Direction d) {
  return s + d < 64 and s + d >= 0 and abs((s + d)%8 - s%8) <= 1;
}
consteval Bitboard init_ray_attack(Square s, Direction d, Bitboard blockers) {
  Bitboard res = 0;
  while (can_add(s, d) and not getbit(blockers, s)) {
    s += d;
    set_1(res, s);
  }
  return res;
};
consteval Bitboard init_ray_mask(Square s, Direction d) {
  if (not can_add(s, d)) return 0;
  Bitboard res = 0;
  while (can_add(s + d, d)) {
    s += d;
    set_1(res, s);
  }
  return res;
}

consteval Bitboard init_pawn_attack(Square s, Bitboard blockers, Side p) {
  Bitboard res = 0;
  if (p == WHITE) {
    if (can_add(s, NORTH) and not getbit(blockers, s + NORTH)) set_1(res, s + NORTH);
    if (can_add(s, NORTH_EAST) and getbit(blockers, s + NORTH_EAST)) set_1(res, s + NORTH_EAST);
    if (can_add(s, NORTH_WEST) and getbit(blockers, s + NORTH_WEST)) set_1(res, s + NORTH_WEST);
    if (s < 16 and not getbit(blockers, s + NORTH) and not getbit(blockers, s + 2*NORTH)) set_1(res, s + 2*NORTH);
  } else {
    if (can_add(s, SOUTH) and not getbit(blockers, s + SOUTH)) set_1(res, s + SOUTH);
    if (can_add(s, SOUTH_EAST) and getbit(blockers, s + SOUTH_EAST)) set_1(res, s + SOUTH_EAST);
    if (can_add(s, SOUTH_WEST) and getbit(blockers, s + SOUTH_WEST)) set_1(res, s + SOUTH_WEST);
    if (s >= 48 and not getbit(blockers, s + SOUTH) and not getbit(blockers, s + 2*SOUTH)) set_1(res, s + 2*SOUTH);
  }
  return res;
}
consteval Bitboard init_bishop_attack(Square s, Bitboard blockers) {
  return
    init_ray_attack(s, NORTH_EAST, blockers) |
    init_ray_attack(s, NORTH_WEST, blockers) |
    init_ray_attack(s, SOUTH_EAST, blockers) |
    init_ray_attack(s, SOUTH_WEST, blockers);
}
consteval Bitboard init_knight_attack(Square s) {
  Bitboard t = 1ull << s;
  Bitboard t1 = (t << 1) & ~FILE_H | (t >> 1) & ~FILE_A;
  Bitboard t2 = (t << 2) & ~(FILE_H | FILE_G) | (t >> 2) & ~(FILE_A | FILE_B);
  return (t1 << 16) | (t1 >> 16) | (t2 << 8) | (t2 >> 8);
}
consteval Bitboard init_rook_attack(Square s, Bitboard blockers) {
  return
    init_ray_attack(s, NORTH, blockers) |
    init_ray_attack(s, SOUTH, blockers) |
    init_ray_attack(s, EAST, blockers) |
    init_ray_attack(s, WEST, blockers);
}
consteval Bitboard init_king_attack(Square s) {
  Bitboard t = 1ull << s;
  Bitboard t1 = (t << 1) & ~FILE_H | (t >> 1) & ~FILE_A | t;
  return (t1 | (t1 << 8) | (t1 >> 8)) & ~t;
}

consteval Bitboard init_pawn_mask(Square s, Side p) {
  Bitboard res = 0;
  if (p == WHITE) {
    if (can_add(s, NORTH)) set_1(res, s + NORTH);
    if (can_add(s, NORTH_EAST)) set_1(res, s + NORTH_EAST);
    if (can_add(s, NORTH_WEST)) set_1(res, s + NORTH_WEST);
    if (s < 16) set_1(res, s + 2*NORTH);
  } else {
    if (can_add(s, SOUTH)) set_1(res, s + SOUTH);
    if (can_add(s, SOUTH_EAST)) set_1(res, s + SOUTH_EAST);
    if (can_add(s, SOUTH_WEST)) set_1(res, s + SOUTH_WEST);
    if (s >= 48) set_1(res, s + 2*SOUTH);
  }
  return res;
}
consteval Bitboard init_bishop_mask(Square s) {
  return
    init_ray_mask(s, NORTH_EAST) |
    init_ray_mask(s, NORTH_WEST) |
    init_ray_mask(s, SOUTH_EAST) |
    init_ray_mask(s, SOUTH_WEST);
}
consteval Bitboard init_rook_mask(Square s) {
  return
    init_ray_mask(s, NORTH) |
    init_ray_mask(s, SOUTH) |
    init_ray_mask(s, EAST) |
    init_ray_mask(s, WEST);
}


consteval std::array<Bitboard, 128> init_pawn_masks() {
  std::array<Bitboard, 128> res;
  for (Square sq = 0; sq < 64; sq++) {
    res[sq] = init_pawn_mask(sq, WHITE);
    res[sq + 64] = init_pawn_mask(sq, BLACK);
  }
  return res;
}
consteval std::array<Bitboard, 64> init_bishop_masks() {
  std::array<Bitboard, 64> res;
  for (Square sq = 0; sq < 64; sq++)
    res[sq] = init_bishop_mask(sq);
  return res;
}
consteval std::array<Bitboard, 64> init_rook_masks() {
  std::array<Bitboard, 64> res;
  for (Square sq = 0; sq < 64; sq++)
    res[sq] = init_rook_mask(sq);
  return res;
}
static constexpr std::array<Bitboard, 128> pawn_masks = init_pawn_masks();
static constexpr std::array<Bitboard, 64> bishop_masks = init_bishop_masks();
static constexpr std::array<Bitboard, 64> rook_masks = init_rook_masks();

consteval std::array< std::array<Bitboard, PAWN_BLOCK_COMBS_COUNT>, 128 > init_pawn_attack() {
  std::array< std::array<Bitboard, PAWN_BLOCK_COMBS_COUNT>, 128 > res;
  Bitboard blockers;
  for (Square sq = 0; sq < 64; sq++) {
    for (Bitboard t = 0; t < PAWN_BLOCK_COMBS_COUNT; t++) {
      blockers = consteval_pdep(t, pawn_masks[sq]);
      res[sq][t] = init_pawn_attack(sq, blockers, WHITE);
      blockers = consteval_pdep(t, pawn_masks[sq + 64]);
      res[sq + 64][t] = init_pawn_attack(sq, blockers, BLACK);
    }
  }
  return res;
}
consteval std::array< std::array<Bitboard, BISHOP_BLOCK_COMBS_COUNT>, 64 > init_bishop_attack() {
  std::array< std::array<Bitboard, BISHOP_BLOCK_COMBS_COUNT>, 64 > res;
  for (Square sq = 0; sq < 64; sq++) {
    for (Bitboard t = 0; t < BISHOP_BLOCK_COMBS_COUNT; t++) {
      Bitboard blockers = consteval_pdep(t, bishop_masks[sq]);
      res[sq][t] = init_bishop_attack(sq, blockers);
    }
  }
  return res;
}
consteval std::array<Bitboard, 64> init_knight_attack() {
  std::array<Bitboard, 64> res;
  for (Square sq = 0; sq < 64; sq++)
    res[sq] = init_knight_attack(sq);
  return res;
}
consteval std::array< std::array<Bitboard, ROOK_BLOCK_COMBS_COUNT>, 64 > init_rook_attack() {
  std::array< std::array<Bitboard, ROOK_BLOCK_COMBS_COUNT>, 64 > res;
  for (Square sq = 0; sq < 64; sq++) {
    for (Bitboard t = 0; t < ROOK_BLOCK_COMBS_COUNT; t++) {
      Bitboard blockers = consteval_pdep(t, rook_masks[sq]);
      res[sq][t] = init_rook_attack(sq, blockers);
    }
  }
  return res;
}
consteval std::array<Bitboard, 64> init_king_attack() {
  std::array<Bitboard, 64> res;
  for (Square sq = 0; sq < 64; sq++)
    res[sq] = init_king_attack(sq);
  return res;
}

static constexpr std::array<Bitboard, 64> knight_attack = init_knight_attack();
static constexpr std::array<Bitboard, 64> king_attack = init_king_attack();
static constexpr std::array< std::array<Bitboard, PAWN_BLOCK_COMBS_COUNT>, 128 > pawn_attack = init_pawn_attack();
static constexpr std::array< std::array<Bitboard, BISHOP_BLOCK_COMBS_COUNT>, 64 > bishop_attack = init_bishop_attack();
static constexpr std::array< std::array<Bitboard, ROOK_BLOCK_COMBS_COUNT>, 64 > rook_attack = init_rook_attack();

Bitboard calc_knight_attack(Square sq) {
  return knight_attack[sq];
}
Bitboard calc_pawn_attack(Square sq, Bitboard blockers, Side p) {
  return pawn_attack[sq + 64*p][pext(blockers, pawn_masks[sq + 64*p])];
}
Bitboard calc_bishop_attack(Square sq, Bitboard blockers) {
  return bishop_attack[sq][pext(blockers, bishop_masks[sq])];
}
Bitboard calc_rook_attack(Square sq, Bitboard blockers) {
  return rook_attack[sq][pext(blockers, rook_masks[sq])];
}
Bitboard calc_king_attack(Square sq) {
  return king_attack[sq];
}
