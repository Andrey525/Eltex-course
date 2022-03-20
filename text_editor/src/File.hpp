#include "Enums.hpp"
#include <iostream>
#include <stdio.h> // fopen, fclose, fread, fwrite
#include <string>
#include <unistd.h> // access

#pragma once

namespace ted {
class File {
  private:
    File() = default;
    ~File() = default;

  public:
    static FILE *ProcessedFile;

    static int createFile(const std::string &filename);
    static int openFile(const std::string &filename, const std::string &mode);
    static int readFile(std::string &text);
    static int saveFile(const std::string &filename, const std::string &text);
    static void closeFile();
};
} // namespace ted
