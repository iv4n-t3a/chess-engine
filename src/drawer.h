#ifndef DRAWER
#define DRAWER

#include <array>
#include <string>
#include <tuple>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "base.h"
#include "posadapter.h"
#include "config.h"


class Drawer {
	PosAdapter const& pos;
	sf::RenderWindow& window;
	int square_size;
	Config cfg;
	std::array<sf::Texture, SQ_COND_COUNT> textures;
	Bitboard bordered = 0;
public:
	Drawer(sf::RenderWindow&, PosAdapter const&, Config c);
	Square pick_square();
	void redraw();

	inline void border(Square sq) { set_1(bordered, sq); }
	inline void unborder_all() { bordered = 0; }

private:
	sf::Color to_sf_color(Color);
};


#endif // #ifndef DRAWER
