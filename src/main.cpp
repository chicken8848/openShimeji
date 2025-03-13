#include <Animation.h>
#include <Object.h>
#include <raylib/raylib.h>
#include <vector>

const int SCREEN_WIDTH = GetScreenWidth();
const int SCREEN_HEIGHT = GetScreenHeight();
const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;
float dt;

class PhysicsObject : Object {
  Vector2 velocity;
  Vector2 acceleration;
  std::vector<Vector4> collision_boxes;
};

int main() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  InitWindow(800, 600, "openShimeji");
  Object player = Object(PLAYER_WIDTH, PLAYER_HEIGHT);
  player.add_animation("../assets/CatPackFree/CatPackFree/Idle.png", 0, 9, 32,
                       0, 9, REPEATING, 0.1, 9);
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLANK);
    player.draw();
    player.anim_update(dt);
    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }
    EndDrawing();
  }
  player.destroy();
  CloseWindow();
}
