#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

typedef unsigned long long int (*fibFunc)(int);
unsigned long long int memo[100];
fibFunc ptr;

void initializeCache(fibFunc);
unsigned long long int fib_i(int);
unsigned long long int fib_r(int);
unsigned long long int fibWrapper(int);
int isOverflow(unsigned long long, unsigned long long);

void initializeCache(fibFunc chosenFunc) {
   ptr = chosenFunc;
   memo[2] = 1;
}

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
   for (int i = 2; i < seq; i++) {
      if (isOverflow(prev, cur)) {
         return ULLONG_MAX;
      }
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

   unsigned long long int prev = fibWrapper(seq - 1);
   unsigned long long int prevprev = fibWrapper(seq - 2);

   if (isOverflow(prev, prevprev)) {
      return ULLONG_MAX;
   }

   return prev + prevprev;
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

int isOverflow(unsigned long long int prev, unsigned long long int cur) {
   // overflow happens when prev + cur > max_int so we have overflow when cur > max_int - prev
   if (cur > ULLONG_MAX - prev) {
      return 1;
   }
   return 0;
}

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

   if (result == ULLONG_MAX) {
      printf("overflow occured");
   }
   else {
      printf("%llu", result);
   }
   
   return 0;
}
