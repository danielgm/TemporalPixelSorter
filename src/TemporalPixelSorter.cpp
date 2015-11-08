#include "TemporalPixelSorter.h"

void TemporalPixelSorter::sort(FrameSequence* frameSequence) {
  int frameCount = frameSequence->getFrameCount();
  int frameWidth = frameSequence->getFrameWidth();
  int frameHeight = frameSequence->getFrameHeight();

  ofColor* pixelColors = new ofColor[frameCount * frameWidth * frameHeight];

  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        ofColor c = frameSequence->getColor(i, x, y);
        pixelColors[x * frameHeight * frameCount + y * frameCount + i] = c;
      }
    }
  }

  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      ofColor* temporalColumn = pixelColors + (x * frameHeight * frameCount + y * frameCount);
      std::sort(temporalColumn, temporalColumn + frameCount, comparePixel);
    }
  }

  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        frameSequence->setColor(i, x, y, pixelColors[x * frameHeight * frameCount + y * frameCount + i]);
      }
    }
  }

  delete[] pixelColors;
}

