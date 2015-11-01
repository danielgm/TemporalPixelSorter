#include "FrameSequence.h"

FrameSequence::FrameSequence() {
  pixels = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;
}

void FrameSequence::clearFrames() {
  if (pixels != NULL) delete[] pixels;

  pixels = NULL;

  frameCount = 0;
  frameWidth = 0;
  frameHeight = 0;
}

void FrameSequence::loadFrames(string path) {
  ofImage image;

  clearFrames();

  int firstFrameIndex = getFirstFrameIndex(path);
  if (firstFrameIndex < 0) {
    cout << "Couldn't find frame: " << path << "/frame%04d.png" << endl;
    return;
  }

  image.loadImage(path + "/frame" + ofToString(firstFrameIndex, 0, 4, '0') + ".png");

  allocatePixels(countFrames(path, firstFrameIndex), image.width, image.height);

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

void FrameSequence::allocatePixels(int count, int width, int height) {
  pixels = new unsigned char[count * width * height * 3];

  frameCount = count;
  frameWidth = width;
  frameHeight = height;
}

unsigned char* FrameSequence::getPixels() {
  return pixels;
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

