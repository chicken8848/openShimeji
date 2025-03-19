#ifndef PET_H
#define PET_H

#include <GlobalMouse.h>
#include <cmath>
#include <unordered_map>
#define Font XFont

#include "PhysicsObject.h"
#include "raylib/raylib.h"
#include <algorithm>

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
  int hunger;
  // cats sleep for about 15 hours a day most awake at dusk and dawn
  int rest;
  int personality;
  PetState state;
  PetDirection direction;
  Vector2 last_mouse;
  std::unordered_map<int, int> anim_map;
  Pet(int width, int height, int mass) : PhysicsObject(width, height, mass) {
    this->direction = LEFT;
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

  PetState determine_state(PetState last_state) {
    if (std::abs(get_velocity().y) > 50.0f)
      return FALLING;
    if (this->rest > 80)
      return SLEEPING;
    if (last_state == SLEEPING && this->rest > 0)
      return SLEEPING;
    if (this->rest > 50 && last_state != SLEEPING) {
      return SLEEPY;
    }
    if (last_state == SLEEPING && this->state == CARRIED) {
      this->affection -= 50;
    }
    if (this->hunger < 0) {
      return CRYING;
    }
    if (is_mouse_in_frame()) {
      return AFFECTION;
    }
    // if (this->affection < 50)
    //  return CRYING;
    if (last_state == EATING && this->hunger > 0) {
      return EATING;
    }
    return IDLE;
  }

  void update_state() {
    PetState last_state = this->state;
    if (is_mouse_in_frame()) {
      if (IsMouseButtonDown(MOUSE_BUTTON_LEFT)) {
        this->state = CARRIED;
      } else if (IsMouseButtonDown(MOUSE_BUTTON_RIGHT)) {
        this->state = EATING;
      } else
        this->state = determine_state(last_state);
    } else
      this->state = determine_state(last_state);
    if (last_state != this->state)
      update_animations();
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

  void update_pet(float dt, std::vector<PhysicsObject> &collision_objs) {
    update_state();
    switch (this->state) {
    case CARRIED:
      mouse_drag_callback();
      anim_update(dt);
      draw(this->direction);
      break;
    default:
      update_attributes();
      update_physics(dt, collision_objs);
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
      this->rest -= 1 * dt;
      break;
    case AFFECTION:
      this->affection += 1 * dt;
      break;
    case EATING:
      this->hunger -= 1 * dt;
      break;
    default:
      this->rest++;
      break;
    }
  }
};

#endif // !PET_H
