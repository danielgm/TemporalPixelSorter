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

    void step();
    void sortPixel(int x, int y, int frame0, int frame1);
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
