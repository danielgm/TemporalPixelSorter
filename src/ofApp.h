#pragma once

#include "ofMain.h"
#include "ColorBarChart.h"
#include "FrameSequence.h"
#include "TemporalPixelSorter.h"

class ofApp : public ofBaseApp {

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

  private:
    FrameSequence* loadFrames(string path);
    void saveFrames(string path);
    string getOutputName();

    void currFrameChanged();
    void setBarChart(int x, int y);

    void screenshot();

    FrameSequence* inputFrames;
    FrameSequence* outputFrames;

    ofImage inputDrawImage;
    ofImage outputDrawImage;

    ColorBarChart* inputChart;
    ColorBarChart* outputChart;

    int targetX;
    int targetY;
};

