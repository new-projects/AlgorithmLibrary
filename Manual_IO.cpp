#include <cstdio>
using namespace std;

int ReadInt() {
  int ch = 0;
  unsigned int v = 0;
  bool sign = false;

  while ((ch = getchar()) && !((ch >= '0' && ch <= '9') || ch == '-')) {
  }
  ch == '-' ? sign = true : v = ch - '0';
  while ((ch = getchar()) && (ch >= '0' && ch <= '9')) {
    v = v * 10 + ch - '0';
  }
  return sign ? -v : v;
}

void WriteInt(int value) {
  if (value == 0) {
    putchar('0');
  } else {
    static char s[30];
    unsigned int len = 0, v = (value < 0 ? -value : value);

    while (v) {
      s[len++] = v % 10 + '0';
      v /= 10;
    }
    if (value < 0) {
      putchar('-');
    }
    for (int i = len - 1; i >= 0; --i) {
      putchar(s[i]);
    }
  }
  putchar('\n');
}

int main() {
  int n = ReadInt();
  WriteInt(n);
  for (int i = 0; i < n; ++i) {
    int v = ReadInt();
    WriteInt(v);
  }
  return 0;
}
