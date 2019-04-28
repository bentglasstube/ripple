#include "title_screen.h"

#include "level_screen.h"

TitleScreen::TitleScreen(GameState state) :
  gs_(state), bg_("title.png"), text_("text.png"), timer_(0) {}

bool TitleScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  timer_ = (timer_ + elapsed) % 1000;

  if (!audio.music_playing()) audio.play_music("title.ogg");

  if (input.key_pressed(Input::Button::A) || input.key_pressed(Input::Button::Start)) {
    audio.stop_music();
    return false;
  }

  return true;
}

void TitleScreen::draw(Graphics& graphics) const {
  bg_.draw(graphics);
  if (timer_ < 500) text_.draw(graphics, "Press Start", 128, 200, Text::Alignment::Center);
}

Screen* TitleScreen::next_screen() const {
  LevelScreen* s = new LevelScreen(gs_);
  return s;
}
