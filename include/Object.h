#ifndef OBJ_H
#define OBJ_H

#include <Animation.h>
#include <string>
#include <vector>

class Object {
public:
  std::string name;
  Vector2 pos;
  int obj_width;
  int obj_height;
  int current_animation = 0;
  std::vector<Animation> animations;

  Object(int obj_width, int obj_height, Vector2 pos = {0, 0});
  void draw(int direction = 1);
  void anim_update(float dt);
  bool add_animation(char *file_path, int first, int last, int tile_size,
                     int begin_frame, int end_frame, int anim_type, float speed,
                     int nframes_row);

  bool destroy();
};

#endif // !OBJ_H
