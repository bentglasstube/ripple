#include "level_screen.h"

LevelScreen::LevelScreen(GameState state) :
  gs_(state), sprites_("level.png", 4, 16, 16), map_(), player_(false) {}

bool LevelScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_held(Input::Button::Left)) {
    player_.move_left();
  } else if (input.key_held(Input::Button::Right)) {
    player_.move_right();
  } else {
    player_.stop_moving();
  }

  if (input.key_pressed(Input::Button::A)) {
    player_.jump();
  }

  player_.update(map_, elapsed);

  return true;
}

void LevelScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, 0, 0);
  player_.draw(graphics, 0, 0);
}

void LevelScreen::load_level(const std::string& level) {
  map_.load(level);
  player_.set_position(16, 96);
}

Screen* LevelScreen::next_screen() const {
  return nullptr;
}
