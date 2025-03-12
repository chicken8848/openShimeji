#include <raylib/raylib.h>
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

void animation_update(Animation *self, float dt) {
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
