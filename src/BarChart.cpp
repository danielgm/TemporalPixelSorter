#include "BarChart.h"

BarChart::BarChart() {
  data = NULL;
  length = 0;
  min = 0;
  max = 1;
}

void BarChart::draw(int x, int y, int width, int height) {
  ofSetColor(255, 0, 0);
  ofNoFill();
  ofRect(x, y, width, height);

  int padding = 4;

  if (length > 0) {
    ofSetColor(0, 255, 0);
    ofFill();
    float w = ((float)width - 2 * padding) / length;
    for (int i = 0; i < length; i++) {
      float barHeight = ofClamp(ofMap(data[i], min, max, 0, height - 2 * padding), 0, height - 2 * padding);
      ofRect(
          x + padding + i * w,
          y + padding + height - 2 * padding - barHeight,
          w - 1,
          barHeight);
    }
  }
}

float* BarChart::getData() {
  return data;
}

void BarChart::setData(float* newData, int newLength) {
  data = newData;
  length = newLength;
}

void BarChart::setRange(float newMin, float newMax) {
  min = newMin;
  max = newMax;
}

