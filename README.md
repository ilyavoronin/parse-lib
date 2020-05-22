What?
-----
Monadic style parser(but not actually monadic)  

Example
-------
```cpp
    auto is_digit = [](char c) {return (bool)std::isdigit(c); };
    auto to_int = [](std::string s) {return std::stoi(s);};

    Parser parseInt = pwhile(is_digit).map<int>(to_int);

    Parser parseSum = Parser<int>::combine([parseInt](auto &ss) {
        auto n1 = parseInt[ss];
        pchar('+')[ss];
        auto n2 = parseInt[ss];
        return n1 + n2;
    });

    Parser parseMult = Parser<int>::combine([parseInt](auto &ss) {
        auto n1 = parseInt[ss];
        pchar('*')[ss];
        auto n2 = parseInt[ss];
        return n1 * n2;
    });

    Parser parseSumOrMult = parseSum | parseMult;

    std::string input1 = "123+456";
    std::string input2 = "8*1023";
    SStream ss1(input1);
    SStream ss2(input2);

    std::cout << parseSumOrMult.runParser(ss1).obj << "\n";
    std::cout << parseSumOrMult.runParser(ss2).obj << "\n";
```