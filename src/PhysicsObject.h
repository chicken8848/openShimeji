#ifndef PHYSOBJ_H
#define PHYSOBJ_H

#include <Object.h>
#include <algorithm>

class PhysicsObject : public Object {
private:
  Vector2 velocity;
  Vector2 acceleration;
  float mass;
  float dampening;
  std::vector<Vector4> collision_boxes;

public:
  PhysicsObject(int width, int height, float mass = 9999)
      : Object(width, height, {100, 100}) {
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

  void update_physics(float dt, std::vector<PhysicsObject> collision_objs,
                      float screen_width, float screen_height) {
    update_velocity(dt);
    update_position(dt, collision_objs, screen_width, screen_height);
    SetWindowPosition(this->pos.x, this->pos.y);
  }

  bool check_collision(Vector2 pos, PhysicsObject collision_obj) {
    for (Vector4 cb : collision_obj.collision_boxes) {
      bool y_overlap = false;
      bool x_overlap = false;
      if (pos.y + this->obj_height > collision_obj.pos.y &&
          collision_obj.pos.y + cb.w > pos.y) {
        y_overlap = true;
      }
      if (pos.x + this->obj_width > collision_obj.pos.x &&
          collision_obj.pos.x + cb.z > pos.x) {
        x_overlap = true;
      }
      if (y_overlap && x_overlap) {
        return true;
      }
    }
    return false;
  }

  bool check_collision(Vector2 &new_pos,
                       std::vector<PhysicsObject> collision_objs) {
    bool collision = false;
    for (PhysicsObject collision_obj : collision_objs) {
      bool collide = check_collision(new_pos, collision_obj);
      if (collide) {
        new_pos.y = collision_obj.pos.y - this->obj_height;
        if (new_pos.x < collision_obj.pos.x) {
          new_pos.x = collision_obj.pos.x - obj_height;
          apply_x_rebound(GetFrameTime());
        } else if (new_pos.x > collision_obj.pos.x + collision_obj.obj_width) {
          new_pos.x = collision_obj.pos.x + collision_obj.obj_width;
          apply_x_rebound(GetFrameTime());
        }
        collision = true;
      }
    }
    return collision;
  }

  void update_velocity(float dt) {
    Vector2 dv = (Vector2){acceleration.x * dt, acceleration.y * dt};
    this->velocity =
        (Vector2){this->velocity.x + dv.x, this->velocity.y + dv.y};
  }

  void update_velocity(Vector2 velocity) { this->velocity = velocity; }

  Vector2 get_velocity() { return this->velocity; }

  void update_position(float dt, std::vector<PhysicsObject> collision_objs,
                       float screen_width, float screen_height) {
    Vector2 ds = (Vector2){this->velocity.x * dt, this->velocity.y * dt};
    Vector2 new_pos = (Vector2){this->pos.x + ds.x, this->pos.y + ds.y};
    if (check_collision(new_pos, collision_objs)) {
      apply_y_rebound(dt);
      apply_x_dampening(dt);
    }
    if (new_pos.y > screen_height) {
      new_pos.y = screen_height;
      apply_y_rebound(dt);
      apply_x_dampening(dt);
    } else if (new_pos.y < 0) {
      new_pos.y = 0;
      apply_y_rebound(dt);
      apply_x_dampening(dt);
    }
    if (new_pos.x + this->obj_width > screen_width) {
      new_pos.x = screen_width - this->obj_width;
      apply_x_rebound(dt);
      apply_y_dampening(dt);
    } else if (new_pos.x < 0) {
      new_pos.x = 0;
      apply_x_rebound(dt);
      apply_y_dampening(dt);
    }
    this->pos = new_pos;
  }

  void apply_x_dampening(float dt) {
    if (this->velocity.x * this->dampening > 0) {
      this->velocity.x =
          std::max(0.0f, this->velocity.x - this->mass * dt * 1000);
    } else if (this->velocity.x * this->dampening < 0) {
      this->velocity.x =
          std::min(0.0f, this->velocity.x + this->mass * dt * 1000);
    }
  }
  void apply_y_dampening(float dt) {
    if (this->velocity.y * this->dampening > 0) {
      this->velocity.y =
          std::max(0.0f, this->velocity.y - this->mass * dt * 1000);
    } else if (this->velocity.y * this->dampening < 0) {
      this->velocity.y =
          std::min(0.0f, this->velocity.y + this->mass * dt * 1000);
    }
  }
  void apply_y_rebound(float dt) {
    this->velocity.y = this->velocity.y * (float)(-1.0f * this->dampening);
  }
  void apply_x_rebound(float dt) {
    this->velocity.x = this->velocity.x * (float)(-1.0f * this->dampening);
  }
};

#endif // !PHYSOBJ_H
