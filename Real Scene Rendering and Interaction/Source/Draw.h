#pragma once

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <vector>

using namespace std;

class Draw {
  public:
  
  enum mode;
  int first;
  sizei count;
  sizei primcount;
  enum type;
  void *indices;
  
  void DrawArraysInstancedARB(mode, first, count, primcount);
  void DrawElementsInstancedARB(enum mode, sizei count, enum type, const void *indices, sizei primcount);
};
