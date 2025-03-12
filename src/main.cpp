#include <iostream>
#include <raylib/raylib.h>

const int SCREEN_WIDTH = GetScreenWidth();
const int SCREEN_HEIGHT = GetScreenHeight();
float dt;

typedef struct Animation {
  int first;
  int last;
  int cur;
  int step;
  int tile_size;
  int begin_frame;
  int end_frame;
  float speed;
  float duration;
} Animation;

void animation_update(Animation *self) {
  self->duration = self->duration - dt;
  if (self->duration <= 0) {
    self->duration = self->speed;
    self->cur = self->cur + self->step;

    if (self->cur > self->last) {
      self->cur = self->begin_frame;
    }
  }
}

Rectangle animation_frame(Animation *self, int nframes_row) {
  int x = self->cur % nframes_row * self->tile_size;
  int y = self->cur / nframes_row * self->tile_size;
  return (Rectangle){.x = (float)x,
                     .y = (float)y,
                     .width = (float)self->tile_size,
                     .height = (float)self->tile_size};
}

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
  InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, "openShimeji");
  Texture2D playeridle_texture =
      LoadTexture("../assets/CatPackFree/CatPackFree/Idle.png");
  while (!WindowShouldClose()) {
    dt = GetFrameTime();
    BeginDrawing();
    ClearBackground(BLANK);
    DrawTexturePro(playeridle_texture, animation_frame(&anim, 9),
                   {10, 10, 42, 42}, {0, 0}, 0.0f, WHITE);
    animation_update(&anim);
    if (IsKeyPressed(KEY_ESCAPE)) {
      break;
    }
    EndDrawing();
  }
  UnloadTexture(playeridle_texture);
  CloseWindow();
}
