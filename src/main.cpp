#include <cstdlib>
#include <window.h>
#define Font XFont

#include "PhysicsObject.h"
#include <Animation.h>
#include <Object.h>
#include <random>
#include <raylib/raylib.h>
#include <vector>

#undef Font

const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;
const float EPSILON = PLAYER_HEIGHT / 4;
const Vector2 GRAVITY = {0, 500};
float dt;
std::vector<PhysicsObject> physics_queue;

void process_input() {}

int get_rand_start(int start, int end) {
  std::random_device rd;  // Seed
  std::mt19937 gen(rd()); // Mersenne Twister PRNG
  std::uniform_int_distribution<int> dist(start,
                                          end); // Random int from 1 to 100

  int randomNum = dist(gen);
  return randomNum;
}

void update_physics_queue(Vector2 illegal_pos) {
  std::vector<WindowInfo> visible_windows = getVisibleWindows();
  std::vector<PhysicsObject> new_queue;
  for (WindowInfo w : visible_windows) {
    Vector2 window_pos = {(float)w.x, (float)w.y};
    if (std::abs(window_pos.y - illegal_pos.y) > EPSILON) {
      PhysicsObject window_obj = PhysicsObject(w.width, w.height);
      Vector2 window_dim = {(float)w.width, (float)w.height};
      window_obj.add_collision_box({0.0f, 0.0f}, {window_dim.x, 1.0f});
      window_obj.pos = window_pos;
      new_queue.push_back(window_obj);
    }
  }
  physics_queue = new_queue;
}

int main() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT | FLAG_WINDOW_UNDECORATED |
                 FLAG_WINDOW_TOPMOST | FLAG_VSYNC_HINT);
  InitWindow(PLAYER_WIDTH, PLAYER_HEIGHT, "openShimeji");
  SetTargetFPS(30);

  int mon_width = GetMonitorWidth(GetCurrentMonitor());
  int mon_height = GetMonitorHeight(GetCurrentMonitor());

  PhysicsObject player = PhysicsObject(PLAYER_WIDTH, PLAYER_HEIGHT, 2);
  player.pos = (Vector2){(float)get_rand_start(0, mon_width),
                         (float)get_rand_start(0, mon_height / 2)};
  player.add_animation("../assets/CatPackFree/CatPackFree/Idle.png", 0, 9, 32,
                       0, 9, REPEATING, 0.1, 9);
  player.change_acceleration(GRAVITY);

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    update_physics_queue(player.pos);
    ClearBackground(BLANK);
    player.draw();
    player.anim_update(dt);
    player.update(dt, physics_queue);
    process_input();
    EndDrawing();
  }
  player.destroy();
  CloseWindow();
}
