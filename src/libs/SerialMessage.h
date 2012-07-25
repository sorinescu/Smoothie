#ifndef SERIALMESSAGE_H
#define SERIALMESSAGE_H
#include "libs/StreamOutput.h"
struct SerialMessage {
        StreamOutput* stream;
        smt_string message;
};
#endif
