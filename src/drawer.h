#ifndef DRAWER
#define DRAWER

#include <array>
#include <string>
#include <tuple>

#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>

#include "base.h"
#include "position.h"
#include "config.h"


enum EventType {
	PICKSQ,
	UNDOMOVE,
	EXIT,
	NONE_EVENT
};
struct Event {
	EventType type = NONE_EVENT;
	Square picked = NONE_SQUARE;
};

class Drawer {
	sf::RenderWindow& window;
	int square_size;
	Position const& pos;
	Config cfg;
	std::array< std::array<sf::Texture, 6>, 2 >textures;
	Bitboard bordered = 0;
public:
	Drawer(sf::RenderWindow&, Position const&, Config c);
	Event wait_event();
	void redraw();

	inline void border(Square sq) { set_1(bordered, sq); }
	inline void unborder_all() { bordered = 0; }

private:
	sf::Color to_sf_color(Color);
};


#endif // #ifndef DRAWER
