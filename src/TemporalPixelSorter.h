#pragma once

#include "ofMain.h"
#include "FrameSequence.h"

class TemporalPixelSorter {

  public:
    TemporalPixelSorter(FrameSequence* frameSequence);
    void sort();

    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    void loadPixels();
    void updatePixels();
    float* getBrightnessByTime(int x, int y);

  private:
    FrameSequence* frameSequence;
    ofColor* pixelColors;

    int frameCount;
    int frameWidth;
    int frameHeight;

    static struct {
      bool operator()(const ofColor &c0, const ofColor &c1) {
        float v0 = c0.getLightness();
        float v1 = c1.getLightness();
        if (v0 != v1) return v0 < v1;
        if (c0.r != c1.r) return c0.r < c1.r;
        if (c0.g != c1.g) return c0.g < c1.g;
        return c0.b < c1.b;
      }
    } comparePixel;
};
