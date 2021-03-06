#pragma once

#include "ofMain.h"
#include "FrameSequence.h"

class TemporalPixelSorter {

  public:
    TemporalPixelSorter(FrameSequence* frameSequence);
    void sort();
    void step();

    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    void loadPixels();
    void updatePixels();

  private:
    void step(ofColor* temporalColumn);
    void stepDrift(ofColor* temporalColumn);
    void stepProbabilityDrift(ofColor* temporalColumn);
    void stepJumpSwap(ofColor* temporalColumn);
    void stepRangeSort(ofColor* temporalColumn);
    void stepSimilarLightnessSort(ofColor* temporalColumn);
    void stepPairSwap(ofColor* temporalColumn);

    int getNextIndex(ofColor* temporalColumn, int startIndex);
    int getNextDifferentLightness(ofColor* temporalColumn, int startIndex);
    int getNextDarkLight(ofColor* temporalColumn, int startIndex);

    FrameSequence* frameSequence;
    ofColor* pixelColors;

    int stepNum;

    int frameCount;
    int frameWidth;
    int frameHeight;

    static bool comparePixel(const ofColor &c0, const ofColor &c1) {
      float v0 = c0.getLightness();
      float v1 = c1.getLightness();
      if (v0 != v1) return v0 < v1;
      if (c0.r != c1.r) return c0.r < c1.r;
      if (c0.g != c1.g) return c0.g < c1.g;
      return c0.b < c1.b;
    }
};

