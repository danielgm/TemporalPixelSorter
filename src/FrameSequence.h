#pragma once

#include "ofMain.h"

class FrameSequence {

  public:
    FrameSequence();
    void clearFrames();
    void loadFrames(string path);
    void allocatePixels(int count, int width, int height);
    unsigned char* getPixels();
    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    int getFirstFrameIndex(string path);
    int countFrames(string path, int startIndex);

  unsigned char* pixels;
  int frameCount;
  int frameWidth;
  int frameHeight;
};
