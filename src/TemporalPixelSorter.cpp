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

void TemporalPixelSorter::step(ofColor* temporalColumn) {
  stepSimilarLightnessSort(temporalColumn);
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

void TemporalPixelSorter::stepProbabilityDrift(ofColor* temporalColumn) {
  for (int i = 0; i < frameCount - 1; i++) {
    ofColor currColor = temporalColumn[i];
    ofColor nextColor = temporalColumn[i+1];
    if (ofRandom(256) < currColor.getLightness()) {
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

void TemporalPixelSorter::stepSimilarLightnessSort(ofColor* temporalColumn) {
  int startIndex = 0;
  int endIndex;
  while (startIndex < frameCount) {
    int endIndex = getNextIndex(temporalColumn, startIndex);
    std::sort(temporalColumn + startIndex, temporalColumn + endIndex, comparePixel);
    startIndex = endIndex;
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

int TemporalPixelSorter::getNextIndex(ofColor* temporalColumn, int startIndex) {
  return getNextDifferentLightness(temporalColumn, startIndex);
}

int TemporalPixelSorter::getNextDifferentLightness(ofColor* temporalColumn, int startIndex) {
  int lightnessDifferenceThreshold = 30;
  float lightness = temporalColumn[startIndex].getLightness();
  for (int i = startIndex + 1; i < frameCount; i++) {
    if (abs(lightness - temporalColumn[i].getLightness())
        > lightnessDifferenceThreshold) {
      return i;
    }
  }
  return frameCount;
}

int TemporalPixelSorter::getNextDarkLight(ofColor* temporalColumn, int startIndex) {
  int lightnessAbsoluteThreshold = 20;
  float lightness = temporalColumn[startIndex].getLightness();
  bool isLight = lightness > lightnessAbsoluteThreshold;
  for (int i = startIndex + 1; i < frameCount; i++) {
    if (isLight != temporalColumn[i].getLightness() > lightnessAbsoluteThreshold) {
      return i;
    }
  }
  return frameCount;
}

