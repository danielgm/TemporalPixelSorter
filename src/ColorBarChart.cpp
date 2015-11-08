#include "ColorBarChart.h"

ColorBarChart::ColorBarChart() {
  data = NULL;
  length = 0;
}

void ColorBarChart::draw(int x, int y, int width, int height) {
  ofSetColor(255, 0, 0);
  ofNoFill();
  ofRect(x, y, width, height);

  int padding = 4;

  if (length > 0) {
    ofSetColor(0, 255, 0);
    ofFill();
    float w = ((float)width - 2 * padding) / length;
    for (int i = 0; i < length; i++) {
      float barHeight = ofClamp(ofMap(data[i].getLightness(), 0, 255, 0, height - 2 * padding), 0, height - 2 * padding);
      ofSetColor(data[i]);
      ofRect(
          x + padding + i * w,
          y + padding + height - 2 * padding - barHeight,
          w - 1,
          barHeight);
    }
  }
}

ofColor* ColorBarChart::getData() {
  return data;
}

void ColorBarChart::setData(ofColor* newData, int newLength) {
  data = newData;
  length = newLength;
}

