#ifndef POSITION
#define POSITION

#include <array>
#include <vector>

#include "base.h"
#include "bitboard.h"


class Position {
	std::array<Bitboard, 6> by_type;
	std::array<Bitboard, 2> by_side;
	Bitboard all;
	Square to_en_passant;

	Side active;
	State state;
	CastleRights castlerights;
	std::vector<Position>* history;
	size_t last_irrevers_move;

public:
	Position();

	void do_move(Move);
	void undo_move();
	void generate_pseudolegal_moves(std::vector<Move>& to_generate_in);
	bool is_legal(Move) const;
	void report_lack_of_legal_moves();
	bool operator==(Position other) const;

	inline Bitboard get_position(Piece p, Side s) const { return by_type[p] & by_side[s]; }
	inline Bitboard get_position(Piece p) const { return by_type[p]; }
	inline Bitboard get_position(Side s) const { return by_side[s]; }
	inline Bitboard get_position() const { return all; }
	inline Side get_active() const { return active; }
	inline State get_state() const { return state; }

protected:
	void generate_normal_moves(std::vector<Move>& to_generate_in);
	void generate_castles(std::vector<Move>& to_generate_in);
	void generate_en_passants(std::vector<Move>& to_generate_in);
	void generate_promotions(std::vector<Move>& to_generate_in);
	void generate_moves_by_attack(Square from, Bitboard attack, std::vector<Move>& to_generate_in);

	void do_normal_move(Move);
	void do_castle(Move);
	void do_en_passant(Move);
	void do_promotion(Move);

	bool is_legal_normal_move(Move) const;
	bool is_legal_castle(Move) const;
	bool is_legal_en_passant(Move) const;
	bool is_legal_promotion(Move) const;

	void update_history(Move m);
	void update_state();
	void update_to_en_passant(Move m);
	void update_castle_rights();

	bool is_check() const;
	bool is_draw_by_rule50() const;
	bool is_draw_by_repetitions() const;

	Bitboard calc_attackers(Square target, Side by) const;
	Bitboard calc_pinned() const;

	void set_piece(Square, Piece, Side);
	void erase_piece(Square);
	void move_piece(Square, Square);
};

#endif // #ifndef POSITION
