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
  bool a(std::istream &in) {
    int c = in.get();

    while (c == '1')
      c = in.get();

    if (c != '0')
      return false;

    while (c == '0')
      c = in.get();

    return c == '\n' || c == EOF;
  }

  bool b(std::istream &in) {
    int c = in.get();

    if (c == '0') {
      c = in.get();

    } else if (c == '+' || c == '-') {
      // +0 -0 erlaubt
      c = in.get();
      if (c == '0')
        c = in.get();
      else {
        while (c >= '0' && c <= '9')
          c = in.get();
      }

    } else if (c >= '1' && c <= '9') {
      c = in.get();
      while (c >= '0' && c <= '9')
        c = in.get();

    } else {
      return false;
    }

    return c == '\n' || c == EOF;
  }

  bool c(std::istream &in) {
    int c = in.get();

    if (c == '0')
      return false;
    while (c == '1' || c == '0')
      c = in.get();

    return c == '\n' || c == EOF;
  }
};

int main() {
  AdhocScanner scanner{};

  /*
  std::ifstream file("hello.hsk_a");
  if (!file) {
      std::cerr << "Datei konnte nicht geöffnet werden\n";
      return 1;
  }
  std::istream& s = file;
  */
  std::cout << "Bitte ein Wort der Sprache 1*0*0 eingeben:\n";
  std::istream &s = std::cin;

  while (s.peek() != EOF) {
    bool ok = scanner.a(s);
    std::cout << (ok ? "akzeptiert" : "abgelehnt") << '\n';

    // Rest der Zeile verwerfen
    if (!ok)
      s.ignore(10000, '\n');
    else
      break;
  }

  std::cout << "Bitte eine ganze Dezimalzahl eingeben:\n";
  while (s.peek() != EOF) {
    bool ok = scanner.b(s);
    std::cout << (ok ? "akzeptiert" : "abgelehnt") << '\n';

    // Rest der Zeile verwerfen
    if (!ok)
      s.ignore(10000, '\n');
    else
      break;
  }

  std::cout << "Bitte eine ganze Binärzahl eingeben:\n";
  while (s.peek() != EOF) {
    bool ok = scanner.b(s);
    std::cout << (ok ? "akzeptiert" : "abgelehnt") << '\n';

    // Rest der Zeile verwerfen
    if (!ok)
      s.ignore(10000, '\n');
    else
      break;
  }
}
