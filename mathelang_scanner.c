#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>

#define u8 uint8_t

#define ERR_NONE 0x00
#define ERR_UNK 0x01
#define ERR_MEM 0x10
#define ERR_SC_INIT 0x20
#define ERR_SC_EOF 0x21
#define ERR_SC_FSEEK 0x22
#define ERR_SC_FREAD 0x23
#define ERR_SC_INPUT 0x24
static u8 err_code = ERR_NONE;

#define ASCII_LF 0xA

const char* CharToAscii(int* c)
{
  switch(*c)
  {
    case ASCII_LF: {
      return "LF";
    }
    default: {
      return (char*)c;
    }
  }
}

typedef enum
{
  ST_Q0,
  ST_Q1,
  ST_Q2,
} State;

const char* State_name(State st)
{
  switch(st)
  {
    case ST_Q0: {
      return "q0";
    }
    case ST_Q1: {
      return "q1";
    }
    case ST_Q2: {
      return "q2";
    }
    default: {
      return "q?";
    }
  }
}

typedef enum
{
  TK_ZAHL,
  TK_OPERATOR,
} TokenType;

const char* TokenType_name(TokenType tt)
{
  switch(tt)
  {
    case TK_ZAHL: {
      return "TK_ZAHL";
    }
    case TK_OPERATOR: {
      return "TK_OPERATOR";
    }
    default: {
      return "TK_UNK";
    }
  }
}

typedef struct
{
  TokenType type;
  char*     value;
} Token;

typedef struct
{
  Token* ptr;
  size_t iterator;
  size_t size;
  size_t capacity;
} TokenArray;

u8 TokenArray_init(TokenArray* a, size_t capacity)
{
  a->capacity = capacity;
  a->iterator = 0;
  a->size     = 0;
  a->ptr      = malloc(sizeof(Token) * capacity);
  if(a->ptr == NULL)
    return ERR_MEM;
  return ERR_NONE;
}

void TokenArray_free(TokenArray* a)
{
  for(size_t i = 0; i < a->size; i++)
    if(a->ptr[i].value != NULL)
      free(a->ptr[i].value);
  free(a->ptr);
}

u8 TokenArray_append(TokenArray* a, TokenType type, FILE* value_stream, size_t value_length)
{
  while(a->size >= a->capacity)
  {
    a->capacity   = a->capacity * 2 + 1;
    Token* newPtr = realloc(a->ptr, sizeof(Token) * a->capacity);
    if(newPtr == NULL)
    {
      free(a->ptr);
      return ERR_MEM;
    }
    a->ptr = newPtr;
  }
  Token* t = &a->ptr[a->size++];
  t->type  = type;
  t->value = malloc(value_length);
  if(fread(t->value, 1, value_length, value_stream) < value_length)
    return ERR_UNK;
  return ERR_NONE;
}

typedef struct
{
  const char* fname;

  FILE* f;
  char  fbuf[40];

  // int        c;
  State      state;
  TokenArray tokens;
  // long       tokenStartPos;

} Scanner;

u8 Scanner_init(Scanner* sc)
{
  if((sc->f = fopen(sc->fname, "r")) == NULL)
  {
    fprintf(stderr, "file '%s' does not exist.", sc->fname);
    return ERR_SC_INIT;
  }
  if(setvbuf(sc->f, sc->fbuf, _IOFBF, 40) != 0)
  {
    fprintf(stderr, "file '%s' does not want our stack buffer.", sc->fname);
    return ERR_SC_INIT;
  }

  sc->state = ST_Q0;

  TokenArray_init(&sc->tokens, 4);
  return ERR_NONE;
}

