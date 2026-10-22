// aufgabe 3
// 22.10.26

/*
Erstellen sie einen ad-hoc-Scanner in C++ wie in der Vorlesung, der folgende
Sprachen erkennt:
a) Die vom regulären Ausdruck 1*0*0 erzeugte Sprache L(1*0*0)
b) Die Sprache der ganzen Dezimalzahlen (Integer) mit Vorzeichen und ohne
führende 0 c) Die Sprache der ganzen Binärzahlen ohne führende 0
*/

#include <fstream>
#include <iostream>

class AdhocScanner {
public:
  int a() {
    std::ifstream f("hello.hsk_a");
    if (!f) {
      std::cerr << "input file does not exist!\n";
      return 1;
    }
    char c;
    f.get(c);
    int ret = 0;
    while (c != 10) {
      switch (c) {
      case '1': {
        while (c == '1')
          f.get(c);

        if (c == '0') {
          while (c == '0')
            f.get(c);
          if (c == 10) {
            ret = 0;
            break;
          }
        }
        [[fallthrough]];
      }
      default: {
        ret = 1;
        break;
      }
      }
      if (ret == 1)
        break;
      f.get(c);
    }

    return ret;
  }

  int b() {
    std::ifstream f("hello.hsk_b");
    if (!f) {
      std::cerr << "input file does not exist!\n";
      return 1;
    }
    char c;
    f.get(c);
    int ret = 0;
    bool zero_allowed = false;
    bool vorz_allowed = true;
    while (c != 10) {
      if (!zero_allowed && c == '0') {
        ret = 1;
        break;
      }
      if (!vorz_allowed && (c == '+' || c == '-')) {
        ret = 1;
        break;
      }
      if (c == '+' || c == '-') {
        vorz_allowed = false;
      } else if (c >= '0' && c <= '9') {
        zero_allowed = true;
      } else {
        ret = 1;
        break;
      }

      f.get(c);
    }

    return ret;
  }

  int c() {
    std::ifstream f("hello.hsk_c");
    if (!f) {
      std::cerr << "input file does not exist!\n";
      return 1;
    }
    char c;
    f.get(c);
    int ret = 0;
    bool zero_allowed = false;
    while (c != 10) {
      if (!zero_allowed && c == '0') {
        ret = 1;
        break;
      }
      if (c >= '0' && c <= '1') {
        zero_allowed = true;
      } else {
        ret = 1;
        break;
      }

      f.get(c);
    }

    return ret;
  }
};

int main() {
  AdhocScanner scanner{};
  int ret{0};

  ret = scanner.a();
  if (ret == 1)
    std::cout << "hello.hsk_a wurde nicht erkannt\n";
  else if (ret == 0)
    std::cout << "hello.hsk_a wurde erkannt\n";

  ret = scanner.b();
  if (ret == 1)
    std::cout << "hello.hsk_b wurde nicht erkannt\n";
  else if (ret == 0)
    std::cout << "hello.hsk_b wurde erkannt\n";

  ret = scanner.c();
  if (ret == 1)
    std::cout << "hello.hsk_c wurde nicht erkannt\n";
  else if (ret == 0)
    std::cout << "hello.hsk_c wurde erkannt\n";
}
