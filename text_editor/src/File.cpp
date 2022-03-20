#include "File.hpp"

namespace ted {

FILE *File::ProcessedFile = NULL;

int File::createFile(const std::string &filename) {
    if (access(filename.c_str(), F_OK) != 0) {
        if (errno == ENOENT) {
            ProcessedFile = fopen(filename.c_str(), "w+");
            if (ProcessedFile == NULL) {
                return ted::ERROR_CREATING;
            } else {
                return ted::CREATE_SUCCESS;
            }
        } else {
            return ted::ERROR_ACCESS_ANOTHER;
        }
    }
    return ted::FILE_EXISTS;
}

int File::openFile(const std::string &filename, const std::string &mode) {
    if (access(filename.c_str(), F_OK) == 0) {
        ProcessedFile = fopen(filename.c_str(), mode.c_str());
        if (ProcessedFile == NULL) {
            return ted::ERROR_OPENING;
        } else {
            return ted::OPEN_SUCCESS;
        }
    }
    return FILE_DOES_NOT_EXIST;
}

int File::readFile(std::string &text) {
    fseek(ProcessedFile, 0, SEEK_END);
    size_t size = ftell(ProcessedFile);
    fseek(ProcessedFile, 0, SEEK_SET);
    char *tempText = (char *)malloc(size * sizeof(char));
    size_t readCount = fread(tempText, sizeof(char), size, ProcessedFile);
    if (readCount == size) {
        text = tempText;
        free(tempText);
        return READ_SUCCESS;
    }
    return ERROR_READ;
}

int File::saveFile(const std::string &filename, const std::string &text) {
    int status = openFile(filename, "w+");
    if (status != OPEN_SUCCESS) {
        return ERROR_SAVE;
    }
    size_t writeCount =
        fwrite(text.c_str(), sizeof(char), text.length(), ProcessedFile);
    if (writeCount == text.length()) {
        fflush(ProcessedFile);
        return SAVE_SUCCESS;
    }
    return ERROR_SAVE;
}

void File::closeFile() {
    if (ProcessedFile != NULL) {
        fclose(ProcessedFile);
        ProcessedFile = NULL;
    }
}

} // namespace ted
