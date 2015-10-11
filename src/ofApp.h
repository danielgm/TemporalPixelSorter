#pragma once

#include "ofMain.h"

class ofApp : public ofBaseApp{

  public:
    void setup();
    void update();
    void draw();

    void keyPressed(int key);
    void keyReleased(int key);
    void mouseMoved(int x, int y );
    void mouseDragged(int x, int y, int button);
    void mousePressed(int x, int y, int button);
    void mouseReleased(int x, int y, int button);
    void windowResized(int w, int h);
    void dragEvent(ofDragInfo dragInfo);
    void gotMessage(ofMessage msg);

    void clearFrames();
    int countFrames(string path);
    void loadFrames(string path);

    void step();

  int screenWidth;
  int screenHeight;

  int frameWidth;
  int frameHeight;
  int frameCount;

  int frameRate;
  unsigned long long prevTime;
  int currFrame;

  unsigned char* pixelBuffer;

  ofImage drawImage;
  unsigned char* drawPixels;
};
