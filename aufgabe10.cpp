#include <iostream>

char token;
void check(char e_token);

/* Grammatik
S: AB
A: aaA
A: B
B: b

L = { (aa)^n bb für n >= 0 }
*/
void S();
void A();
void B();

void S()
{
  if((token == 'a') || (token == 'b'))
  {
    std::cout << "Regel S->AB angewandt\n";
    A();
    B();
  }
  else
  {
    std::cout << "S() Syntaxfehler: a oder b erwartet\n";
  }
}

void A()
{
  if(token == 'a')
  {
    std::cout << "Regel A->aaA angewandt\n";
    check('a');
    check('a');
    A();
  }
  else if(token == 'b')
  {
    std::cout << "Regel A->B angewandt\n";
    B();
  }
  else
  {
    std::cout << "A() Syntaxfehler: a oder b erwartet\n";
  }
}

void B() {
  if (token == 'b')
  {
    std::cout << "Regel B->b angewandt\n";
    check('b');
  } else
  {
    std::cout << "B() Syntaxfehler: b erwartet\n";
  }
}

char scanner()
{
  return static_cast<char>(getchar());
}

void check(char e_token)
{
  // std::cout << "Check: " << e_token << "\n";
  if(token == e_token)
  {
    token = scanner();
  }
  else
  {
    std::cout << "Syntaxfehler: " << e_token << " erwartet\n";
    while(token != '\n')
      token = scanner();  // Lesen bis Eingabeende
  }
}

void parser()
{
  token = scanner();
  S();
  while(token != '\n')
  {
    std::cout << "Syntaxfehler am Eingabeende\n";
    token = scanner();
  }
}

int main()
{
  while(feof(stdin) == 0)
    parser();
  return 0;
}
