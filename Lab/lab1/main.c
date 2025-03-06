#include <stdio.h>

int main() {

  printf("Introdu 2 numere:\n");
  int a, b;

  if (scanf("%d %d", &a, &b) != 2) {
    printf("Nu ai introdus 2 numere\n");
    return 1;
  }

  printf("Suma celor 2 numere este: %d\n", a + b);
  return 0;
}
