#ifndef PHYSOBJ_H
#define PHYSOBJ_H

#include <Object.h>

class PhysicsObject : public Object {
private:
  Vector2 velocity;
  Vector2 acceleration;
  int mass;
  float dampening;
  std::vector<Vector4> collision_boxes;

public:
  PhysicsObject(int height, int width, int mass = 9999)
      : Object(height, width, {100, 100}) {
    this->mass = mass;
    this->dampening = 1.0f / mass;
  }

  void add_collision_box(Vector2 pos, Vector2 dimen) {
    Vector4 collision_box = {pos.x, pos.y, dimen.x, dimen.y};
    collision_boxes.push_back(collision_box);
  }

  void change_acceleration(Vector2 acceleration) {
    this->acceleration = acceleration;
  }

  void change_velocity(Vector2 velocity) { this->velocity = velocity; }

  void update(float dt, std::vector<PhysicsObject> collision_objs) {
    update_velocity(dt);
    update_position(dt, collision_objs);
    SetWindowPosition(this->pos.x, this->pos.y);
  }

  bool collide(Vector2 box1, Vector2 box2) {}

  bool check_collision(Vector2 pos, std::vector<PhysicsObject> collision_objs) {
    for (PhysicsObject o : collision_objs) {
      for (Vector4 cb : o.collision_boxes) {
        if (pos.y + this->obj_height > o.pos.y && o.pos.y + cb.y > pos.y) {
          return true;
        }
      }
    }
    return false;
  }

  void update_velocity(float dt) {
    Vector2 dv = (Vector2){acceleration.x * dt, acceleration.y * dt};
    this->velocity =
        (Vector2){this->velocity.x + dv.x, this->velocity.y + dv.y};
  }

  void update_position(float dt, std::vector<PhysicsObject> collision_objs) {
    Vector2 ds = (Vector2){this->velocity.x * dt, this->velocity.y * dt};
    Vector2 new_pos = (Vector2){this->pos.x + ds.x, this->pos.y + ds.y};
    if (check_collision(new_pos, collision_objs)) {
      this->velocity.y = this->velocity.y * (float)(-1.0f * this->dampening);
    } else {
      this->pos = new_pos;
    }
  }
};

#endif // !PHYSOBJ_H
