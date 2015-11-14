
#include "FileNamer.h"

FileNamer::FileNamer() {
  prefix = "frame";
  extension = "png";
  currIndex = getAvailableFrameIndex();
}

void FileNamer::init(std::string _prefix, std::string _extension) {
  prefix = _prefix;
  extension = _extension;
  currIndex = getAvailableFrameIndex();
}

std::string FileNamer::curr() {
  return getFilename(currIndex);
}

std::string FileNamer::next() {
  currIndex = getAvailableFrameIndex();
  return getFilename(currIndex);
}

std::string FileNamer::getFilename(int frameIndex) {
  std::string s = prefix + ofToString(frameIndex, 0, 4, '0');
  if (extension == "" || extension == "/") return s + extension;
  return s + "." + extension;
}

int FileNamer::getAvailableFrameIndex() {
  for (int i = 0; i < 1000; i++) {
    ofDirectory dir(getFilename(i));
    if (!dir.exists()) {
      return i;
    }
  }
  return -1;
}

