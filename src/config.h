#ifndef CONFIG
#define CONFIG

#include <array>
#include <iostream>

#include "base.h"

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
  int depth, wight, height;
  std::string etcdir;
  std::array<PlayerType, 2> players;
  std::array<Color, 2> square_color;
  std::array<Color, 2> inbordered_color;
  Color background_color;
  std::array<std::array<std::string, 7>, 3> texture_names;

  inline std::string get_texture_path(Piece p, Side s) {
    return etcdir + texture_names[s][p];
  }
};

Config generate_config(int argc, char *argv[]);

#endif
