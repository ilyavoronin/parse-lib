#include "SStream.h"

bool SStream::get(char &c) {
    if (pos < s.size()) {
        c = s[pos++];
        return true;
    }
    else {
        return false;
    }
}

void SStream::unget() {
    if (pos > 0) {
        pos--;
    }
}

SStream& SStream::operator=(const SStream &ot) {
    pos = ot.pos;
    return *this;
}