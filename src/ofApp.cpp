#include "ofApp.h"

void ofApp::setup() {
  screenWidth = ofGetWindowWidth();
  screenHeight = ofGetWindowHeight();

  frameWidth = 0;
  frameHeight = 0;
  frameCount = 0;

  inputDrawImage = ofImage();
  outputDrawImage = ofImage();

  inputFrames = new FrameSequence();
  inputFrames->loadFrames("rickmorty-dimension35c-6-short");

  frameCount = inputFrames->getFrameCount();
  frameWidth = inputFrames->getFrameWidth();
  frameHeight = inputFrames->getFrameHeight();

  outputFrames = inputFrames->clone();
  TemporalPixelSorter::sort(outputFrames);
}

void ofApp::update() {
  inputFrames->update();
  outputFrames->update();
  currFrameChanged();
}

void ofApp::draw() {
  ofBackground(0);

  if (inputFrames != NULL) {
    ofSetColor(255);
    inputDrawImage.draw(
        (screenWidth - frameWidth) / 2,
        (screenHeight - frameHeight - frameHeight) / 2);
  }

  if (outputFrames != NULL) {
    ofSetColor(255);
    outputDrawImage.draw(
        (screenWidth - frameWidth) / 2,
        (screenHeight - frameHeight - frameHeight) / 2 + frameHeight);
  }
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
}

void ofApp::windowResized(int w, int h) {
}

void ofApp::gotMessage(ofMessage msg) {
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
    inputDrawImage.setFromPixels(inputFrames->getCurrFramePixels(), frameWidth, frameHeight, OF_IMAGE_COLOR);
  }

  if (outputFrames != NULL) {
    outputDrawImage.setFromPixels(outputFrames->getCurrFramePixels(), frameWidth, frameHeight, OF_IMAGE_COLOR);
  }
}

