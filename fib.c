#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

typedef unsigned long long int (*ProviderPtr)(int);
unsigned long long int memo[100];
ProviderPtr ptr;

unsigned long long int fib_i(int);
unsigned long long int fib_r(int);
unsigned long long int fibWrapper(int);
void initializeCache(ProviderPtr);

unsigned long long int fib_i(int seq) {
   // base
   if (seq == 1) {
      return 0;
   } else if (seq == 2) {
      return 1;
   }

   unsigned long long int prev = 0;
   unsigned long long int cur = 1;

   // iterate starts from 3rd fib since we already have 1st and 2nd fib
   for (int i = 0; i < seq - 2; i++) {
      unsigned long long int nxt = prev + cur;
      prev = cur;
      cur = nxt;
   }

   return cur;
}

unsigned long long int fib_r(int seq) {
   // base case
   if (seq == 1) {
      return 0;
   } else if (seq == 2) {
      return 1;
   }

   unsigned long long int cur = fibWrapper(seq - 1) + fibWrapper(seq - 2);

   return cur;
}

void initializeCache(ProviderPtr fib) {
   ptr = fib;
   // memo[1] = 0;
   memo[2] = 1;
}

unsigned long long int fibWrapper(int seq) {
   // base case
   if (memo[seq] != 0) {
      return memo[seq];
   }

   // recursive case
   memo[seq] = ptr(seq);
   return memo[seq];
}

// int isOverflow(unsigned long long int prev, unsigned long long int cur) {
//    // overflow happens when prev + cur > max_int so we have overflow when cur > max_int - prev
//    if (cur > ULLONG_MAX - prev) {
//       return 1;
//    }
//    return 0;
// }

int main(int argc, char *argv[]) {
   int seq;
   sscanf(argv[1], "%d", &seq);
   char* method = argv[2];

   unsigned long long int result;
   
   // nth number starts from 1 not 0
   if (*method == 'r') {
      initializeCache(fib_r);
   } else if (*method == 'i') {
      initializeCache(fib_i);
   }

   result = fibWrapper(seq);

   printf("%llu", result);

   return 0;
}
