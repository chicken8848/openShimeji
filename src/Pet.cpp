#ifndef PET_H
#define PET_H

#include "Rand.h"
#include <GlobalMouse.h>
#include <cmath>
#include <unordered_map>
#define Font XFont

#include "PhysicsObject.h"
#include "raylib/raylib.h"
#include <algorithm>
#include <iostream>

#undef Font

enum PetState {
  IDLE,
  SLEEPING,
  AFFECTION,
  HUNGRY,
  FALLING,
  CARRIED,
  HUNTING,
  PLAYFUL,
  EATING,
  SLEEPY,
  CRYING
};
enum PetDirection { LEFT = 1, RIGHT = -1 };
enum Personality { HUMAN, HUNTER, INQUISITIVE, CANTANKEROUS };

class Pet : public PhysicsObject {
public:
  int max_affection;
  int affection;
  float hunger;
  // cats sleep for about 15 hours a day most awake at dusk and dawn
  float tired;
  int personality;
  PetState state;
  PetState last_state;
  PetDirection direction;
  Vector2 last_mouse;
  std::vector<PetState> state_stack;
  std::unordered_map<int, int> anim_map;
  Pet(int width, int height, int mass) : PhysicsObject(width, height, mass) {
    this->direction = LEFT;
    this->tired = 0;
    this->hunger = 0;
    this->state = IDLE;
    this->state_stack.push_back(IDLE);
  }

  bool is_mouse_in_frame() {
    std::pair<int, int> mouse_pos = GetGlobalMouseCoordinates();
    Vector2 mouse_vector = {(float)mouse_pos.first, (float)mouse_pos.second};
    return mouse_vector.x > this->pos.x && mouse_vector.y > this->pos.y &&
           mouse_vector.x < this->pos.x + this->obj_width &&
           mouse_vector.y < this->pos.y + this->obj_height;
  }

  bool check_collision(Vector2 single_point) {
    bool x_overlap = single_point.x > this->pos.x &&
                     single_point.x < this->pos.x + this->obj_width;
    bool y_overlap = single_point.y > this->pos.y &&
                     single_point.y < this->pos.y + this->obj_height;
    return x_overlap && y_overlap;
  }

  void mouse_drag_callback() {
    float dt = GetFrameTime();
    std::pair<int, int> mouse_pos = GetGlobalMouseCoordinates();
    this->pos.x = mouse_pos.first - 0.5 * this->obj_width;
    this->pos.y = mouse_pos.second - 0.5 * this->obj_height;
    Vector2 new_velocity = {(mouse_pos.first - last_mouse.x) / dt,
                            (mouse_pos.second - last_mouse.y) / dt};
    update_velocity(new_velocity);
    last_mouse.x = mouse_pos.first;
    last_mouse.y = mouse_pos.second;
    SetWindowPosition(this->pos.x, this->pos.y);
  }

