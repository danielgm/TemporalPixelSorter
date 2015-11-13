#include "ofApp.h"

void ofApp::setup() {
  inputDrawImage = ofImage();
  outputDrawImage = ofImage();

  inputFrames = new FrameSequence();
  inputFrames->loadFrames("lolarunsmall", 50);

  outputFrames = inputFrames->clone();

  //pixelBlurrer = new TemporalPixelBlurrer(outputFrames, 5);
  pixelSorter = new TemporalPixelSorter(outputFrames);
  //pixelSorter->sort();
  //pixelSorter->updatePixels();

  inputChart = new ColorBarChart();
  outputChart = new ColorBarChart();

  setBarChart(308, 118);

  hudFont.loadFont("verdana.ttf", 9);
}

void ofApp::update() {
  inputFrames->update();
  outputFrames->update();
  currFrameChanged();
}

void ofApp::draw() {
  ofBackground(0);

  int y = 0;

  int screenWidth = ofGetWindowWidth();
  int screenHeight = ofGetWindowHeight();

  int frameWidth = inputFrames->getFrameWidth();
  int frameHeight = inputFrames->getFrameHeight();

  if (inputFrames != NULL) {
    ofSetColor(255);
    inputDrawImage.draw(0, 0);
  }

  if (outputFrames != NULL) {
    ofSetColor(255);
    outputDrawImage.draw(frameWidth, 0);
  }

  y += frameHeight;

  ofNoFill();
  ofSetColor(255, 255, 0);
  ofCircle(targetX, targetY, 4);
  ofCircle(frameWidth + targetX, targetY, 4);

  int chartHeight = 150;
  inputChart->draw(0, y, frameWidth * 2, chartHeight);
  y += chartHeight;
  outputChart->draw(0, y, frameWidth * 2, chartHeight);
  y += chartHeight;

  stringstream ss;
  ss << "Target: " << targetX << ", " << targetY;
  hudFont.drawString(ss.str(), 12, y + 28);
}

void ofApp::keyPressed(int key) {
}

void ofApp::keyReleased(int key) {
  switch (key) {
    case 'f':
      if (inputFrames->isPlaying()) {
        inputFrames->stop();
        outputFrames->stop();
      }
      else {
        inputFrames->play();
        outputFrames->play();
      }
      break;
    case 'h':
      inputFrames->prevFrame();
      outputFrames->prevFrame();
      currFrameChanged();
      break;
    case 'l':
      inputFrames->nextFrame();
      outputFrames->nextFrame();
      currFrameChanged();
      break;
    case 'p':
      screenshot();
      break;
    case '1':
      step(1);
      break;
    case '2':
      step(2);
      break;
    case '3':
      step(3);
      break;
    case '4':
      step(5);
      break;
    case '5':
      step(10);
      break;
    case '6':
      step(20);
      break;
    case 's':
      string outputName = getOutputName();
      outputFrames->saveFrames(outputName);
      break;
  }
}

void ofApp::mouseMoved(int x, int y ) {
}

void ofApp::mouseDragged(int x, int y, int button) {
}

void ofApp::mousePressed(int x, int y, int button) {
}

void ofApp::mouseReleased(int x, int y, int button) {
  int frameCount = inputFrames->getFrameCount();
  int frameWidth = inputFrames->getFrameWidth();
  int frameHeight = inputFrames->getFrameHeight();

  if (y < frameHeight) {
    int imageX = -1;
    int imageY = mouseY;
    if (x < frameWidth) {
      imageX = x;
    }
    else if (x < frameWidth * 2) {
      imageX = x - frameWidth;
    }

    if (imageX >= 0 && imageX < frameWidth
      && imageY >= 0 && imageY < frameHeight) {
      setBarChart(imageX, imageY);
    }
  }
}

void ofApp::windowResized(int w, int h) {
}

void ofApp::gotMessage(ofMessage msg) {
}

void ofApp::step(int n) {
  for (int i = 0; i < n; i++) {
    pixelSorter->step();
  }
  pixelSorter->updatePixels();
  currFrameChanged();
  setBarChart(targetX, targetY);
}

void ofApp::dragEvent(ofDragInfo dragInfo) {
}

string ofApp::getOutputName() {
  int i = 1;
  while (true) {
    string path = "output/out" + ofToString(i, 0, 4, '0');
    ofDirectory dir(path);
    if (!dir.exists()) {
      return path;
    }
    i++;
  }
}

void ofApp::currFrameChanged() {
  if (inputFrames != NULL) {
    inputDrawImage.setFromPixels(
        inputFrames->getCurrFramePixels(),
        inputFrames->getFrameWidth(),
        inputFrames->getFrameHeight(),
        OF_IMAGE_COLOR);
  }

  if (outputFrames != NULL) {
    outputDrawImage.setFromPixels(
        outputFrames->getCurrFramePixels(),
        outputFrames->getFrameWidth(),
        outputFrames->getFrameHeight(),
        OF_IMAGE_COLOR);
  }
}

void ofApp::setBarChart(int x, int y) {
  ofColor* data;
  int frameCount;

  targetX = x;
  targetY = y;

  data = inputFrames->getColorsByTime(x, y);
  frameCount = inputFrames->getFrameCount();
  inputChart->setData(data, frameCount);

  data = outputFrames->getColorsByTime(x, y);
  frameCount = outputFrames->getFrameCount();
  outputChart->setData(data, frameCount);
}

void ofApp::screenshot() {
  ofImage i;
  i.allocate(ofGetScreenWidth(), ofGetScreenHeight(), OF_IMAGE_COLOR);
  i.grabScreen(0, 0, ofGetScreenWidth(), ofGetScreenHeight());
  i.saveImage("screenie.png");
}

