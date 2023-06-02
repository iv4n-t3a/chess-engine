#ifndef CONFIG
#define CONFIG

#include <iostream>
#include <array>

#include "posadapter.h"


enum PlayerType {
    HUMAN,
    ENGINE
};
enum ParsingException {
    FORMATERR,
	HELPEXEPT,
};
struct Color {
	uint8_t r, g, b;
};

struct Config {
	std::array<PlayerType, 2> players;
    int
		depth,
		wight,
		height;
	std::string etcdir;
	std::array<Color, 2> square_color;
	Color
			inbordered_color,
			background_color;
	std::array<std::string, SQ_COND_COUNT> texture_names;

	inline std::string get_texture_path(SquareContent sc) {
		return etcdir + texture_names[sc];
	}
};

Config generate_config(int argc, char *argv[]);

#endif
