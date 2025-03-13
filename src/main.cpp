#include <Animation.h>
#include <raylib/raylib.h>
#include <string>
#include <vector>

const int SCREEN_WIDTH = GetScreenWidth();
const int SCREEN_HEIGHT = GetScreenHeight();
const int PLAYER_WIDTH = 100;
const int PLAYER_HEIGHT = 100;
float dt;

class Object {
  std::string name;
  int pos_x;
  int pos_y;
  int obj_width;
  int obj_height;
  int current_animation;
  std::vector<Animation> animations;

  void draw() {
    Animation cur_anim = animations[current_animation];
    Texture2D cur_tex = cur_anim.texture_file;
    DrawTexturePro(
        cur_tex, cur_anim.animation_frame(),
        {(float)pos_x, (float)pos_y, (float)obj_width, (float)obj_height},
        {0, 0}, 0.0f, WHITE);
  }

  void anim_update() {
    Animation cur_anim = animations[current_animation];
    cur_anim.animation_update(dt);
  }
};

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
