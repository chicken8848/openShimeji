#ifndef PET_H
#define PET_H

#include "PhysicsObject.h"

enum PetState { AWAKE, SLEEPING, AFFECTION, HUNGRY, FALLING, CARRIED };

class Pet : public PhysicsObject {
public:
  Pet();
};

#endif // !PET_H
