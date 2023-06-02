#include <iostream>
#include <array>
#include <string>

#include "base.h"

#include "config.h"


inline PlayerType player_type(char l) {
	if (l == 'h')
		return HUMAN;
	else if (l == 'b')
		return ENGINE;
	else
		throw FORMATERR;
}
Config generate_default_config() {
	Config c;

	c.depth = 12;
	c.wight = 600;
	c.height = 800;

	c.etcdir = "/home/ivan/Projects/CHESS/etc/";

	c.square_color[WHITE] = {238, 238, 210};
	c.square_color[BLACK] = {118, 150, 86};

	c.inbordered_color = {186, 202, 68};
	c.background_color = {33, 32, 47};


	std::string ext = "png";

	c.texture_names[WHITE_PAWN] = "white-pawn." + ext;
	c.texture_names[WHITE_BISHOP] = "white-bishop." + ext;
	c.texture_names[WHITE_KNIGHT] = "white-knight." + ext;
	c.texture_names[WHITE_ROOK] = "white-rook." + ext;
	c.texture_names[WHITE_QUEEN] = "white-queen." + ext;
	c.texture_names[WHITE_KING] = "white-king." + ext;

	c.texture_names[BLACK_PAWN] = "black-pawn." + ext;
	c.texture_names[BLACK_BISHOP] = "black-bishop." + ext;
	c.texture_names[BLACK_KNIGHT] = "black-knight." + ext;
	c.texture_names[BLACK_ROOK] = "black-rook." + ext;
	c.texture_names[BLACK_QUEEN] = "black-queen." + ext;
	c.texture_names[BLACK_KING] = "black-king." + ext;

	return c;
}
Config generate_config(int argc, char *argv[]) {
	Config c = generate_default_config();
	int i = 1;

	for (; i < argc; i++) {
		if (argv[i][0] != '-')
			break;

		std::string arg = argv[i];
		if (arg == "-d" or arg == "--depth")
			c.depth = atoi(argv[++i]);
		else if (arg == "-p" or arg == "--program-dir")
			c.etcdir = argv[++i];
		else if (arg == "-w" or arg == "--wight")
			c.wight = atoi(argv[++i]);
		else if (arg == "-h" or arg == "--height")
			c.height = atoi(argv[++i]);
		else if (arg == "-?" or arg == "--help")
			throw HELPEXEPT;
		else
			throw FORMATERR;
	}

	if (argc - i == 0) {
		c.players[WHITE] = HUMAN;
		c.players[BLACK] = ENGINE;
	} else {
		c.players[WHITE] = player_type(argv[i + 0][0]);
		c.players[BLACK] = player_type(argv[i + 1][0]);
	}
	return c;
}
