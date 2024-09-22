#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <limits.h>

// enum vals (success, failure}
enum State {
   SUCCESS = 1,
   FAILURE = 0
};

// Monad
typedef struct { unsigned long long num; enum State state; } ReturnVal;
typedef ReturnVal (*fibFunc)(int);
void initializeCache(fibFunc);
int sumWouldOverflow(unsigned long long, unsigned long long);
ReturnVal fib_i(int);
ReturnVal fib_r(int);
ReturnVal fibWrapper(int);

// global variables
fibFunc ptr;
const int memo_size = 100;
ReturnVal memo[100];

void initializeCache(fibFunc chosenFunc) {
   ptr = chosenFunc;
   for (int i = 0; i < memo_size; i++) {
      memo[i] = (ReturnVal){ 0, SUCCESS };
   }
}

int sumWouldOverflow(unsigned long long int num1, unsigned long long int num2) {
   return num2 > ULLONG_MAX - num1;
}

ReturnVal fib_i(int seq) {
   // base
   if (seq == 1) {
      return (ReturnVal){ 0, SUCCESS } ;
   } else if (seq == 2) {
      return (ReturnVal){ 1, SUCCESS } ;
   }

   unsigned long long int prev = 0;
   unsigned long long int cur = 1;

   // iterate starts from 3rd fib since we already have 1st and 2nd fib
   for (int i = 2; i < seq; i++) {
      if (sumWouldOverflow(prev, cur)) {
         printf("overflow occured at %d\n", seq);
         return (ReturnVal){ 0, FAILURE };
      }
      unsigned long long int nxt = prev + cur;
      prev = cur;
      cur = nxt;
   }

   return (ReturnVal){ cur, SUCCESS };
}

ReturnVal fib_r(int seq) {
   // base case
   if (seq == 1) {
      return (ReturnVal){ 0, SUCCESS } ;
   } else if (seq == 2) {
      return (ReturnVal){ 1, SUCCESS } ;
   }

   ReturnVal prev = fibWrapper(seq - 1);
   if (prev.state == FAILURE) {
      return prev;
   }
   ReturnVal prevprev = fibWrapper(seq - 2);
   if (prevprev.state == FAILURE) {
      return prevprev;
   }

   if (sumWouldOverflow(prev.num, prevprev.num)) {
      printf("overflow occured at %d\n", seq);
      return (ReturnVal){ 0, FAILURE } ;
   }

   return (ReturnVal){ prev.num + prevprev.num, SUCCESS } ;
}

ReturnVal fibWrapper(int seq) {
   // base case
   if (memo[seq].num != 0) {
      return memo[seq];
   }

   // recursive case
   memo[seq] = ptr(seq);
   return memo[seq];
}

int main(int argc, char *argv[]) {
   int seq;
   sscanf(argv[1], "%d", &seq);
   char* method = argv[2];

   ReturnVal result;
   
   if (*method == 'r') {
      initializeCache(fib_r);
   } else if (*method == 'i') {
      initializeCache(fib_i);
   }

   // nth number starts from 1 not 0
   if (seq < 1) {
      printf("fibonacci index should be positive");
   } else {
      result = fibWrapper(seq);

      if (result.state) {
         printf("%llu", result.num);
      }
   }

   return 0;
}

// // enum vals (success, failure}
// enum State {
//    SUCCESS = 1,
//    FAILURE = 0
// };

// // Monad
// typedef struct { unsigned long long num; enum State state; } ReturnVal;
// int sumWouldOverflow(unsigned long long, unsigned long long);
// ReturnVal fib_i(int);
// ReturnVal fib_r(int);

// unsigned long long int fib_i(int seq) {
//    // base
//    if (seq == 1) {
//       return 0;
//    } else if (seq == 2) {
//       return 1;
//    }

//    unsigned long long int prev = 0;
//    unsigned long long int cur = 1;

//    // iterate starts from 3rd fib since we already have 1st and 2nd fib
//    for (int i = 2; i < seq; i++) {
//       if (isOverflow(prev, cur)) {
//          return ULLONG_MAX;
//       }
//       unsigned long long int nxt = prev + cur;
//       prev = cur;
//       cur = nxt;
//    }

//    return cur;
// }

// unsigned long long int fib_r(int seq) {
//    // base case
//    if (seq == 1) {
//       return 0;
//    } else if (seq == 2) {
//       return 1;
//    }

//    unsigned long long int prev = fibWrapper(seq - 1);
//    unsigned long long int prevprev = fibWrapper(seq - 2);

//    if (isOverflow(prev, prevprev)) {
//       return ULLONG_MAX;
//    }

//    return prev + prevprev;
// }




// int main(int argc, char *argv[]) {
//    int seq;
//    sscanf(argv[1], "%d", &seq);
//    char* method = argv[2];

//    ReturnVal result;
   
//    if (*method == 'r') {
//       result = fib_r(seq);
//    } else if (*method == 'i') {
//       result = fib_i(seq);
//    }

//    // nth number starts from 1 not 0
//    if (seq < 1) {
//       printf("fibonacci index should be positive");
//    } else {
//       if (result.state) {
//          printf("%llu", result.num);
//       }
//    }

//    return 0;
// }

