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
  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      ofColor* temporalColumn = pixelColors + (x * frameHeight * frameCount + y * frameCount);
      step(temporalColumn);
    }
  }

  stepNum++;
}

void TemporalPixelSorter::step(ofColor* temporalColumn) {
  stepDrift(temporalColumn);
}

void TemporalPixelSorter::stepDrift(ofColor* temporalColumn) {
  for (int i = 0; i < frameCount - 1; i++) {
    ofColor currColor = temporalColumn[i];
    ofColor nextColor = temporalColumn[i+1];
    if (currColor.getLightness() > nextColor.getLightness()) {
      temporalColumn[i+1] = currColor;
      i++;
    }
  }
}

void TemporalPixelSorter::stepJumpSwap(ofColor* temporalColumn) {
  for (int i = 0; i < frameCount; i++) {
    ofColor currColor = temporalColumn[i];
    float currLightness = currColor.getLightness();

    for (int j = i + 1; j < frameCount; j++) {
      ofColor nextColor = temporalColumn[j];
      if (nextColor.getLightness() < currLightness) {
        temporalColumn[i] = nextColor;
        temporalColumn[j] = currColor;
        i = j + 1;
        break;
      }
    }
  }
}

void TemporalPixelSorter::stepRangeSort(ofColor* temporalColumn) {
  int size = 5;
  for (int i = stepNum % size; i < frameCount - size; i += size) {
    std::sort(temporalColumn + i, temporalColumn + i + size, comparePixel);
  }
}

void TemporalPixelSorter::stepPairSwap(ofColor* temporalColumn) {
  ofColor temp;
  for (int i = stepNum % 2; i < frameCount - 1; i += 2) {
    if (comparePixel(temporalColumn[i+1], temporalColumn[i])) {
      temp = temporalColumn[i];
      temporalColumn[i] = temporalColumn[i+1];
      temporalColumn[i+1] = temp;
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

