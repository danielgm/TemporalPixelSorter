#pragma once

#include "ofMain.h"

class FileNamer {

  public:
    FileNamer();
    void init(std::string _prefix, std::string _extension);
    std::string curr();
    std::string next();

  private:
    string getFilename(int n);
    int getAvailableFrameIndex();

    std::string prefix;
    std::string extension;
    int currIndex;
};

