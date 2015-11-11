#include "TemporalPixelBlurrer.h"

TemporalPixelBlurrer::TemporalPixelBlurrer(FrameSequence* fs, int r) {
  frameSequence = fs;

  frameCount = frameSequence->getFrameCount();
  frameWidth = frameSequence->getFrameWidth();
  frameHeight = frameSequence->getFrameHeight();

  radius = r;

  blurDistance = 3;
  runningSum = new int[NUM_COLORS];
  runningValues = new std::queue<unsigned char>[NUM_COLORS];

  pixels = new unsigned char[frameCount * frameWidth * frameHeight * NUM_COLORS];

  loadPixels();
}

void TemporalPixelBlurrer::loadPixels() {
  unsigned char* source = frameSequence->getPixels();

  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        for (int c = 0; c < NUM_COLORS; c++) {
          pixels[x * frameHeight * frameCount * NUM_COLORS + y * frameCount * NUM_COLORS + i * NUM_COLORS + c] =
            source[i * frameWidth * frameHeight * NUM_COLORS + y * frameWidth * NUM_COLORS + x * NUM_COLORS + c];
        }
      }
    }
  }
}

void TemporalPixelBlurrer::step() {
  for (int x = 0; x < frameWidth; x++) {
    for (int y = 0; y < frameHeight; y++) {
      unsigned char* temporalColumn = pixels + (x * frameHeight * frameCount * NUM_COLORS + y * frameCount * NUM_COLORS);
      blur(temporalColumn);
    }
  }
}

void TemporalPixelBlurrer::updatePixels() {
  unsigned char* source = frameSequence->getPixels();

  for (int i = 0; i < frameCount; i++) {
    for (int x = 0; x < frameWidth; x++) {
      for (int y = 0; y < frameHeight; y++) {
        for (int c = 0; c < NUM_COLORS; c++) {
          source[i * frameWidth * frameHeight * NUM_COLORS + y * frameWidth * NUM_COLORS + x * NUM_COLORS + c] =
            pixels[x * frameHeight * frameCount * NUM_COLORS + y * frameCount * NUM_COLORS + i * NUM_COLORS + c];
        }
      }
    }
  }
}

int TemporalPixelBlurrer::getFrameCount() {
  return frameCount;
}

int TemporalPixelBlurrer::getFrameWidth() {
  return frameWidth;
}

int TemporalPixelBlurrer::getFrameHeight() {
  return frameHeight;
}

void TemporalPixelBlurrer::blur(unsigned char* temporalColumn) {
  for (int i = 0; i < NUM_COLORS; i++) {
    runningSum[i] = 0;

    while (!runningValues[i].empty()) {
      runningValues[i].pop();
    }
  }

  for (int i = 0; i < frameCount && i < blurDistance; i++) {
    for (int c = 0; c < NUM_COLORS; c++) {
      int index = i * NUM_COLORS + c;
      runningSum[c] += temporalColumn[index];
      runningValues[c].push(temporalColumn[index]);
    }
  }

  for (int i = 0; i < frameCount; i++) {
    for (int c = 0; c < NUM_COLORS; c++) {
      int index = i * NUM_COLORS + c;
      temporalColumn[index] = runningSum[c] / runningValues[c].size();

      if (i + blurDistance < frameCount) {
        int horizonIndex = (i + blurDistance) * NUM_COLORS + c;
        runningSum[c] += temporalColumn[horizonIndex];
        runningValues[c].push(temporalColumn[horizonIndex]);
      }

      if (i - blurDistance >= 0) {
        runningSum[c] -= runningValues[c].front();
        runningValues[c].pop();
      }
    }
  }
}

void TemporalPixelBlurrer::blurForward(unsigned char* temporalColumn) {
  for (int i = 0; i < frameCount; i++) {
    for (int c = 0; c < NUM_COLORS; c++) {
      int sum = 0;
      int count = 0;
      for (int j = i; j < frameCount && j < i + blurDistance; j++) {
        sum += temporalColumn[j * NUM_COLORS + c];
        count++;
      }
      temporalColumn[i * NUM_COLORS + c] = sum / count;
    }
  }
}

void TemporalPixelBlurrer::blurReverse(unsigned char* temporalColumn) {
  for (int i = frameCount - 1; i >= 0; i--) {
    for (int c = 0; c < NUM_COLORS; c++) {
      int sum = 0;
      int count = 0;
      for (int j = i; j >= 0 && j > i - blurDistance; j--) {
        sum += temporalColumn[j * NUM_COLORS + c];
        count++;
      }
      temporalColumn[i * NUM_COLORS + c] = sum / count;
    }
  }
}
