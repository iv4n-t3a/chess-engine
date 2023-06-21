#include <array>
#include <assert.h>
#include <vector>

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
		0x0800'0000'0000'0008, // QUEEN
		0x1000'0000'0000'0010, // KING
	};
	by_side = { 0x0000'0000'0000'ffff, 0xffff'0000'0000'0000 };
	all = 0xffff'0000'0000'ffff;
	active = WHITE;
	castlerights = 0b1111;
	to_en_passant = NONE_SQUARE;
}

void Position::generate_pseudolegal_moves(std::vector<Move>& g) {
	generate_normal_moves(g);
	generate_castles(g);
	generate_en_passants(g);
	generate_promotions(g);
}

void Position::do_move(Move m) {
	active = invert(active);
	update_history(m); // must be used before aplaying move

	switch (move_type(m)) {
		case NORMAL: do_normal_move(m); break;
		case CASTLE: do_castle(m); break;
		case EN_PASSANT: do_en_passant(m); break;
		case PROMOTION: do_promotion(m); break;
	};

	update_castle_rights();
	update_to_en_passant(m);
	update_state();
}
bool Position::is_legal(Move m) const {
	PosDelta pd = posdelta(m);
	Bitboard next = get_position() & ~pd.rm | pd.add;
	Square king = bsf(get_position(KING, active));
	if (move_type(m) == NORMAL and from(m) == king) king = to(m);
	return not ( calc_attackers(king, invert(active), next) & ~pd.rm );
}
void Position::report_lack_of_legal_moves() {
	if (state == CHECK) state = WIN;
	else state = DRAW;
}
Hash Position::hash() const {
	return
		get_position(PAWN)   * 0x2601'2006'1606'2016 +
		get_position(BISHOP) * 0x1234'5678'90ab'cdef +
		get_position(KNIGHT) * 0x1984'2280'7770'6660 +
		get_position(ROOK)   * 0xACDC'ABBA'BABA'DEDA +
		get_position(QUEEN)  * 0x3141'5926'5358'9793 +
		get_position(KING)   * 0x2718'2818'2845'9045 +
		get_position(BLACK)  * 0xD2D4'D7D5'C2C4'DFE4 +
		active               * 0x1248'1632'6412'8256;
}


void Position::generate_normal_moves(std::vector<Move>& g) {
	for (Bb_iterator i(get_position(PAWN, active)); i.not_ended(); ++i)
		generate_moves_by_attack(*i, calc_pawn_attack(*i, get_position(), active), g);
	for (Bb_iterator i(get_position(BISHOP, active) | get_position(QUEEN, active)); i.not_ended(); ++i)
		generate_moves_by_attack(*i, calc_bishop_attack(*i, get_position()), g);
	for (Bb_iterator i(get_position(KNIGHT, active)); i.not_ended(); ++i)
		generate_moves_by_attack(*i, calc_knight_attack(*i), g);
	for (Bb_iterator i(get_position(ROOK, active) | get_position(QUEEN, active)); i.not_ended(); ++i)
		generate_moves_by_attack(*i, calc_rook_attack(*i, get_position()), g);
	for (Bb_iterator i(get_position(KING, active)); i.not_ended(); ++i)
		generate_moves_by_attack(*i, calc_king_attack(*i), g);
}
void Position::generate_castles(std::vector<Move>& g) {
	if (active == WHITE) {
		if (can_castle(castlerights, WHITE_OO) and not getbit(get_position(), F1) and not getbit(get_position(), G1))
			g.push_back(build_castle(WHITE_OO));
		if (can_castle(castlerights, WHITE_OOO) and not getbit(get_position(), B1) and not getbit(get_position(), C1) and not getbit(get_position(), D1))
			g.push_back(build_castle(WHITE_OOO));
	} else {
		if (can_castle(castlerights, BLACK_OO) and not getbit(get_position(), F8) and not getbit(get_position(), G8))
			g.push_back(build_castle(BLACK_OO));
		if (can_castle(castlerights, BLACK_OOO) and not getbit(get_position(), B8) and not getbit(get_position(), C8) and not getbit(get_position(), D8))
			g.push_back(build_castle(BLACK_OOO));
	}
}
void Position::generate_en_passants(std::vector<Move>& g) {
	/* std::cout << (int)to_en_passant << std::endl; */
	/* if ((1ull << to_en_passant) & (get_position(PAWN, active) >> WEST) & ~FILE_H) { */
	/* 	std::cout << int(rank(to_en_passant) - 1) << " " << (int)rank(to_en_passant) << std::endl; */
	/* 	g.push_back( build_en_passant(rank(to_en_passant) + 1, rank(to_en_passant), active) ); */
	/* } if ((1ull << to_en_passant) & (get_position(PAWN, active) << WEST) & ~FILE_A) { */
	/* 	std::cout << int(rank(to_en_passant) + 1) << " " << (int)rank(to_en_passant) << std::endl; */
	/* 	g.push_back( build_en_passant(rank(to_en_passant) - 1, rank(to_en_passant), active) ); */
	/* } */
}
void Position::generate_promotions(std::vector<Move>& g) {
}
void Position::generate_moves_by_attack(Square from, Bitboard attack, std::vector<Move>& g) {
	if ( (attack & ~get_position(active)) == 0 )
		return;
	for (Bb_iterator to(attack & ~get_position(active)); to.not_ended(); ++to)
		g.push_back( build_move(from, *to) );
}

