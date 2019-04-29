#pragma once

class GameState {
  public:

    GameState();

    static constexpr int kMaxLevel = 10;

    void next_level(bool normal, bool invert);
    void add_time(unsigned int elapsed);

    int level() const;
    int saved_normal() const;
    int saved_invert() const;
    unsigned long time() const;

  private:

    int level_ = 1;
    int saved_normal_ = 0;
    int saved_invert_ = 0;
    unsigned long elapsed_ = 0;

};
