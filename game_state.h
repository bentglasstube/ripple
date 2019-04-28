#pragma once

class GameState {
  public:

    GameState();

    void next_level(bool normal, bool invert);

    int level() const;
    int saved_normal() const;
    int saved_invert() const;

  private:

    int level_ = 1;
    int saved_normal_ = 0;
    int saved_invert_ = 0;

};
