#include <assert.h>
#include <libgen.h>
#include <stdio.h>
#include <stdlib.h>

#define EMPTY (-1)
#define SIZE 16
struct stack {
   int top;
   double numbers[SIZE];
};


void err_operator (char *err_oper) {
   printf("%s: invalid operator \n", err_oper);
}

void push (struct stack *stack_ptr, double number) {
  if (stack_ptr->top >= SIZE - 1) {
      printf("%.15g :stack overflow\n", number);
   } else {
      stack_ptr->numbers[++stack_ptr->top] = number;
   }
}

void do_binop (struct stack *stack_ptr, char operator) {
      if (stack_ptr->top < 1) {
         printf("%c: stack underflow\n", operator);  // help
      }else {
         double right = stack_ptr->numbers[stack_ptr->top--];
         double left = stack_ptr->numbers[stack_ptr->top--];
         switch (operator) {
            case '+': push (stack_ptr, left + right); break;
            case '-': push (stack_ptr, left - right); break;
            case '*': push (stack_ptr, left * right); break;
            case '/': push (stack_ptr, left / right); break;
         }
      }


}

void do_print (struct stack *stack_ptr) {
      if (stack_ptr->top == EMPTY) {
         printf("stack is empty\n");
      }else {
         for (int pos = 0; pos <= stack_ptr->top; ++pos) {
            printf ("%15g\n", stack_ptr->numbers[pos]);
         }
      }
   }


void do_clear (struct stack *stack_ptr) {
   stack_ptr->top = EMPTY;
}

void do_operator (struct stack *stack_ptr, char *operator) {
      switch (operator[0]) {
         case '+': do_binop (stack_ptr, '+');   break;
         case '-': do_binop (stack_ptr, '-');   break;
         case '*': do_binop (stack_ptr, '*');   break;
         case '/': do_binop (stack_ptr, '/');   break;
         case ';': do_print (stack_ptr);        break;
         case '@': do_clear (stack_ptr);        break;
         default : err_operator (operator); break;
   }

}


int main (int argc, char **argv) {
   if (argc != 1) {
      fprintf (stderr, "Usage: %s\n", basename (argv[0]));
      exit (EXIT_FAILURE);
   }
   struct stack stack;
   stack.top = EMPTY;
   char buffer[1024];
   for (;;) {
      int scanrc = scanf ("%1023s", buffer);
      if (scanrc == EOF) break;
      assert (scanrc == 1);
      if (buffer[0] == '#') {
         scanrc = scanf ("%1023[^\n]", buffer);
         continue;
      }
      char *endptr;
      double number = strtod (buffer, &endptr);
      if (*endptr == '\0') {
         push (&stack, number);
      }else if (buffer[1] != '\0') {
         err_operator (buffer);
      }else {
         do_operator (&stack, buffer);
      }
   }
   return EXIT_SUCCESS;
}
