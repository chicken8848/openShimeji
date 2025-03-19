#include <cstdlib>
#include <window.h>
#define Font XFont

#include "Pet.cpp"
#include "PhysicsObject.h"
#include <Animation.h>
#include <Object.h>
#include <Rand.h>
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

  Pet player = Pet(PLAYER_WIDTH, PLAYER_HEIGHT, 2);
  player.pos = (Vector2){(float)get_rand(0, mon_width),
                         (float)get_rand(0, mon_height / 2)};
  player.add_animation_with_state(IDLE,
                                  "../assets/CatPackFree/CatPackFree/Idle.png",
                                  0, 8, 32, 0, 8, REPEATING, 0.1, 8);
  player.add_animation_with_state(
      CARRIED, "../assets/CatPackPaid/CatPackPaid/Sprites/Dance.png", 0, 3, 32,
      0, 3, REPEATING, 0.1, 3);
  player.add_animation_with_state(
      FALLING, "../assets/CatPackPaid/CatPackPaid/Sprites/Dance.png", 0, 3, 32,
      0, 3, REPEATING, 0.1, 3);
  player.add_animation_with_state(
      AFFECTION, "../assets/CatPackPaid/CatPackPaid/Sprites/Excited.png", 0, 11,
      32, 0, 11, REPEATING, 0.1, 11);
  player.add_animation_with_state(
      SLEEPING, "../assets/CatPackPaid/CatPackPaid/Sprites/Sleep.png", 0, 3, 32,
      0, 3, REPEATING, 0.1, 3);
  player.add_animation_with_state(
      EATING, "../assets/CatPackPaid/CatPackPaid/Sprites/Eating.png", 0, 14, 32,
      0, 14, REPEATING, 0.1, 14);
  player.add_animation_with_state(
      CRYING, "../assets/CatPackPaid/CatPackPaid/Sprites/Cry.png", 0, 3, 32, 0,
      3, REPEATING, 0.1, 3);
  player.add_animation_with_state(
      SLEEPY, "../assets/CatPackPaid/CatPackPaid/Sprites/Sleepy.png", 0, 7, 32,
      0, 7, REPEATING, 0.3, 7);
  player.change_acceleration(GRAVITY);

  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    update_physics_queue(player.pos);
    ClearBackground(BLANK);
    player.update_pet(dt, physics_queue);
    process_input();
    EndDrawing();
  }
  player.destroy();
  CloseWindow();
}
