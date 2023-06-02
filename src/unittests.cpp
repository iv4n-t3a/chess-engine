#include <assert.h>
#include <iostream>

#include "attack.h"
#include "base.h"
#include "bitboard.h"
#include "moves.h"
#include "position.h"

#include "unittests.h"


void rook_attack_test() {
	std::cout << "Running rook attack test... ";
	Bitboard blockers = 0x0201'0001'0000'0000;
	Bitboard expected = 0x0000'0001'0101'01fe;
	Square sq = 0;
	assert(calc_rook_attack(sq, blockers) == expected);
	std::cout << "OK";
}

void runtests() {
	rook_attack_test();
}
