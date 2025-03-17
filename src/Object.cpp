#include "raylib/raylib.h"
#include <Animation.h>
#include <Object.h>
#include <string>
#include <vector>

Object::Object(int obj_width, int obj_height, Vector2 pos) {
  this->obj_width = obj_width;
  this->obj_height = obj_height;
  this->pos = pos;
}

void Object::draw() {
  Animation cur_anim = animations[current_animation];
  Texture2D cur_tex = cur_anim.texture_file;
  Vector2 window_pos = GetWindowPosition();
  DrawTexturePro(cur_tex, cur_anim.animation_frame(),
                 {(float)pos.x - window_pos.x, (float)pos.y - window_pos.y,
                  (float)obj_width, (float)obj_height},
                 {0, 0}, 0.0f, WHITE);
}

void Object::anim_update(float dt) {
  Animation *cur_anim = &animations[current_animation];
  cur_anim->animation_update(dt);
}

bool Object::add_animation(char *file_path, int first, int last, int tile_size,
                           int begin_frame, int end_frame, int anim_type,
                           float speed, int nframes_row) {
  Animation new_anim =
      Animation(first, last, begin_frame, 1, tile_size, begin_frame, end_frame,
                anim_type, speed, nframes_row, file_path);
  animations.push_back(new_anim);
  return 1;
}

bool Object::destroy() {
  for (auto i : animations) {
    UnloadTexture(i.texture_file);
  }
  return 1;
}
