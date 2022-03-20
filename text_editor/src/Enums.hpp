#pragma once

namespace ted {

enum Status {
    FILE_EXISTS,
    FILE_DOES_NOT_EXIST,
    ERROR_CREATING,
    ERROR_OPENING,
    ERROR_READ,
    ERROR_SAVE,
    ERROR_ACCESS_ANOTHER,
    CREATE_SUCCESS,
    OPEN_SUCCESS,
    READ_SUCCESS,
    SAVE_SUCCESS
};

enum Operation { CREATE, OPEN, SAVE, CLOSE, EXIT, COPY, CUT, PASTE };

} // namespace ted