void Position::do_normal_move(Move m) {
	move_piece(from(m), to(m));
}
void Position::do_castle(Move m) {
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
	move_piece( square(file_from(m), en_passant_rank[side(m)]), square(file_to(m), en_passant_rank[side(m)]) + pawn_direction[side(m)] );
	erase_piece(square( file_to(m), en_passant_rank[side(m)] ));
}
void Position::do_promotion(Move m) {
	erase_piece( square(file_from(m), promotion_rank[side(m)]) );
	erase_piece( square(file_to(m), promotion_rank[side(m)]) + pawn_direction[side(m)]);
	set_piece( square(file_to(m), promotion_rank[side(m)]) + pawn_direction[side(m)], new_piece(m), side(m));
}

void Position::update_history(Move m) {
	if ( move_type(m) == NORMAL and (getbit(get_position(), to(m)) or getbit(get_position(PAWN), from(m))) )
		history.clear();
	history.push_back(hash());
}
void Position::update_state() {
	if (state == CHECK) state = PLAYING;

	if (is_draw_by_rule50() or is_draw_by_repetitions()) state = DRAW;
	else if (is_check()) state = CHECK;
}
void Position::update_castle_rights() {
	castlerights &=	~(!getbit(get_position(WHITE), H1) << WHITE_OO);
	castlerights &= ~(!getbit(get_position(WHITE), A1) << WHITE_OOO);
	castlerights &=	~(!getbit(get_position(BLACK), H8) << BLACK_OO);
	castlerights &=	~(!getbit(get_position(BLACK), A8) << BLACK_OOO);
	castlerights &=	~(0b0011 * !getbit(get_position(WHITE), E1));
	castlerights &=	~(0b1100 * !getbit(get_position(BLACK), E8));
}
void Position::update_to_en_passant(Move m) {
	if ((1ull << from(m)) & (RANK_2 | RANK_7) and (1ull << to(m)) & (RANK_4 | RANK_5) & get_position(PAWN))
		to_en_passant = to(m);
	else
		to_en_passant = NONE_SQUARE;
}

bool Position::is_check() const {
	return calc_attackers(bsf(get_position(KING, active)), invert(active), all);
}
bool Position::is_draw_by_rule50() const {
	return history.size() == 50;
}
bool Position::is_draw_by_repetitions() const {
	int c = 0;
	for (Hash h : history)
		c += hash() == h;
	return c == 3;
}

Bitboard Position::calc_attackers(Square sq, Side by, Bitboard blockers) const {
	return
		calc_knight_attack(sq)                     &  get_position(KNIGHT, by) |
		calc_bishop_attack(sq, blockers)           & (get_position(BISHOP, by) | get_position(QUEEN, by)) |
		calc_rook_attack(sq, blockers)             & (get_position(ROOK, by) | get_position(QUEEN, by)) |
		calc_pawn_attack(sq, blockers, by)         &  get_position(PAWN, by);
}

void Position::set_piece(Square sq, Piece p, Side s) {
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
