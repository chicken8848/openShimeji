#include <raylib/raylib.h>
typedef enum AnimationType { REPEATING = 1, ONCE = 2 } AnimationType;

class Animation {
public:
  int first;
  int last;
  int cur;
  int step;
  int tile_size;
  int begin_frame;
  int end_frame;
  int anim_type;
  int nframes_row;
  bool end_flag;
  float speed;
  float duration;
  Texture2D texture_file;

  Animation(int first, int last, int cur, int title_size, int begin_frame,
            int anim_type, float speed, float duration, int nframes_row,
            char *file_path) {
    this->first = first;
    this->last = last;
    this->tile_size = title_size;
    this->begin_frame = begin_frame;
    this->anim_type = anim_type;
    this->speed = speed;
    this->duration = duration;
    this->nframes_row = nframes_row;
    load_texture(file_path);
  }

  void reset() {
    this->duration = this->speed;
    this->end_flag = false;
    this->cur = this->begin_frame;
  }

  void animation_update(float dt) {
    this->duration = this->duration - dt;
    if (this->duration <= 0) {
      this->duration = this->speed;
      this->cur = this->cur + this->step;

      if (this->cur > this->end_frame) {
        switch (this->anim_type) {
        case REPEATING:
          this->cur = this->begin_frame;
          this->end_flag = false;
          break;
        case ONCE:
          this->end_flag = true;
        }
      }
    }
  }

  Rectangle animation_frame() {
    int x = this->cur % this->nframes_row * this->tile_size;
    int y = this->cur / this->nframes_row * this->tile_size;
    return (Rectangle){.x = (float)x,
                       .y = (float)y,
                       .width = (float)this->tile_size,
                       .height = (float)this->tile_size};
  }

private:
  void load_texture(char *file_path) {
    this->texture_file = LoadTexture(file_path);
  }
};
