#include "TemporalPixelSorter.h"

TemporalPixelSorter::TemporalPixelSorter(FrameSequence* fs) {
  frameSequence = fs;

  frameCount = frameSequence->getFrameCount();
  frameWidth = frameSequence->getFrameWidth();
  frameHeight = frameSequence->getFrameHeight();

  pixelColors = new ofColor[frameCount * frameWidth * frameHeight];

  loadPixels();
}

void TemporalPixelSorter::loadPixels() {
  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        ofColor c = frameSequence->getColor(i, x, y);
        pixelColors[x * frameHeight * frameCount + y * frameCount + i] = c;
      }
    }
  }
}

void TemporalPixelSorter::sort() {
  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      ofColor* temporalColumn = pixelColors + (x * frameHeight * frameCount + y * frameCount);
      std::sort(temporalColumn, temporalColumn + frameCount, comparePixel);
    }
  }
}

void TemporalPixelSorter::updatePixels() {
  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        frameSequence->setColor(i, x, y, pixelColors[x * frameHeight * frameCount + y * frameCount + i]);
      }
    }
  }
}

int TemporalPixelSorter::getFrameCount() {
  return frameCount;
}

int TemporalPixelSorter::getFrameWidth() {
  return frameWidth;
}

int TemporalPixelSorter::getFrameHeight() {
  return frameHeight;
}

ofColor* TemporalPixelSorter::getColorsByTime(int x, int y) {
  ofColor* data = new ofColor[frameCount];
  memcpy(data, pixelColors + x * frameHeight * frameCount + y * frameCount, frameCount * 4);
  return data;
}

float* TemporalPixelSorter::getLightnessByTime(int x, int y) {
  float* data = new float[frameCount];
  for (int i = 0; i < frameCount; i++) {
    data[i] = ofMap(
        pixelColors[x * frameHeight * frameCount + y * frameCount + i].getLightness(),
        0, 255, 0, 1);
  }
  return data;
}