  PetState handle_idle_state() {
    if (is_mouse_in_frame()) {
      if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT)) {
        this->state_stack.push_back(EATING);
        return EATING;
      } else if (GetMouseDelta().x > 10) {
        return AFFECTION;
      } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT)) {
        this->state_stack.push_back(EATING);
        return EATING;
      }
    } else if (this->tired > 80) {
      this->state_stack.push_back(SLEEPING);
      return SLEEPING;
    }
    this->state_stack.push_back(IDLE);
    return IDLE;
  }

  PetState pop_state_stack() {
    PetState state = state_stack.back();
    if (state_stack.size() > 0) {
      state_stack.pop_back();
    }
    return state;
  }

  PetState handle_sleep_state() {
    if (this->tired <= 0) {
      return pop_state_stack();
    }
    if (this->state_stack.back() != SLEEPING) {
      state_stack.push_back(SLEEPING);
    }
    return SLEEPING;
  }

  PetState handle_affection_state() {
    if (is_mouse_in_frame()) {
      return AFFECTION;
    } else
      return state_stack.back();
  }

  PetState handle_playful_state() {
    int chance = get_rand(0, 1000);
    if (chance > 950) {
      return pop_state_stack();
    }
    if (state_stack.back() != PLAYFUL) {
      state_stack.push_back(PLAYFUL);
    }
    return PLAYFUL;
  }

  PetState handle_hunger_state() {
    if (this->hunger < 0) {
      return pop_state_stack();
    }
    if (state_stack.back() != EATING) {
      state_stack.push_back(EATING);
    }
    return EATING;
  }

  PetState handle_sleepy_state() {
    int chance = get_rand(0, 1000);
    if (chance > 750) {
      return pop_state_stack();
    }
    if (state_stack.back() != SLEEPY) {
      state_stack.push_back(SLEEPY);
    }
    return SLEEPY;
  }

  PetState handle_crying_state() {
    if (this->affection > 25) {
      return pop_state_stack();
    }
    if (state_stack.back() != CRYING) {
      state_stack.push_back(CRYING);
    }
    return CRYING;
  }

  PetState handle_falling_state() {
    if (std::abs(get_velocity().y) < 20.0f) {
      return state_stack.back();
    } else
      return FALLING;
  }

  PetState determine_state() {
    switch (this->state) {
    case IDLE:
      return handle_idle_state();
      break;
    case SLEEPING:
      return handle_sleep_state();
      break;
    case AFFECTION:
      return handle_affection_state();
      break;
    case HUNGRY:
      break;
    case FALLING:
      return handle_falling_state();
      break;
    case CARRIED:
      return this->last_state;
      break;
    case HUNTING:
      break;
    case PLAYFUL:
      return handle_playful_state();
      break;
    case EATING:
      return handle_hunger_state();
      break;
    case SLEEPY:
      return handle_sleepy_state();
      break;
    case CRYING:
      return handle_crying_state();
      break;
    default:
      return handle_idle_state();
      break;
    }
  }

  void update_state() {
    PetState last_state = this->state;
    if (std::abs(get_velocity().y) > 10.0f) {
      this->state = FALLING;
    }
    if (is_mouse_in_frame()) {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        this->state = CARRIED;
      } else
        this->state = determine_state();
    } else
      this->state = determine_state();
    if (last_state != this->state) {
      update_animations();
    }
  }

  void change_direction() {
    if (get_velocity().x > 0) {
      this->direction = RIGHT;
    } else if (get_velocity().x < 0)
      this->direction = LEFT;
  }

  void update_animations() {
    this->current_animation = anim_map.at(this->state);
    change_direction();
  }

  void update_pet(float dt, std::vector<PhysicsObject> &collision_objs,
                  float screen_width, float screen_height) {
    update_state();
    switch (this->state) {
    case CARRIED:
      mouse_drag_callback();
      anim_update(dt);
      draw(this->direction);
      break;
    default:
      update_attributes();
      update_physics(dt, collision_objs, screen_width, screen_height);
      anim_update(dt);
      draw(this->direction);
      break;
    }
  }

  void add_animation_with_state(PetState state, char *file_path, int first,
                                int last, int tile_size, int begin_frame,
                                int end_frame, int anim_type, float speed,
                                int nframes_row) {
    this->anim_map.insert({state, this->animations.size()});
    add_animation(file_path, first, last, tile_size, begin_frame, end_frame,
                  anim_type, speed, nframes_row);
  }

  void update_attributes() {
    float dt = GetFrameTime();
    switch (this->state) {
    case SLEEPING:
      this->tired -= dt;
      break;
    case AFFECTION:
      this->affection += dt;
      break;
    case EATING:
      this->hunger -= dt;
      break;
    default:
      if (tired < 60 * 60) {
        this->tired += dt;
      }
      if (hunger < 1000) {
        this->hunger += dt;
      }
      break;
    }
  }
};

#endif // !PET_H
