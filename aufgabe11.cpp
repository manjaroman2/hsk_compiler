#include <iostream>

char token;
void check(char e_token);

/* Grammatik
S: ABC
A: aB
A: b
B: bb
B: C
C: c

L = (abb|ac|b)(bb|c)c
  = abbbbc|abbcc|acbbc|accc|bbbc|bcc
*/
void S();
void A();
void B();
void C();

void S()
{
  if((token == 'a') || (token == 'b'))
  {
    std::cout << "Regel S->ABC angewandt\n";
    A();
    B();
    C();
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
    std::cout << "Regel A->aB angewandt\n";
    check('a');
    B();
  }
  else if(token == 'b')
  {
    std::cout << "Regel A->b angewandt\n";
    check('b');
  }
  else
  {
    std::cout << "A() Syntaxfehler: a oder b erwartet\n";
  }
}

void B()
{
  if(token == 'b')
  {
    std::cout << "Regel B->bb angewandt\n";
    check('b');
    check('b');
  }
  else if(token == 'c')
  {
    std::cout << "Regel B->C angewandt\n";
    C();
  }
  else
  {
    std::cout << "B() Syntaxfehler: b oder c erwartet\n";
  }
}

void C()
{
  if(token == 'c')
  {
    std::cout << "Regel C->c angewandt\n";
    check('c');
  }
  else
  {
    std::cout << "C() Syntaxfehler: c erwartet\n";
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
