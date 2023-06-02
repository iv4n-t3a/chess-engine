#include <vector>
#include <array>

#include "attack.h"
#include "base.h"
#include "bitboard.h"
#include "moves.h"

#include "position.h"


Position::Position() {
	by_type = {
		0x00ff'0000'0000'ff00, // PAWN
		0x2400'0000'0000'0024, // BISHOP
		0x4200'0000'0000'0042, // KNIGHT
		0x8100'0000'0000'0081, // ROOK
		0x1000'0000'0000'0010, // QUEEN
		0x0800'0000'0000'0008, // KING
	};
	by_side = { 0x0000'0000'0000'ffff, 0xffff'0000'0000'0000 };
	all = 0xffff'0000'0000'ffff;
	active = WHITE;
}

void Position::generate_pseudolegal_moves(std::vector<Move>& to_add_in) {
	for (Bb_iterator i(get_position(PAWN, active)); i.not_ended(); ++i)
		add_moves_by_attack(*i, calc_pawn_attack(*i, get_position(), active), to_add_in);
	for (Bb_iterator i(get_position(BISHOP, active) | get_position(QUEEN, active)); i.not_ended(); ++i)
		add_moves_by_attack(*i, calc_bishop_attack(*i, get_position()), to_add_in);
	for (Bb_iterator i(get_position(KNIGHT, active)); i.not_ended(); ++i)
		add_moves_by_attack(*i, calc_knight_attack(*i), to_add_in);
	for (Bb_iterator i(get_position(ROOK, active) | get_position(QUEEN, active)); i.not_ended(); ++i)
		add_moves_by_attack(*i, calc_rook_attack(*i, get_position()), to_add_in);
	for (Bb_iterator i(get_position(KING, active)); i.not_ended(); ++i)
		add_moves_by_attack(*i, calc_king_attack(*i), to_add_in);
}

void Position::do_move(Move m) {
	active = invert(active);

	switch (move_type(m)) {
		case NORMAL: do_normal_move(m); break;
		case CASTLE: do_castling(m); break;
		case EN_PASSANT: do_en_passant(m); break;
		case PROMOTION: do_promotion(m); break;
	};

	/* update_history(m); */
	/* update_state(); */
}
void Position::undo_move() {
	*this = (*history)[history->size() - 1];
}
bool Position::is_legal(Move m) const {
	switch (move_type(m)) {
		case NORMAL: return is_legal_normal_move(m);
		case CASTLE: return is_legal_castling(m);
		case EN_PASSANT: return is_legal_en_passant(m);
		case PROMOTION: return is_legal_promotion(m);
	};
}
void Position::report_lack_of_legal_moves() {
	if (state == CHECK)
		state = WIN;
	else
		state = DRAW;
}
bool Position::operator==(Position other) const {
	return
		get_position(PAWN) == other.get_position(PAWN) and
		get_position(BISHOP) == other.get_position(BISHOP) and
		get_position(KNIGHT) == other.get_position(KNIGHT) and
		get_position(ROOK) == other.get_position(ROOK) and
		get_position(QUEEN) == other.get_position(QUEEN) and
		get_position(KING) == other.get_position(KING) and

		get_position(BLACK) == other.get_position(BLACK);
}

void Position::add_moves_by_attack(Square from, Bitboard attack, std::vector<Move>& to_add_in) {
	if ( (attack & ~get_position(active)) == 0 )
		return;
	for (Bb_iterator to(attack & ~get_position(active)); to.not_ended(); ++to)
		to_add_in.push_back( build_move(from, *to) );
}

