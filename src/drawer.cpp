#include "drawer.h"

#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

#include "attack.h"
#include "base.h"
#include "bitboard.h"
#include "config.h"
#include "position.h"

Drawer::Drawer(sf::RenderWindow& w, Position const& p, Config c) : window(w), pos(p), cfg(c) {
  sf::Vector2u size = w.getSize();
  square_size = std::min(size.x, size.y) / 8;
  for (Side s : {WHITE, BLACK})
    for (Piece p : {PAWN, KNIGHT, BISHOP, ROOK, QUEEN, KING})
      textures[s][p].loadFromFile(cfg.get_texture_path(p, s));
  redraw();
}
void Drawer::redraw() {
  window.clear(to_sf_color(cfg.background_color));

  for (Square sq = 0; sq < 64; sq++) {
    Piece p = pos.piece_at(sq);
    Side s = pos.side_at(sq);
    sf::RectangleShape shape(sf::Vector2f(square_size, square_size));

    if (p != NONE_PIECE)
      shape.setTexture(&textures[s][p]);

    Bitboard WHITE_SQUARES = 0xaa55'aa55'aa55'aa55;

    Side sq_color = getbit(WHITE_SQUARES, sq) ? WHITE : BLACK;
    if (getbit(bordered, sq))
      shape.setFillColor(to_sf_color(cfg.inbordered_color[sq_color]));
    else
      shape.setFillColor(to_sf_color(cfg.square_color[sq_color]));

    shape.setPosition((7 - sq % 8) * square_size, sq / 8 * square_size);
    window.draw(shape);
  }

  window.display();
}
Event Drawer::wait_event() {
  sf::Event event;
  while (window.isOpen() and window.waitEvent(event)) {
    if (event.type == sf::Event::Closed) {
      window.close();
      exit(0);
    }
    if (event.type == sf::Event::MouseButtonPressed) {
      int x = event.mouseButton.x;
      int y = event.mouseButton.y;
      if (x >= square_size * 8 or y >= square_size * 8)
        return {PICKSQ, NONE_SQUARE};
      Square sq = (7 - x / square_size) + 8 * (y / square_size);
      unborder_all();
      border(sq);
      redraw();
      return {PICKSQ, sq};
    } else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::U) {
      return {UNDOMOVE, NONE_SQUARE};
    } else if (event.type == sf::Event::KeyPressed and event.key.code == sf::Keyboard::Q) {
      return {EXIT, NONE_SQUARE};
    }
  }
  return {NONE_EVENT, NONE_SQUARE};
}

sf::Color Drawer::to_sf_color(Color c) {
  return sf::Color(c.r, c.g, c.b);
}
