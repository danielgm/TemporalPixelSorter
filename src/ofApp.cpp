#include "ofApp.h"

void ofApp::setup() {
  screenWidth = ofGetWindowWidth();
  screenHeight = ofGetWindowHeight();

  frameWidth = 0;
  frameHeight = 0;
  frameCount = 0;

  frameRate = 10;
  prevTime = ofGetElapsedTimeMillis();
  currFrame = 0;

  pixelBuffer = NULL;

  loadFrames("bmosmall");
}

void ofApp::update() {
  unsigned long long now = ofGetElapsedTimeMillis();
  unsigned long delta = now - prevTime;
  if (delta >= frameRate) {
    currFrame += delta / frameRate;
    if (currFrame >= frameCount) {
      currFrame = 0;
    }
    prevTime = now;

    memcpy(drawPixels, pixelBuffer + (currFrame * frameWidth * frameHeight * 3), frameWidth * frameHeight * 3);
    drawImage.setFromPixels(drawPixels, frameWidth, frameHeight, OF_IMAGE_COLOR);
  }
}

void ofApp::draw() {
  ofBackground(0);

  if (pixelBuffer != NULL) {
    ofSetColor(255);
    drawImage.draw(
        (screenWidth - frameWidth) / 2,
        (screenHeight - frameHeight) / 2);
  }
}

void ofApp::keyPressed(int key) {
}

void ofApp::keyReleased(int key) {
  switch (key) {
    case ' ':
      step();
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

void ofApp::clearFrames() {
  if (pixelBuffer != NULL) delete[] pixelBuffer;

  pixelBuffer = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;
}

int ofApp::countFrames(string path) {
  int n = 0;
  ofFile file;
  while (file.doesFileExist(path + "/frame" + ofToString(n + 1, 0, 4, '0') + ".png")) n++;
  return n;
}

void ofApp::loadFrames(string path) {
  ofImage image;

  clearFrames();

  image.loadImage(path + "/frame0001.png");
  frameWidth = image.width;
  frameHeight = image.height;
  frameCount = countFrames(path);

  cout << "Loading " << frameCount << " frames " << endl
    << "\tPath: " << path << endl
    << "\tDimensions: " << frameWidth << "x" << frameHeight << endl
    << "\tSize: " << floor(frameCount * frameWidth * frameHeight * 3 / 1024 / 1024) << " MB" << endl;

  pixelBuffer = new unsigned char[frameCount * frameWidth * frameHeight * 3];

  for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    image.loadImage(path + "/frame" + ofToString(frameIndex + 1, 0, 4, '0') + ".png");
    image.setImageType(OF_IMAGE_COLOR);

    for (int i = 0; i < frameWidth * frameHeight * 3; i++) {
      pixelBuffer[frameIndex * frameWidth * frameHeight * 3 + i] = image.getPixels()[i];
    }
  }

  drawPixels = new unsigned char[frameWidth * frameHeight * 3];

  drawImage.setFromPixels(pixelBuffer, frameWidth, frameHeight, OF_IMAGE_COLOR);

  cout << "Loading complete." << endl;
}

void ofApp::step() {
}
