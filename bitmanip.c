#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <assert.h>

#define p(x) putHex(#x,&x,size)
#define b(x) putBinary(#x,&x,size)

const unsigned int size = 32;

void putHex(char* label, void *a, unsigned int size)
{
  char format[128];
  unsigned long long int b = *(unsigned long long int*)a;
  sprintf(format, "%s%d%s", "%5s: %0", (size+3)>>2, "lx\n");
  printf(format, label, b);
}

void putBinary(char* label, void *a, unsigned int size)
{
  int i;
  unsigned long int b = *(unsigned long long int*) a;
  unsigned long int mask = 1UL << (size-1);
  //unsigned long int mask = 1 << (size-1);

  printf("%5s: ", label);
  for(i=size; i>0; i--, b<<=1)
    printf("%u", (b&mask)? 1: 0);

  printf("\n");
}


long long int getInteger()
{
  char str[128];
  fgets(str, sizeof(str), stdin);

  char* p = str;
  if(*p == '-' || *p == '+') p++;
  for(; *p && '0' <= *p && *p <= '9'; p++) ;
  if(*p && *p != '\n')
  {
    fprintf(stderr, "incompatible integer\n");
    return 0;
  }

  return atoll(str);
}

double getFloat()
{
  char str[128];
  fgets(str, sizeof(str), stdin);

  char* p = str;
  if(*p == '-' || *p == '+') p++;
  for(; *p && '0' <= *p && *p <= '9'; p++) ;
  if(*p == '.')
  {
    for(p++; *p && '0' <= *p && *p <= '9'; p++) ;

    if(*p && *p != '\n')
    {
      fprintf(stderr, "incompatible float\n");
      return 0;
    }
  }
  else if(*p && *p != '\n')
  {
    fprintf(stderr, "incompatible float\n");
    return 0;
  }

  return atof(str);
}

unsigned long long int getBinary()
{
  char str[128];
  fgets(str, sizeof(str), stdin);

  char* p = str;
  for(; *p && ('0' == *p || '1' == *p); p++) ;
  if(*p && *p != '\n')
  {
    fprintf(stderr, "incompatible binary, %c\n", *p);
    return 0;
  }

  char* q = str;
  unsigned long long int x = 0;
  while(q != p)
  {
    int parity = *q++ == '0'? 0: 1;
    x <<= 1;
    x |= parity;
  }
  
  return x;
}

unsigned long long int
bitmanip(unsigned long long int x, unsigned int w)
{

  struct termios term, term_orig;
  tcgetattr(0, &term_orig);
  term = term_orig;

  term.c_lflag &= ~ICANON;
  term.c_lflag &= ~ECHO;
  term.c_cc[VMIN] = 1;
  term.c_cc[VTIME] = 0;

  tcsetattr(0, TCSANOW, &term);

  assert(0 < w && w <= 64);

  int keep = 1;
  int putmode = 1;
  unsigned long long int man = 0;
  unsigned long long int mbs = 1 << (w-1);
  unsigned long long int mask = (mbs << 1) - 1;
  unsigned long long int temp;

  if(putmode)
  {
    putHex("x", &x, w);
    putHex("man", &man, w);
  }
  else
  {
    putBinary("x", &x, w);
    putBinary("man", &man, w);
  }
  putchar('\n');

  while(keep)
  {
    int c = getchar();
    int ignore = 0;
    switch(c)
    {
    case EOF:
    case 'q':
    case 'Q':
      keep = 0;
      break;

    case 'h':
    case 'H':
      if(man == 0) man = 1;
      else man <<= 1;
      man &= mask;
      break;

    case 'l':
    case 'L':
      if(man == 0) man = mbs;
      else man >>= 1;
      man &= mask;
      break;

    case '\n':
      x &= man;
      break;

    case ' ':
      x |= man;
      break;

    case 'n':
    case 'N':
      tcsetattr(0, TCSANOW, &term_orig);
      printf(c & 0x80? "insert man: ": "insert x: " );
      temp = ((unsigned long long int)getInteger()) & mask;
      if(c & 0x80) man = temp; else x = temp;
      tcsetattr(0, TCSANOW, &term);
      break;

    case 'f':
    case 'F':
      tcsetattr(0, TCSANOW, &term_orig);
      printf(c & 0x80? "insert man: ": "insert x: " );
      temp = ((unsigned long long int)getFloat()) & mask;
      if(c & 0x80) man = temp; else x = temp;
      tcsetattr(0, TCSANOW, &term);
      break;

    case 'b':
    case 'B':
      tcsetattr(0, TCSANOW, &term_orig);
      printf(c & 0x80? "insert man: ": "insert x: " );
      temp = ((unsigned long long int)getBinary()) & mask;
      if(c & 0x80) man = temp; else x = temp;
      tcsetattr(0, TCSANOW, &term);
      break;

    case 'p':
      if(putmode) putmode = 0;
      else putmode = 1;
      printf("--------------------------------\n");
      printf("  putmode changed to %s\n", putmode?"Hex":"Binary");
      printf("--------------------------------\n\n");
      break;

    default: ignore = 1;
      break;
    }

    if(ignore) continue;

    if(putmode)
    {
      putHex("x", &x, w);
      putHex("man", &man, w);
    }
    else
    {
      putBinary("x", &x, w);
      putBinary("man", &man, w);
    }
    putchar('\n');

  } // end of while

  tcsetattr(0, TCSANOW, &term_orig);

  return x;
} // end of function

int main(int argc, char** argv)
{
  if(argc < 3)
  { printf(
      "Usage: bitmanip <preset> <bitwidth>\n"
      "\tyou can manipulate a number by bit\n"
      "\tthe maximum bitwidth depends on your machine\n"
      "\tq, Q, CTRL-D to quit \n"
      "\th, H to left shift manipulator\n"
      "\tl, L to right shift manipulator\n"
      "\tEnter to perform & operation with the number\n"
      "\tSpace to perform | operation with the number\n"
      "\tn/N set the number/manipulator by integer form\n"
      "\tf/F set the number/manipulator by float form\n"
      "\tb/B set the number/manipulator by binary form\n"
      "\tp to toggle display mode between binary and hex\n"
    );
    return 1;
  }

  unsigned long long int x = atoll(argv[1]);
  int w = atoi(argv[2]);

  printf("starting with\n  preset: %lld\n  bitwidth: %d\n", x, w);
  bitmanip(x, w);

  return 0;
}
