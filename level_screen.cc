#include "level_screen.h"

#include <algorithm>
#include <iostream>
#include <fstream>

#include "party_screen.h"

LevelScreen::LevelScreen(GameState state) :
  gs_(state), state_(State::Play),
  sprites_("level.png", 4, 16, 16),
  camera_(), map_(), p1_(false, 16, 96), p2_(true, 16, 96),
  control_inverted_(false), fade_timer_(kFadeTime)
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

        case '=':
          map_.set_tile(x, height, Map::TileType::Bricks);
          break;

        case '\'':
          map_.set_tile(x, height, Map::TileType::Neutral);
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

        case '!':
          map_.set_tile(x, height, Map::TileType::Switch);
          break;

        case 'i':
          map_.set_tile(x, height, Map::TileType::InvSwitch);
          break;

        case 'o':
          map_.set_tile(x, height, Map::TileType::BlockOff);
          break;

        case 'O':
          map_.set_tile(x, height, Map::TileType::InvBlockOff);
          break;

        case 'x':
          map_.set_tile(x, height, Map::TileType::BlockOn);
          break;

        case 'X':
          map_.set_tile(x, height, Map::TileType::InvBlockOn);
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
            Map::TileType t = i ? Map::TileType::InvEmpty : Map::TileType::Empty;
            if (line[x - 1] == '\'') t = Map::TileType::Neutral;
            map_.set_tile(x, height, t);
            enemies_.emplace_back(Enemy::Type::Spark, i, x * 16 + 8, height * 16 + (i ? 0 : 16));
          }
          break;

      }
    }
    ++height;
    map_.set_size(width, height);
  }
  map_.set_size(width, height - 1);
}

bool LevelScreen::update(const Input& input, Audio& audio, unsigned int elapsed) {
  gs_.add_time(elapsed);

  if (fade_timer_ > 0) {
    fade_timer_ -= elapsed;
    if (fade_timer_ < 0) fade_timer_ = 0;
  }

  if (state_ == State::FadeOut) {
    if (fade_timer_ == 0) return false;
  }

  if (state_ == State::Pause) {
    if (input.key_pressed(Input::Button::Start)) {
      state_ = State::Play;
      audio.music_volume(10);
      return true;
    }
  }

  if (input.key_pressed(Input::Button::Start)) {
    state_ = State::Pause;
    audio.music_volume(5);
    return true;
  }

  if (state_ == State::Play) {
    if (input.key_pressed(Input::Button::Select)) {
      Player& o = control_inverted_ ? p1_ : p2_;
      if (!o.done()) {
        control_inverted_ = !control_inverted_;
        audio.play_sample("select.wav");
      }
    }

    Player& p = control_inverted_ ? p2_ : p1_;

    camera_.update(p, map_, elapsed);

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
      if (p.at_switch(map_)) {
        audio.play_sample("switch.wav");
        map_.toggle_blocks();
      } else if (p.shoot()) {
        audio.play_sample("fireball.wav");
      }
    }

    if (input.key_pressed(control_inverted_ ? Input::Button::Down : Input::Button::Up)) {
      if (p.at_door(map_)) {
        audio.play_sample("exit.wav");
        p.exit();
      }
    }

    p1_.update(map_, elapsed);
    p2_.update(map_, elapsed);

    for (auto& enemy : enemies_) {
      enemy.update(map_, elapsed);
      if (p1_.check_fireballs(enemy.hitbox())) {
        audio.play_sample("kill.wav");
        enemy.kill();
      } else if (p2_.check_fireballs(enemy.hitbox())) {
        audio.play_sample("kill.wav");
        enemy.kill();
      } else if (!p1_.done() && enemy.collision(p1_.hitbox())) {
        audio.play_sample("dead.wav");
        p1_.kill();
        p2_.grant_fireballs();
      } else if (!p2_.done() && enemy.collision(p2_.hitbox())) {
        audio.play_sample("dead.wav");
        p2_.kill();
        p1_.grant_fireballs();
      }
    }

    enemies_.erase(std::remove_if(
          enemies_.begin(), enemies_.end(),
          [](const Enemy& e){ return e.dead(); }),
        enemies_.end());

    if (p1_.on_spikes(map_)) {
      audio.play_sample("dead.wav");
      p1_.kill();
      p2_.grant_big_jump();
    }

    if (p2_.on_spikes(map_)) {
      audio.play_sample("dead.wav");
      p2_.kill();
      p1_.grant_big_jump();
    }

    if (p1_.dead() && p2_.dead()) {
      fade_timer_ = kFadeTime / 2;
      state_ = State::FadeOut;
    } else if (p1_.done() && p2_.done()) {
      gs_.next_level(!p1_.dead(), !p2_.dead());
      fade_timer_ = kFadeTime / 2;
      state_ = State::FadeOut;
    }
  }

  return true;
}

void LevelScreen::draw(Graphics& graphics) const {
  map_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  p1_.draw(graphics, camera_.xoffset(), camera_.yoffset());
  p2_.draw(graphics, camera_.xoffset(), camera_.yoffset());

  for (const auto& enemy : enemies_) {
    enemy.draw(graphics, camera_.xoffset(), camera_.yoffset());
  }

  double alpha = 0;
  if (state_ == State::FadeOut) {
    alpha = 1 - (2 * fade_timer_ / (double) kFadeTime);
  } else if (fade_timer_ > 0) {
    alpha = fade_timer_ / kFadeTime;
  } else if (state_ == State::Pause) {
    alpha = 0.5;
  }

  if (alpha > 0) {
    SDL_Rect r = {0, 0, graphics.width(), graphics.height()};
    graphics.draw_rect(&r, 255 * alpha, true);
  }
}

Screen* LevelScreen::next_screen() const {
  if (gs_.level() > GameState::kMaxLevel) return new PartyScreen(gs_);
  return new LevelScreen(gs_);
}