void Position::do_normal_move(Move m) {
	move_piece(from(m), to(m));
}
void Position::do_castling(Move m) {
	switch(castle_type(m)) {
		case WHITE_OO:
			move_piece(E1, G1);
			move_piece(H1, F1);
			break;
		case WHITE_OOO:
			move_piece(E1, C1);
			move_piece(A1, D1);
			break;
		case BLACK_OO:
			move_piece(E8, G8);
			move_piece(H8, F8);
			break;
		case BLACK_OOO:
			move_piece(E8, C8);
			move_piece(A8, D8);
			break;
	};
}
void Position::do_en_passant(Move m) {
	move_piece(from_by_file(m), to_by_file(m));
	erase_piece(square( to_by_file(m), en_passant_rank[side(m)] ));
}
void Position::do_promotion(Move m) {
	move_piece(from_by_file(m), to_by_file(m));
	set_piece(to_by_file(m), new_piece(m), side(m));
}

bool Position::is_legal_normal_move(Move m) const {
	return true;
}
bool Position::is_legal_castling(Move m) const {
	return true;
}
bool Position::is_legal_en_passant(Move m) const {
	return true;
}
bool Position::is_legal_promotion(Move m) const {
	return true;
}

void Position::update_history(Move m) {
	history->push_back(*this);
	if (move_type(m) == NORMAL and getbit(all, m))
		last_irrevers_move = history->size();
}
void Position::update_state() {
	if (is_draw_by_rule50() or is_draw_by_repetitions()) {
		state = DRAW;
		return;
	}
	if (is_check())
		state = CHECK;
}
void Position::update_castle_rights() {
	castlerights &=	~(getbit(get_position(WHITE), H1) << WHITE_OO);
	castlerights &= ~(getbit(get_position(WHITE), A1) << WHITE_OOO);
	castlerights &=	~(getbit(get_position(WHITE), H8) << BLACK_OO);
	castlerights &=	~(getbit(get_position(WHITE), A8) << BLACK_OOO);

	castlerights &= ~(3*getbit(get_position(), E1));
	castlerights &= ~(3*getbit(get_position(), E8) << 2);
}

bool Position::is_check() const {
	return calc_attackers(bsf(get_position(KING, active)), invert(active));
}
bool Position::is_draw_by_rule50() const {
	return history->size() - last_irrevers_move == 50;
}
bool Position::is_draw_by_repetitions() const {
	int c = 0;
	for (size_t i = last_irrevers_move; i < history->size(); i++)
		c += *this == (*history)[i - 1];
	return c == 4;

}

Bitboard Position::calc_attackers(Square sq, Side by) const {
	Bitboard blockers = all & ~(1ull << sq);
	return
		calc_knight_attack(sq) & get_position(KNIGHT, by) |
		calc_bishop_attack(sq, blockers) & (get_position(BISHOP, by) | get_position(QUEEN, by)) |
		calc_rook_attack  (sq, blockers) & (get_position(ROOK,   by) | get_position(QUEEN, by)) |
		calc_pawn_attack(sq, blockers, invert(by)) & get_position(PAWN, by);
}
Bitboard Position::calc_pinned() const {
	Square sq = bsf(get_position(KING, active));
	return 0;
}

void Position::set_piece(Square sq, Piece p, Side s) {
	erase_piece(sq);
	set_1(by_type[p], sq);
	set_1(by_side[s], sq);
	set_1(all, sq);
}
void Position::erase_piece(Square s) {
	set_0(by_type[PAWN], s);
	set_0(by_type[BISHOP], s);
	set_0(by_type[KNIGHT], s);
	set_0(by_type[ROOK], s);
	set_0(by_type[QUEEN], s);
	set_0(by_type[KING], s);

	set_0(by_side[WHITE], s);
	set_0(by_side[BLACK], s);

	set_0(all, s);
}
void Position::move_piece(Square from, Square to) {
	mv(by_type[PAWN], from, to);
	mv(by_type[BISHOP], from, to);
	mv(by_type[KNIGHT], from, to);
	mv(by_type[ROOK], from, to);
	mv(by_type[QUEEN], from, to);
	mv(by_type[KING], from, to);

	mv(by_side[WHITE], from, to);
	mv(by_side[BLACK], from, to);

	mv(all, from, to);
}
