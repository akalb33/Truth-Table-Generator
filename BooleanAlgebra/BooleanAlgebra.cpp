#include <iostream>
#include "Expression.h"

int main()
{
   std::string input = "";
   std::cout << "This program is a truth table generator for boolean algebra expressions." << std::endl;
   std::cout << "Valid inputs include capital singular letters only, and the acceptable operations." << std::endl;
   std::cout << "Two letters next to each other signifies an AND relationship." << std::endl;
   std::cout << "Two letters separated by a + signifies an OR relationship." << std::endl;
   std::cout << "A ' after a letter signifies that letter is NOTTED." << std::endl;
   std::cout << "Parentheses are valid and everything is evaluated in an infix manner." << std::endl;
   std::cout << "Sample input: (AB)(A'+B+C')" << std::endl;
   std::cout << "NOTE: whitespaced is removed in the evaluation process." << std::endl;
   do
   {
      std::cout << "Enter a boolean expression to be evaluated, or enter \'done\' to exit." << std::endl;
      getline(std::cin, input);
      if (input != "done")
      {
         Expression e(input);
         e.GenerateTruthTable();
      }
   } while (input != "done");
   /*Expression e("(B+D')(B'+D)(B+C')");
   //e.RunUnitTests();
   e.GenerateTruthTable();
   Expression a("A'B' + ABC");
   a.GenerateTruthTable();  */
   return 0;
}