#pragma once

#include "ofMain.h"

class BarChart {

  public:
    BarChart();
    void draw(int x, int y, int w, int h);
    float* getData();
    void setData(float* newData, int newLength);
    void setRange(float min, float max);

  float* data;
  int length;

  float max;
  float min;
};