u8 Scanner_populate(Scanner* sc)
{
  // need more tokens
  long tokenStartPos = 0;
  int  c             = -1;

  while(1)
  {
    c = fgetc(sc->f);
    printf("[%s]  c=0x%X (%s)\n", State_name(sc->state), c, CharToAscii(&c));
    if (feof(sc->f) != 0)
      return ERR_SC_EOF;
    switch(sc->state)
    {
      case ST_Q0: {
        if(c == '0')
        {
          tokenStartPos = ftell(sc->f) - 1;
          sc->state     = ST_Q2;
          break;
        }
        if(c >= '1' && c <= '9')
        {
          tokenStartPos = ftell(sc->f) - 1;
          sc->state     = ST_Q1;
          break;
        }
        return ERR_SC_INPUT;
      }
      case ST_Q1: {
        if(c >= '0' && c <= '9')
        {
          sc->state = ST_Q1;
          break;
        }
        if(c == '+' || c == '-' || c == '*' || c == '/')
        {
          long newTokenStartPos = ftell(sc->f) - 1;
          long tokenLength      = newTokenStartPos - tokenStartPos;
          if(fseek(sc->f, tokenStartPos, SEEK_SET) != 0)
          {
            return ERR_SC_FSEEK;
          }
          tokenStartPos = newTokenStartPos;
          TokenArray_append(&sc->tokens, TK_ZAHL, sc->f, tokenLength);
          TokenArray_append(&sc->tokens, TK_OPERATOR, sc->f, 1);

          sc->state = ST_Q0;
          break;
        }
        if(c == ASCII_LF)
        {
          long newTokenStartPos = ftell(sc->f) - 1;
          long tokenLength      = newTokenStartPos - tokenStartPos;
          if(fseek(sc->f, tokenStartPos, SEEK_SET) != 0)
          {
            return ERR_SC_FSEEK;
          }
          tokenStartPos = newTokenStartPos;
          TokenArray_append(&sc->tokens, TK_ZAHL, sc->f, tokenLength);

          sc->state = ST_Q0;
          break;
        }
        return ERR_SC_INPUT;
      }
      case ST_Q2: {
        if(c == '+' || c == '-' || c == '*' || c == '/')
        {
          long newTokenStartPos = ftell(sc->f) - 1;
          long tokenLength      = newTokenStartPos - tokenStartPos;
          if(fseek(sc->f, tokenStartPos, SEEK_SET) != 0)
          {
            return ERR_SC_FSEEK;
          }
          tokenStartPos = newTokenStartPos;
          TokenArray_append(&sc->tokens, TK_ZAHL, sc->f, tokenLength);
          TokenArray_append(&sc->tokens, TK_OPERATOR, sc->f, 1);

          sc->state = ST_Q0;
          break;
        }
        if(c == ASCII_LF)
        {
          long newTokenStartPos = ftell(sc->f) - 1;
          long tokenLength      = newTokenStartPos - tokenStartPos;
          if(fseek(sc->f, tokenStartPos, SEEK_SET) != 0)
          {
            return ERR_SC_FSEEK;
          }
          tokenStartPos = newTokenStartPos;
          TokenArray_append(&sc->tokens, TK_ZAHL, sc->f, tokenLength);

          sc->state = ST_Q0;
          break;
        }
        return ERR_SC_INPUT;
      }
      default: {
        printf("bad state\n");
        return ERR_UNK;
      }
    }
  }
}


Token* Scanner_next_token(Scanner* sc)
{
  if(sc->tokens.iterator < sc->tokens.size)
    return &(sc->tokens.ptr[sc->tokens.iterator++]);


  if(sc->tokens.iterator > 0)
  {
    sc->tokens.size     = 0;
    sc->tokens.iterator = 0;
  }

  if((err_code = Scanner_populate(sc)) != ERR_NONE)
  {
    printf("Scanner_populate error: 0x%X\n", err_code);
    return NULL;
  }

  if(sc->tokens.iterator < sc->tokens.size)
    return &(sc->tokens.ptr[sc->tokens.iterator++]);
}


void Scanner_free(Scanner* sc)
{
  TokenArray_free(&sc->tokens);
}

int main()
{
  Scanner sc = {.fname = "hello.mathelang"};
  Scanner_init(&sc);

  printf("Tokens:\n");
  while(1)
  {
    Token* token = Scanner_next_token(&sc);
    if(token == NULL)
      return 1;
    printf("%s(%s)\n", TokenType_name(token->type), token->value);
  }


  return 0;
}
