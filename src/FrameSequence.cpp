#include "FrameSequence.h"

FrameSequence::FrameSequence() {
  pixels = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;

  frameRate = 12;
  prevTime = 0;
  playing = true;
  currFrame = 0;
}

FrameSequence::~FrameSequence() {
  clearFrames();
}

FrameSequence* FrameSequence::clone() {
  FrameSequence* fs = new FrameSequence();
  fs->setFromPixels(pixels, frameCount, frameWidth, frameHeight);
  return fs;
}

void FrameSequence::update() {
  if (playing) {
    unsigned long long now = ofGetElapsedTimeMillis();
    unsigned long delta = now - prevTime;
    if (delta >= 1000.0 / frameRate) {
      currFrame += floor(delta * frameRate / 1000);
      if (currFrame >= frameCount) {
        currFrame = 0;
      }
      prevTime = now - (delta % frameRate);
    }
  }
}

void FrameSequence::clearFrames() {
  if (pixels != NULL) delete[] pixels;

  pixels = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;
}

void FrameSequence::loadFrames(string path) {
  loadFrames(path, -1);
}

void FrameSequence::loadFrames(string path, int maxFrameCount) {
  ofImage image;

  int firstFrameIndex = getFirstFrameIndex(path);
  if (firstFrameIndex < 0) {
    cout << "Couldn't find frame: " << path << "/frame%04d.png" << endl;
    return;
  }

  image.loadImage(path + "/frame" + ofToString(firstFrameIndex, 0, 4, '0') + ".png");

  int totalFrameCount = countFrames(path, firstFrameIndex);
  totalFrameCount = MIN(maxFrameCount, totalFrameCount);

  allocatePixels(totalFrameCount, image.width, image.height);

  cout << "Loading " << frameCount << " frames " << endl
    << "\tPath: " << path << endl
    << "\tDimensions: " << frameWidth << "x" << frameHeight << endl
    << "\tSize: " << floor(frameCount * frameWidth * frameHeight * 3 / 1024 / 1024) << " MB" << endl;

  for (int frameIndex = 0; frameIndex < frameCount; frameIndex++) {
    image.loadImage(path + "/frame" + ofToString(firstFrameIndex + frameIndex, 0, 4, '0') + ".png");
    image.setImageType(OF_IMAGE_COLOR);

    for (int i = 0; i < frameWidth * frameHeight * 3; i++) {
      unsigned char c = image.getPixels()[i];
      pixels[frameIndex * frameWidth * frameHeight * 3 + i] = c;
    }
  }

  cout << "Loading complete." << endl;
}

void FrameSequence::saveFrames(string path) {
  cout << "Saving to " << path << endl;

  ofImage image;

  ofDirectory dir(path);
  if(!dir.exists()){
    dir.create(true);
  }

  for (int i = 0; i < frameCount; i++) {
    image.setFromPixels(pixels + (i * frameWidth * frameHeight * 3), frameWidth, frameHeight, OF_IMAGE_COLOR);
    image.saveImage(path + "/frame" + ofToString(i + 1, 0, 4, '0') + ".bmp");
  }

  cout << "Saving complete." << endl;
}


void FrameSequence::allocatePixels(int count, int width, int height) {
  pixels = new unsigned char[count * width * height * 3];

  frameCount = count;
  frameWidth = width;
  frameHeight = height;
}

unsigned char* FrameSequence::getPixels() {
  return pixels;
}

void FrameSequence::setFromPixels(unsigned char* p, int count, int width, int height) {
  int len = count * width * height * 3;
  pixels = new unsigned char[len];
  memcpy(pixels, p, len);

  frameCount = count;
  frameWidth = width;
  frameHeight = height;
}

int FrameSequence::getFrameCount() {
  return frameCount;
}

int FrameSequence::getFrameWidth() {
  return frameWidth;
}

int FrameSequence::getFrameHeight() {
  return frameHeight;
}

int FrameSequence::getFirstFrameIndex(string path) {
  for (int frameIndex = 0; frameIndex < 1000; frameIndex++) {
    ofDirectory dir(path + "/frame" + ofToString(frameIndex, 0, 4, '0') + ".png");
    if (dir.exists()) {
      return frameIndex;
    }
  }
  return -1;
}

int FrameSequence::countFrames(string path, int startIndex) {
  int n = startIndex;
  ofFile file;
  while (file.doesFileExist(path + "/frame" + ofToString(n, 0, 4, '0') + ".png")) n++;
  return n - startIndex;
}

ofColor FrameSequence::getColor(int frame, int x, int y) {
  ofColor result(
      pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 0],
      pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 1],
      pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 2]);
  return result;
}

void FrameSequence::setColor(int frame, int x, int y, ofColor c) {
  pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 0] = c.r;
  pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 1] = c.g;
  pixels[frame * frameWidth * frameHeight * 3 + y * frameWidth * 3 + x * 3 + 2] = c.b;
}

int FrameSequence::getCurrFrame() {
  return currFrame;
}

void FrameSequence::setCurrFrame(int index) {
  currFrame = index;
}

void FrameSequence::nextFrame() {
  currFrame++;
  if (currFrame >= frameCount) {
    currFrame = 0;
  }
}

void FrameSequence::prevFrame() {
  currFrame--;
  if (currFrame < 0) {
    currFrame = frameCount - 1;
  }
}

bool FrameSequence::isPlaying() {
  return playing;
}

void FrameSequence::play() {
  playing = true;
}

void FrameSequence::stop() {
  playing = false;
}

unsigned char* FrameSequence::getCurrFramePixels() {
  return pixels + currFrame * frameWidth * frameHeight * 3;
}

float* FrameSequence::getLightnessByTime(int x, int y) {
  float* data = new float[frameCount];
  for (int i = 0; i < frameCount; i++) {
    data[i] = ofMap(
        getColor(i, x, y).getLightness(),
        0, 255, 0, 1);
  }
  return data;
}

ofColor* FrameSequence::getColorsByTime(int x, int y) {
  ofColor* data = new ofColor[frameCount];
  for (int i = 0; i < frameCount; i++) {
    data[i] = getColor(i, x, y);
  }
  return data;
}

