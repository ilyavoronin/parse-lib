#ifndef FL_2020_SPBU_SPR_SSTREAM_H
#define FL_2020_SPBU_SPR_SSTREAM_H

#include <string>

class SStream {
public:
    SStream(std::string &s_): s(s_), pos(0) {}
    SStream& operator=(const SStream &ot);
    bool get(char &c);
    void unget();
    void setPos(int npos) {
        pos = npos;
    }
    int getPos() {
        return pos;
    }

private:
    const std::string &s;
    int pos;
};


#endif //FL_2020_SPBU_SPR_SSTREAM_H
