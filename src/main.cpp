#include <Animation.h>
#include <raylib/raylib.h>

const int SCREEN_WIDTH = GetScreenWidth();
const int SCREEN_HEIGHT = GetScreenHeight();
const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;
float dt;

Animation anim = (Animation){.first = 0,
                             .last = 9,
                             .cur = 0,
                             .step = 1,
                             .tile_size = 32,
                             .begin_frame = 0,
                             .end_frame = 9,
                             .speed = 0.1,
                             .duration = 0.1};

int main() {
  SetConfigFlags(FLAG_WINDOW_TRANSPARENT);
  InitWindow(PLAYER_WIDTH, PLAYER_HEIGHT, "openShimeji");
  Texture2D playeridle_texture =
      LoadTexture("../assets/CatPackFree/CatPackFree/Idle.png");
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(playeridle_texture, animation_frame(&anim, 9),
                   {0, 0, PLAYER_WIDTH, PLAYER_HEIGHT}, {0, 0}, 0.0f, WHITE);
    animation_update(&anim, dt);
    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }
    EndDrawing();
  }
  UnloadTexture(playeridle_texture);
  CloseWindow();
}
