#ifndef RAND_H
#define RAND_H
#include <random>
int get_rand(int start, int end) {
  std::random_device rd;  // Seed
  std::mt19937 gen(rd()); // Mersenne Twister PRNG
  std::uniform_int_distribution<int> dist(start,
                                          end); // Random int from 1 to 100

  int randomNum = dist(gen);
  return randomNum;
}
#endif // !RAND_H
