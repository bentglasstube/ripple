#pragma once

#include "backdrop.h"
#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "text.h"

#include "game_state.h"

class TitleScreen : public Screen {
  public:

    TitleScreen(GameState state);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override { return "title.ogg"; }

  private:

    GameState gs_;
    Backdrop bg_;
    Text text_;
    int timer_;
};
