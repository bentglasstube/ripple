#include "level_screen.h"

LevelScreen::LevelScreen(GameState state) :
  gs_(state),
  text_("text.png"), sprites_("level.png", 4, 16, 16),
  map_(), p1_(false), p2_(true) {}

bool LevelScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_held(Input::Button::Left)) {
    p1_.move_left();
    p2_.move_left();
  } else if (input.key_held(Input::Button::Right)) {
    p1_.move_right();
    p2_.move_right();
  } else {
    p1_.stop_moving();
    p2_.stop_moving();
  }

  if (input.key_pressed(Input::Button::A)) {
    p1_.jump();
    p2_.jump();
  }

  p1_.update(map_, elapsed);
  p2_.update(map_, elapsed);

  return true;
}

void LevelScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, 0, 0);
  p1_.draw(graphics, 0, 0);
  p2_.draw(graphics, 0, 0);
}

void LevelScreen::load_level(const std::string& level) {
  map_.load(level);
  p1_.set_position(16, 96);
  p2_.set_position(16, 96);
}

Screen* LevelScreen::next_screen() const {
  return nullptr;
}
