#pragma once

class GameState {
  public:

    GameState();

    void next_level();

    int level() const;

  private:

    int level_ = 1;

};
