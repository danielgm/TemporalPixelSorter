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
    void saveFrames(string path);
    string getOutputName();

    void doSort();
    ofColor getColor(int x, int y, int frame);
    void setColor(int x, int y, int frame, ofColor c);

    void currFrameChanged();

  int screenWidth;
  int screenHeight;

  int frameWidth;
  int frameHeight;
  int frameCount;

  int frameRate;
  unsigned long long prevTime;
  int currFrame;
  bool isPlaying;

  unsigned char* inputPixels;
  unsigned char* outputPixels;

  ofImage inputDrawImage;
  ofImage outputDrawImage;
  unsigned char* inputDrawPixels;
  unsigned char* outputDrawPixels;
};

bool comparePixel(const ofColor &c0, const ofColor &c1) {
  float v0 = c0.getLightness();
  float v1 = c1.getLightness();
  if (v0 != v1) return v0 < v1;
  if (c0.r != c1.r) return c0.r < c1.r;
  if (c0.g != c1.g) return c0.g < c1.g;
  return c0.b < c1.b;
}
