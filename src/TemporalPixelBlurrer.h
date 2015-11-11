#pragma once

#include "ofMain.h"
#include "FrameSequence.h"

#define NUM_COLORS 3

class TemporalPixelBlurrer {

  public:
    TemporalPixelBlurrer(FrameSequence* frameSequence, int r);
    void step();

    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    void loadPixels();
    void updatePixels();

  private:
    void blur(unsigned char* temporalColumn);
    void blurForward(unsigned char* temporalColumn);
    void blurReverse(unsigned char* temporalColumn);

    FrameSequence* frameSequence;
    unsigned char* pixels;

    int frameCount;
    int frameWidth;
    int frameHeight;

    int radius;

    int blurDistance;
    int* runningSum;
    std::queue<unsigned char>* runningValues;

    static bool comparePixel(const ofColor &c0, const ofColor &c1) {
      float v0 = c0.getLightness();
      float v1 = c1.getLightness();
      if (v0 != v1) return v0 < v1;
      if (c0.r != c1.r) return c0.r < c1.r;
      if (c0.g != c1.g) return c0.g < c1.g;
      return c0.b < c1.b;
    }
};

