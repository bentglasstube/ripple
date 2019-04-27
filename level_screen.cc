#include "level_screen.h"

LevelScreen::LevelScreen(GameState state) :
  gs_(state),
  text_("text.png"), sprites_("level.png", 4, 16, 16),
  map_(), p1_(false), p2_(true) {
    load_level();
}

bool LevelScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  if (input.key_pressed(Input::Button::Select)) {
    control_inverted_ = !control_inverted_;
  }

  Player& p = control_inverted_ ? p2_ : p1_;

  p1_.stop_moving();
  p2_.stop_moving();

  if (input.key_held(Input::Button::Left)) {
    p.move_left();
  } else if (input.key_held(Input::Button::Right)) {
    p.move_right();
  }

  if (input.key_pressed(Input::Button::A)) {
    p.jump();
  }

  p1_.update(map_, elapsed);
  p2_.update(map_, elapsed);

  if (p1_.dead() && p2_.dead()) {
    load_level();
  } else if (p1_.done(map_) && p2_.done(map_)) {
    gs_.next_level();
    return false;
  }

  return true;
}

void LevelScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, 0, 0);
  p1_.draw(graphics, 0, 0);
  p2_.draw(graphics, 0, 0);

  if (p1_.done(map_)) text_.draw(graphics, "1", 0, 0);
  if (p2_.done(map_)) text_.draw(graphics, "2", 8, 0);
}

void LevelScreen::load_level() {
  map_.load("level" + std::to_string(gs_.level()) + ".txt");
  p1_.init(16, 96);
  p2_.init(16, 96);
}

Screen* LevelScreen::next_screen() const {
  return new LevelScreen(gs_);
}
