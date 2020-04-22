#pragma once

#include <vector>

#include "screen.h"
#include "spritemap.h"

#include "camera.h"
#include "enemy.h"
#include "game_state.h"
#include "map.h"
#include "player.h"

class LevelScreen : public Screen {
  public:

    enum class State { Play, Pause, FadeOut };

    LevelScreen(GameState state);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;
    std::string get_music_track() const override { return "spooky.ogg"; }

  private:

    static constexpr int kFadeTime = 1000;

    GameState gs_;
    State state_;
    SpriteMap sprites_;
    Camera camera_;
    Map map_;
    Player p1_, p2_;
    std::vector<Enemy> enemies_;
    bool control_inverted_;
    int fade_timer_;
};
