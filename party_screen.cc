#include "party_screen.h"

#include "util.h"

#include "fireball.h"
#include "title_screen.h"

PartyScreen::PartyScreen(GameState state) : gs_(state) {
  rand_.seed(Util::random_seed());

  int width = 16;
  int height = 15;

  map_.set_size(width, height);
  std::uniform_real_distribution<double> block(0, 1);
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      map_.set_tile(x, y, (x == 0 || y == 0 || x == width - 1 || y == height - 1 || block(rand_) < 0.05) ? Map::TileType::Bricks : Map::TileType::Neutral);
    }
  }

  std::uniform_real_distribution<double> xdist(24, 232);

  for (int i = 0; i < gs_.saved_normal(); ++i) {
    peeps_.emplace_back(false, xdist(rand_), 224);
  }
  for (int i = 0; i < gs_.saved_invert(); ++i) {
    peeps_.emplace_back(true, xdist(rand_), 16);
  }
}

bool PartyScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  std::uniform_real_distribution<double> p(0, 1);

  for (auto& peep : peeps_) {
    peep.update(map_, elapsed);

    if (p(rand_) < 0.1) {
      const double a = p(rand_);
      if (a < 0.35) peep.move_left();
      else if (a < 0.70) peep.move_right();
      else if (a < 0.90) peep.stop_moving();
      else peep.jump();
    }
  }

  if (p(rand_) < 0.1) {
    fireworks_.emplace_back(p(rand_) < 0.5, p(rand_) * 224 + 16, p(rand_) * 208 + 16, p(rand_) < 0.5 ? Character::Facing::Left : Character::Facing::Right);
  }

  for (auto& f : fireworks_) {
    f.update(map_, elapsed);
  }

  return true;
}

void PartyScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, 0, 0);

  for (const auto& peep : peeps_) {
    peep.draw(graphics, 0, 0);
  }

  for (const auto& f :fireworks_) {
    f.draw(graphics, 0, 0);
  }
}

Screen* PartyScreen::next_screen() const {
  return new TitleScreen(GameState());
}

