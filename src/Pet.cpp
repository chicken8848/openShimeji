#ifndef PET_H
#define PET_H

#include "PhysicsObject.h"

enum PetState { IDLE, SLEEPING, AFFECTION, HUNGRY, FALLING, CARRIED, HUNTING };
enum Personality { HUMAN, HUNTER, INQUISITIVE, CANTANKEROUS };

class Pet : public PhysicsObject {
public:
  int max_affection;
  int affection;
  int hunger;
  // cats sleep for about 15 hours a day most awake at dusk and dawn
  int rest;
  int personality;
  int state;
  Pet();
};

#endif // !PET_H
