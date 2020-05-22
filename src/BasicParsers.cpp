#include <BasicParsers.h>

#include <SStream.h>

Parser<std::string> pstring(const std::string &s) {
    return Parser<std::string>([s](SStream &ss) -> Result<std::string> {
        for (char i : s) {
            char c;
            if (!ss.get(c) || c != i) {
                return Result<std::string>("", false);
            }
        }
        return Result(s, true);
    });
}


Parser<char> pchar(char c) {
    return pcheck([c](char t) {return c == t;});
}

Parser<char> pcheck(std::function<bool(char)> f) {
    return Parser<char>([f](auto &ss) {
        char t;
        if (!ss.get(t) || !f(t)) {
            return Result<char>(0, false);
        }
        return Result(t, true);
    });
}

Parser<char> psuc() {
    return Parser<char>([](auto &ss) {
        char t;
        if (!ss.get(t)) {
            return Result<char>((char)0, false);
        }
        return Result<char>(t, true);
    });
}

Parser<char> pfail() {
    return Parser<char>([](auto &ss) {
        return Result<char>(0, false);
    });
}

Parser<std::string> puntil(std::string until_) {
    auto until = std::make_shared<std::string>(until_);
    return Parser<std::string>([until](auto &ss) {
        char t;
        std::vector<char> last;
        while (true) {
            bool eq = false;
            if (last.size() >= until->size()) {
                eq = true;
                for (int i = 0; i < until->size(); i++) {
                    if ((*until)[i] != last[last.size() - until->size() + i]) {
                        eq = false;
                    }
                }
            }
            if (eq) {
                break;
            }
            if (!ss.get(t)) {
                return Result<std::string>("", false);
            }
            last.push_back(t);
        }
        std::string ress;
        for (auto z: last) {ress += z;}
        return Result<std::string>(ress, true);
    });
}

Parser<std::string> pwhile(std::function<bool(char)> f_) {
    auto f = std::make_shared<std::function<bool(char)>>(f_);
    return Parser<std::string>([f](auto &ss) {
        std::string res = "";
        char t;
        while (true) {
            if (!ss.get(t)) {
                break;
            }
            if (!(*f)(t)) {
                ss.unget();
                break;
            }
            res += t;
        }
        return Result(res, true);
    });
}