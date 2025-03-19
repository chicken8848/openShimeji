#ifndef PET_H
#define PET_H

#include "PhysicsObject.h"

enum PetState { AWAKE, SLEEPING, AFFECTION, HUNGRY, FALLING, CARRIED, HUNTING };
enum Personality { HUMAN, HUNTER, INQUISITIVE, CANTANKEROUS };

class Pet : public PhysicsObject {
public:
  int max_affection;
  int affection;
  int hunger;
  int rest;
  int personality;
  Pet();

  void print_personality_descriptions() {}
};

#endif // !PET_H
