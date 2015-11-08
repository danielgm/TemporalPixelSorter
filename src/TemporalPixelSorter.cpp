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

