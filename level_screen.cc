#include "level_screen.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "party_screen.h"

LevelScreen::LevelScreen(GameState state) :
  gs_(state),
  text_("text.png"), sprites_("level.png", 4, 16, 16),
  map_(), p1_(false, 16, 96), p2_(true, 16, 96),
  control_inverted_(false)
{
  const std::string level_file = "content/level" + std::to_string(gs_.level()) + ".txt";
  std::ifstream reader(level_file);

  int width = 0;
  int height = 0;

  std::string line;
  while (reader) {
    std::getline(reader, line);
    const size_t l = line.length();
    if (width == 0) width = l;
    for (size_t x = 0; x < l; ++x) {
      switch (line[x]) {
        case ' ':
          map_.set_tile(x, height, Map::TileType::Empty);
          break;

        case '.':
          map_.set_tile(x, height, Map::TileType::InvEmpty);
          break;

        case '^':
          map_.set_tile(x, height, Map::TileType::Spikes);
          break;

        case 'v':
          map_.set_tile(x, height, Map::TileType::InvSpikes);
          break;

        case '+':
          map_.set_tile(x, height, Map::TileType::DoorTop);
          break;

        case '-':
          map_.set_tile(x, height, Map::TileType::InvDoorBottom);
          break;

        case 'g':
        case 'G':
          {
            const bool i = line[x] == 'G';
            map_.set_tile(x, height, i ? Map::TileType::InvEmpty : Map::TileType::Empty);
            enemies_.emplace_back(Enemy::Type::Goomba, i, x * 16 + 8, height * 16 + (i ? 0 : 16));
          }

          break;

        case 's':
        case 'S':
          {
            const bool i = line[x] == 'S';
            map_.set_tile(x, height, i ? Map::TileType::InvEmpty : Map::TileType::Empty);
            enemies_.emplace_back(Enemy::Type::Spark, i, x * 16 + 8, height * 16 + (i ? 0 : 16));
          }

      }
    }
    ++height;
    map_.set_size(width, height);
  }
  map_.set_size(width, height - 1);
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

  if (input.key_pressed(Input::Button::B)) {
    p.shoot();
  }

  p1_.update(map_, elapsed);
  p2_.update(map_, elapsed);

  for (auto& enemy : enemies_) {
    enemy.update(map_, elapsed);
    if (p1_.check_fireballs(enemy.hitbox())) {
      enemy.kill();
    } else if (p2_.check_fireballs(enemy.hitbox())) {
      enemy.kill();
    } else if (enemy.collision(p1_.hitbox())) {
      p1_.kill();
      p2_.grant_fireballs();
    } else if (enemy.collision(p2_.hitbox())) {
      p2_.kill();
      p1_.grant_fireballs();
    }
  }

  enemies_.erase(std::remove_if(
        enemies_.begin(), enemies_.end(),
        [](const Enemy& e){ return e.dead(); }),
      enemies_.end());

  if (p1_.on_spikes(map_)) {
    p1_.kill();
    p2_.grant_big_jump();
  }

  if (p2_.on_spikes(map_)) {
    p2_.kill();
    p1_.grant_big_jump();
  }

  if (p1_.dead() && p2_.dead()) {
    return false;
  } else if (p1_.done(map_) && p2_.done(map_)) {
    gs_.next_level(!p1_.dead(), !p2_.dead());
    return false;
  }

  return true;
}

void LevelScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, 0, 0);
  p1_.draw(graphics, 0, 0);
  p2_.draw(graphics, 0, 0);

  for (const auto& enemy : enemies_) {
    enemy.draw(graphics, 0, 0);
  }
}

Screen* LevelScreen::next_screen() const {
  if (gs_.level() > 4) return new PartyScreen(gs_);
  return new LevelScreen(gs_);
}
