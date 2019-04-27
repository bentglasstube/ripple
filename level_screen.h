#pragma once

#include "graphics.h"
#include "input.h"
#include "screen.h"
#include "spritemap.h"

#include "game_state.h"
#include "map.h"
#include "player.h"

class LevelScreen : public Screen {
  public:

    LevelScreen(GameState state);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    void load_level(const std::string& level);

    Screen* next_screen() const override;

  private:

    GameState gs_;
    SpriteMap sprites_;
    Map map_;
    Player player_;
};
