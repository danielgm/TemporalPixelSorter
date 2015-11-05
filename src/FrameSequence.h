#pragma once

#include "ofMain.h"

class FrameSequence {

  public:
    FrameSequence();
    ~FrameSequence();
    FrameSequence* clone();

    void update();

    void clearFrames();
    void loadFrames(string path); // alloc
    void saveFrames(string path);
    void allocatePixels(int count, int width, int height);

    unsigned char* getPixels();
    void setFromPixels(unsigned char* pixels, int count, int w, int h); // alloc

    int getFrameCount();
    int getFrameWidth();
    int getFrameHeight();

    ofColor getColor(int frame, int x, int y);
    void setColor(int frame, int x, int y, ofColor c);

    int getCurrFrame();
    void setCurrFrame(int index);
    void nextFrame();
    void prevFrame();
    bool isPlaying();
    void play();
    void stop();
    unsigned char* getCurrFramePixels();

  private:
    int getFirstFrameIndex(string path);
    int countFrames(string path, int startIndex);

    unsigned char* pixels;

    int frameCount;
    int frameWidth;
    int frameHeight;

    int frameRate;
    unsigned long long prevTime;
    int currFrame;
    bool playing;
};
