#include "attack.h"

#include "engine.h"

const Evaluation material_k = 1000;
const Evaluation mobility_k = 10;
const Evaluation missed_castle_k = 100;
const Evaluation pawn_shield_k = 100;
const Evaluation double_bishop_k = 50;
const Evaluation bishop_on_main_diagonal_k = 10;
const Evaluation double_pawn_k = 40;
const Evaluation connected_pawn_k = 30;
const Evaluation connected_rooks_k = 80;

const Evaluation knight_mobility_k = 40;
const Evaluation bishop_mobility_k = 12;
const Evaluation rook_mobility_k = 4;
const Evaluation queen_mobility_k = 2;

Evaluation material(Position const&);
Evaluation mobility(Position const&);
Evaluation missed_castle(Position const&);
Evaluation pawn_shield(Position const&);
Evaluation double_bishop(Position const&);
Evaluation bishop_on_main_diagonal(Position const&);
Evaluation double_pawn(Position const&);
Evaluation connected_pawn(Position const&);


Evaluation evaluate(Position const& p) {
  return
    material_k * material(p) +
    mobility_k * mobility(p) +
    missed_castle_k * missed_castle(p) +
    pawn_shield_k * pawn_shield(p) +
    double_bishop_k * double_bishop(p) +
    bishop_on_main_diagonal_k * bishop_on_main_diagonal(p) +
    double_pawn_k * double_pawn(p) +
    connected_pawn_k * connected_pawn(p);
}


Evaluation material(Position const& p) {
  return
    ( popcount(p.get_position(PAWN, WHITE)) - popcount(p.get_position(PAWN, BLACK)) ) * 1 +
    ( popcount(p.get_position(KNIGHT, WHITE)) - popcount(p.get_position(KNIGHT, BLACK)) ) * 3 +
    ( popcount(p.get_position(BISHOP, WHITE)) - popcount(p.get_position(BISHOP, BLACK)) ) * 3 +
    ( popcount(p.get_position(ROOK, WHITE)) - popcount(p.get_position(ROOK, BLACK)) ) * 5 +
    ( popcount(p.get_position(QUEEN, WHITE)) - popcount(p.get_position(QUEEN, BLACK)) ) * 8;
}

Evaluation mobility(Position const& p) {
  int knight_moves = 0;
  int bishop_moves = 0;
  int rook_moves = 0;
  int queen_moves = 0;

  for (Bb_iterator i(p.get_position(KNIGHT, WHITE)); i.not_ended(); ++i) knight_moves += popcount( calc_knight_attack(*i) );
  for (Bb_iterator i(p.get_position(BISHOP, WHITE)); i.not_ended(); ++i) bishop_moves += popcount( calc_bishop_attack(*i, p.get_position()) );
  for (Bb_iterator i(p.get_position(ROOK, WHITE)); i.not_ended(); ++i) rook_moves += popcount( calc_rook_attack(*i, p.get_position()) );
  for (Bb_iterator i(p.get_position(QUEEN, WHITE)); i.not_ended(); ++i)
    queen_moves += popcount( calc_bishop_attack(*i, p.get_position()) ) + popcount( calc_rook_attack(*i, p.get_position()) );

  for (Bb_iterator i(p.get_position(KNIGHT, BLACK)); i.not_ended(); ++i) knight_moves -= popcount( calc_knight_attack(*i) );
  for (Bb_iterator i(p.get_position(BISHOP, BLACK)); i.not_ended(); ++i) bishop_moves -= popcount( calc_bishop_attack(*i, p.get_position()) );
  for (Bb_iterator i(p.get_position(ROOK, BLACK)); i.not_ended(); ++i) rook_moves -= popcount( calc_rook_attack(*i, p.get_position()) );
  for (Bb_iterator i(p.get_position(QUEEN, BLACK)); i.not_ended(); ++i)
    queen_moves -= popcount( calc_bishop_attack(*i, p.get_position()) ) + popcount( calc_rook_attack(*i, p.get_position()) );

  return
    knight_moves * knight_mobility_k +
    bishop_moves * bishop_mobility_k +
    rook_moves * rook_mobility_k +
    queen_moves * queen_mobility_k;
}

Evaluation missed_castle(Position const& p) {
  return
    can_castle(p.get_castle_rights(), WHITE_OO) +
    can_castle(p.get_castle_rights(), WHITE_OOO) -
    can_castle(p.get_castle_rights(), BLACK_OO) -
    can_castle(p.get_castle_rights(), BLACK_OOO);
}

const Bitboard white_OO_pawn_shield  = 0x0000'0000'0000'e000;
const Bitboard white_OOO_pawn_shield = 0x0000'0000'0000'0700;
const Bitboard black_OO_pawn_shield  = 0x00e0'0000'0000'0000;
const Bitboard black_OOO_pawn_shield = 0x00e7'0000'0000'0000;
Evaluation pawn_shield(Position const& p) {
  Evaluation r = 0;
  if (can_castle(p.get_castle_happened(), WHITE_OO))  r += popcount(p.get_position(PAWN, WHITE) & white_OO_pawn_shield);
  if (can_castle(p.get_castle_happened(), WHITE_OOO)) r += popcount(p.get_position(PAWN, WHITE) & white_OOO_pawn_shield);
  if (can_castle(p.get_castle_happened(), BLACK_OO))  r -= popcount(p.get_position(PAWN, BLACK) & black_OO_pawn_shield);
  if (can_castle(p.get_castle_happened(), BLACK_OOO)) r -= popcount(p.get_position(PAWN, BLACK) & black_OOO_pawn_shield);
  return r;
}

Evaluation double_bishop(Position const& p) {
  return (popcount(p.get_position(BISHOP, WHITE)) == 2) - (popcount(p.get_position(BISHOP, BLACK)) == 2);
}

const Bitboard main_diagonal = 0x8142'2418'1824'4281;
Evaluation bishop_on_main_diagonal(Position const& p) {
  return
    popcount( (p.get_position(BISHOP, WHITE) | p.get_position(QUEEN, WHITE)) & main_diagonal ) -
    popcount( (p.get_position(BISHOP, BLACK) | p.get_position(QUEEN, BLACK)) & main_diagonal );
}

Evaluation double_pawn(Position const& p) {
  return
    popcount( (p.get_position(PAWN, BLACK) << NORTH) & p.get_position(PAWN, BLACK) ) -
    popcount( (p.get_position(PAWN, WHITE) << NORTH) & p.get_position(PAWN, WHITE) );
}

Evaluation connected_pawn(Position const& p) {
  return
    popcount( ( ((p.get_position(PAWN, WHITE) << NORTH_WEST) & ~FILE_H) | ((p.get_position(PAWN, WHITE) << NORTH_EAST) & ~FILE_A) ) & p.get_position(PAWN, WHITE) ) -
    popcount( ( ((p.get_position(PAWN, BLACK) << NORTH_WEST) & ~FILE_H) | ((p.get_position(PAWN, BLACK) << NORTH_EAST) & ~FILE_A) ) & p.get_position(PAWN, BLACK) );
}
