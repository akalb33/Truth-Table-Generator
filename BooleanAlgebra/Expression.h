#pragma once

#include <string>
#include <vector>

class Expression
{
public:
   Expression(std::string expression);

   void ResetClass(std::string expression);

   // returns true if success
   bool GenerateTruthTable();

   void RunUnitTests();

private:
   void RemoveWhiteSpace();

   // returns false if prime syntax is incorrect.
   bool FillVariablesVector();
   // replaces all instances of variables with the provided value from values
   void ConvertExpressionToBinary(unsigned int values);
   // All characters acceptable except spaces
   void AdjustForNots();
   // Acceptable Characters: 1, 0, +
   std::string EliminateAnds(std::string expression) const;
   // Acceptable Characters: 1, 0, + 
   char EliminateOrs(std::string expression) const;
   // return true if there were parentheses to replace
   // replaces everything from the first parentheses to the last parentheses
   // including the parentheses with a single 0 or 1
   // Acceptable Characters: (, ), 1, 0, +, '
   bool RemoveInnerMostParentheses();

   char GetBinaryDigit(unsigned int number, unsigned int index);

   std::vector<char> mVariables;
   std::string mExpression;
};