#pragma once

#include "ofMain.h"

class ColorBarChart {

  public:
    ColorBarChart();
    void draw(int x, int y, int w, int h);
    ofColor* getData();
    void setData(ofColor* newData, int newLength);

  ofColor* data;
  int length;
};
