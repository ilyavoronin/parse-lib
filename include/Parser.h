#ifndef FL_2020_SPBU_SPR_PARSER_H
#define FL_2020_SPBU_SPR_PARSER_H

#include <sstream>
#include <functional>
#include <queue>
#include <memory>

#include "SStream.h"

class ParsingFailedException: std::exception {};

template <typename T>
class Result {
public:
    T obj;
    bool success = true;
    Result(T obj, bool success = true): obj(obj), success(success) {}
};

template <typename T>
class Parser {
public:

    explicit Parser(std::function<Result<T>(SStream &)> parse): parse(parse) {}

    Result<T> runParser(SStream &s) const;

    template <typename L>
    friend Parser<L> operator|(const Parser<L> &p1, const Parser<L> &p2);

    static Parser oneOf(std::vector <Parser> parsers);

    static Parser<std::vector<T>> many(const Parser &p);

    static Parser<std::vector<T>> many1(const Parser &p);

    T operator[](SStream &s) const;

    static Parser<T> combine(std::function<T(SStream&)> f);

    template <typename L>
    Parser<L> map(std::function<L(T)> f) const;

private:
    std::function<Result<T>(SStream &s)> parse;

    static Parser oneOf(std::queue <Parser> &parsers);
};

template <typename T>
Result<T> Parser<T>::runParser(SStream &s) const {
    SStream tmp = s;
    Result res = this->parse(tmp);
    if (res.success) {
        s = tmp;
    }
    return res;
}
template <typename T>
T Parser<T>::operator[](SStream &s) const {
    auto res = this->runParser(s);
    if (!res.success) {
        throw ParsingFailedException();
    }
    return res.obj;
}


template <typename T>
Parser<T> Parser<T>::combine(std::function<T(SStream&)> f) {
    return Parser([f](SStream &s) -> Result<T> {
        try {
            T res = f(s);
            return Result(res, true);
        } catch (ParsingFailedException) {
            return Result(T{}, false);
        }
    });
}

template <typename T>
template <typename L>
Parser<L> Parser<T>::map(std::function<L(T)> f) const {
    return Parser<L>([f, pparse = this->parse](auto &ss) {
        auto res = pparse(ss);
        if (res.success) {
            return Result(f(res.obj), true);
        }
        else {
            return Result(L{}, false);
        }
    });
}

template <typename T>
Parser<T> operator|(const Parser<T> &p1, const Parser<T> &p2) {
    auto f1 = p1.parse;
    auto f2 = p2.parse;
    return Parser<T>([f1, f2](SStream &s) -> Result<T> {
        Result res1 = Parser<T>(f1).runParser(s);
        if (res1.success) {
            return res1;
        }
        return Parser<T>(f2).runParser(s);
    });
}
template <typename T>
Parser<T> Parser<T>::oneOf(std::vector <Parser> parsers) {
    std::queue <Parser> qparsers;
    for (auto &z: parsers) {
        qparsers.push(z);
    }
    return oneOf(qparsers);
}

template <typename T>
Parser<T> Parser<T>::oneOf(std::queue <Parser> &parsers) {
    if (parsers.size() == 1) {
        return parsers.front();
    }
    Parser firstParser = parsers.front();
    parsers.pop();
    return firstParser | oneOf(parsers);
}

template <typename T>
Parser<std::vector<T>> Parser<T>::many(const Parser<T> &p) {
    return Parser<std::vector<T>>::combine([p](auto &ss) {
        std::vector <T> resv;
        while (true) {
            Result<T> res = p.runParser(ss);
            if (!res.success) {
                break;
            }
            resv.push_back(res.obj);
        }
        return resv;
    });
}

template <typename T>
Parser<std::vector<T>> Parser<T>::many1(const Parser<T> &p) {
    return Parser<std::vector<T>>::combine([p](auto &ss) {
        std::vector <T> resv;
        while (true) {
            Result<T> res = p.runParser(ss);
            if (!res.success) {
                break;
            }
            resv.push_back(res.obj);
        }
        if (resv.size() == 0) {
            throw ParsingFailedException();
        }
        return resv;
    });
}

#endif //FL_2020_SPBU_SPR_PARSER_H
