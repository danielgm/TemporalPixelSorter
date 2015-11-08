#include "TemporalPixelSorter.h"

TemporalPixelSorter::TemporalPixelSorter(FrameSequence* fs) {
  frameSequence = fs;

  stepNum = 0;

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

  stepNum = 0;
}

void TemporalPixelSorter::sort() {
  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      ofColor* temporalColumn = pixelColors + (x * frameHeight * frameCount + y * frameCount);
      std::sort(temporalColumn, temporalColumn + frameCount, comparePixel);
    }
  }
}

void TemporalPixelSorter::step() {
  ofColor temp;

  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      ofColor* temporalColumn = pixelColors + (x * frameHeight * frameCount + y * frameCount);

      for (int i = stepNum % 2; i < frameCount - 1; i += 2) {
        if (comparePixel(temporalColumn[i+1], temporalColumn[i])) {
          temp = temporalColumn[i];
          temporalColumn[i] = temporalColumn[i+1];
          temporalColumn[i+1] = temp;
        }
      }
    }
  }

  stepNum++;
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

