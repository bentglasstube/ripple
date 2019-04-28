#pragma once

#include <random>
#include <vector>

#include "screen.h"

#include "game_state.h"
#include "map.h"
#include "player.h"

class PartyScreen : public Screen {
  public:

    PartyScreen(GameState state);

    bool update(const Input& input, Audio& audio, unsigned int elapsed) override;
    void draw(Graphics& graphics) const override;

    Screen* next_screen() const override;

  private:

    GameState gs_;
    Map map_;
    std::vector<Player> peeps_;
    std::vector<Fireball> fireworks_;
    std::mt19937 rand_;
};
