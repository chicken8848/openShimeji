#include <window.h>
#define Font XFont

#include <Animation.h>
#include <Object.h>
#include <raylib/raylib.h>
#include <vector>

#undef Font

const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;
const Vector2 GRAVITY = {0, 100};
float dt;

class PhysicsObject : public Object {
private:
  const float DAMP_FACTOR = 0.1;
  Vector2 velocity;
  Vector2 acceleration;
  int mass;
  int dampening;
  std::vector<Vector4> collision_boxes;

public:
  PhysicsObject(int height, int width, int mass)
      : Object(height, width, {0, 0}) {
    this->mass = mass;
    this->dampening = this->mass * DAMP_FACTOR;
  }

  void add_collision_box(Vector2 pos, Vector2 dimen) {
    Vector4 collision_box = {pos.x, pos.y, dimen.x, dimen.y};
    collision_boxes.push_back(collision_box);
  }

  void change_acceleration(Vector2 acceleration) {
    this->acceleration = acceleration;
  }

  void change_velocity(Vector2 velocity) { this->velocity = velocity; }

  void update() {
    update_velocity();
    update_position();
    SetWindowPosition(this->pos.x, this->pos.y);
  }

  void check_collision() {}

  void update_velocity() {
    Vector2 dv = (Vector2){acceleration.x * dt, acceleration.y * dt};
    this->velocity =
        (Vector2){this->velocity.x + dv.x, this->velocity.y + dv.y};
  }

  void update_position() {
    Vector2 ds = (Vector2){this->velocity.x * dt, this->velocity.y * dt};
    this->pos = (Vector2){this->pos.x + ds.x, this->pos.y + ds.y};
  }
};

void process_input() {}

int main() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  SetConfigFlags(FLAG_WINDOW_UNDECORATED);
  SetConfigFlags(FLAG_WINDOW_TOPMOST);
  InitWindow(PLAYER_WIDTH, PLAYER_HEIGHT, "openShimeji");
  PhysicsObject player = PhysicsObject(PLAYER_WIDTH, PLAYER_HEIGHT, 10);
  player.add_animation("../assets/CatPackFree/CatPackFree/Idle.png", 0, 9, 32,
                       0, 9, REPEATING, 0.1, 9);
  player.change_acceleration(GRAVITY);

  std::vector<WindowInfo> visible_windows = getVisibleWindows();

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLANK);
    player.draw();
    player.anim_update(dt);
    player.update();
    process_input();
    EndDrawing();
  }
  player.destroy();
  CloseWindow();
}
