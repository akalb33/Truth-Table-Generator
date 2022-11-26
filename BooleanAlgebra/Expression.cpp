#include "Expression.h"  
#include "assert.h"
#include <algorithm>
#include <cmath>
#include <iostream>

Expression::Expression(std::string expression)
{
   ResetClass(expression);
}

void Expression::ResetClass(std::string expression)
{
   mExpression = expression;
   mVariables.clear();
}

bool Expression::GenerateTruthTable()
{
   RemoveWhiteSpace();
   bool pass = FillVariablesVector();
   // loop through all cases
   if (pass)
   {
      for (int i = 0; i < mVariables.size(); i++)
      {
         std::cout << mVariables.at(i) << " | ";
      }
      std::cout << "output\n";
      std::string originalExpression = mExpression;
      for (int i = 0; i < pow(2, mVariables.size()); i++)
      {
         mExpression = originalExpression;
         ConvertExpressionToBinary(i);
         do
         {
            AdjustForNots();
         } while (RemoveInnerMostParentheses());
         mExpression = EliminateAnds(mExpression);
         mExpression = EliminateOrs(mExpression);
         for (int j = 0; j < mVariables.size(); j++)
         {
            std::cout << GetBinaryDigit(i, j) << " | ";
         }
         std::cout << mExpression << std::endl;
      }
   }
   return pass;
}


void Expression::RunUnitTests()
{
   ResetClass("   A 1  + C dasf }  ");
   RemoveWhiteSpace();
   assert(mExpression == "A1+Cdasf}");

   
   ResetClass("BA' + BC + A(CD')");
   RemoveWhiteSpace();
   assert(FillVariablesVector());
   assert(mVariables.size() == 4);
   assert(mVariables[0] == 'A');
   assert(mVariables[1] == 'B');
   assert(mVariables[2] == 'C');
   assert(mVariables[3] == 'D');

   ResetClass("BA'' + BC + A(CD')");
   RemoveWhiteSpace();
   assert(!FillVariablesVector());

   ResetClass("BA' + BC. + A(CD')");
   RemoveWhiteSpace();
   assert(!FillVariablesVector());

   ResetClass("A+B");
   RemoveWhiteSpace();
   FillVariablesVector();
   ConvertExpressionToBinary(0b10);
   assert(mExpression == "1+0");

   ResetClass("A'+B");
   RemoveWhiteSpace();
   FillVariablesVector();
   ConvertExpressionToBinary(0b10);
   assert(mExpression == "1'+0");

   ResetClass("((A')+B)");
   RemoveWhiteSpace();
   FillVariablesVector();
   ConvertExpressionToBinary(0b10);
   assert(mExpression == "((1')+0)");

   ResetClass("((A')+B)");
   AdjustForNots();
   assert(mExpression == "((A')+B)");

   ResetClass("1'0'+0'+0");
   AdjustForNots();
   assert(mExpression == "01+1+0");

   ResetClass("");
   assert(EliminateAnds("101+0+10") == "0+0+0");

   ResetClass("");
   assert(EliminateAnds("000+0+11") == "0+0+1");

   ResetClass("");
   assert(EliminateOrs("0+0+1") == '1');

   ResetClass("");
   assert(EliminateOrs("0+0+0") == '0');

   ResetClass("((1+0)'+1)");
   RemoveInnerMostParentheses();
   assert(mExpression == "(1'+1)");
}

void Expression::RemoveWhiteSpace()
{
   std::string spacesRemoved;
   for (unsigned int i = 0; i < mExpression.length(); i++)
   {
      char c = mExpression.at(i);
      if (c != ' ')
      {
         spacesRemoved += c;
      }
   }
   mExpression = spacesRemoved;
}

bool Expression::FillVariablesVector()
{
   bool properSyntax = true;
   for (unsigned int i = 0; properSyntax && i < mExpression.length(); i++)
   {
      char c = mExpression.at(i);
      if (c >= 'A' && c <= 'Z')
      {     
         bool inserted = false;
         std::vector<char>::iterator indexToInsert;
         for (indexToInsert = mVariables.begin(); !inserted && indexToInsert != mVariables.end(); indexToInsert++)
         {
            if (*indexToInsert == c)
            {
               inserted = true;
            }
         }
         if (!inserted)
         {
            mVariables.push_back(c);
         }
      }
      else if (c == '+')
      {
         if (i == mExpression.length() - 1 || i == 0 || (mExpression.at(i + 1) == '+'  || mExpression.at(i + 1) == '\''))
         {
            properSyntax = false;
         }
      }
      else if (c == '\'')
      {
         if (i == 0 || (i != mExpression.length() - 1 && mExpression.at(i + 1) == '\''))
         {
            properSyntax = false;
         }
      }     
      else if(c != '(' && c != ')')
      {
         properSyntax = false;
      }
   }
   if (properSyntax)
   {
      std::sort(mVariables.begin(), mVariables.end());
   }
   return properSyntax;
}

void Expression::ConvertExpressionToBinary(unsigned int values)
{
   for (unsigned int i = 0; i < mExpression.length(); i++)
   {
      char c = mExpression.at(i);
      std::vector<char>::iterator it = std::find(mVariables.begin(), mVariables.end(), c);
      if (it != mVariables.end())
      {
         unsigned int variableIndex = it - mVariables.begin();
         unsigned int value = values;
         
         /*for (unsigned int o = 0; o < mVariables.size() - variableIndex - 1; o++)
         {
            value /= 2;
         }
         if ((value & 1) == 1)
         {
            mExpression.at(i) = '1';
         }
         else
         {
            mExpression.at(i) = '0';
         }                     */
         mExpression.at(i) = GetBinaryDigit(value, variableIndex);
      }
   }
}

void Expression::AdjustForNots()
{
   for (int i = mExpression.length() - 1; i >= 0; i--)
   {
      char c = mExpression.at(i);
      if (c == '\'' && (mExpression.at(i - 1) == '0' || mExpression.at(i - 1) == '1'))
      {
         mExpression.at(i - 1) = '1' - mExpression.at(i - 1) + '0';
         mExpression = mExpression.substr(0, i) + mExpression.substr(i + 1);
      }
   }
}

std::string Expression::EliminateAnds(std::string expression) const
{
   std::string andsRemoved = expression;
   int index = andsRemoved.find('+');
   int first = 0;
   while (index != -1)
   {
      bool done = false;
      for (int i = first; !done && i < index; i++)
      {
         if (andsRemoved.at(i) == '0')
         {
            done = true;
            andsRemoved = andsRemoved.substr(0, first) + '0' + andsRemoved.substr(index);
         }
      }                                                                  
      if (!done)
      {
         andsRemoved = andsRemoved.substr(0, first) + '1' + andsRemoved.substr(index);
      }
      first += 2;
      index = andsRemoved.find('+', first);
   }                  

   bool done = false;
   for (int i = first; !done && i < andsRemoved.length(); i++)
   {
      if (andsRemoved.at(i) == '0')
      {
         done = true;
         andsRemoved = andsRemoved.substr(0, first) + '0';
      }
   }
   if (!done)
   {
      andsRemoved = andsRemoved.substr(0, first) + '1';
   }

   return andsRemoved;
}

char Expression::EliminateOrs(std::string expression) const
{
   for(int i = 0; i < expression.length(); i++)
   {
      if (expression.at(i) == '1')
      {
         return '1';
      }
   }
   return '0';
}

bool Expression::RemoveInnerMostParentheses()
{
   bool anyLeft = true;
   unsigned int startingIndex = -1;
   unsigned int endingIndex = -1;
   for (int i = 0; endingIndex == -1 && i < mExpression.length(); i++)
   {
      if (mExpression.at(i) == ')')
      {
         endingIndex = i;
      }
   }

   for (int i = endingIndex; startingIndex == -1 && i >= 0; i--)
   {
      if (mExpression.at(i) == '(')
      {
         startingIndex = i;
      }
   }

   if (endingIndex == -1 || startingIndex == -1)
   {
      anyLeft = false;
   }
   else 
   {
      std::string result = EliminateAnds(mExpression.substr(startingIndex + 1, endingIndex));
      char c = EliminateOrs(result);
      mExpression = mExpression.substr(0, startingIndex) + c + mExpression.substr(endingIndex + 1);
   }


   return anyLeft;
}

char Expression::GetBinaryDigit(unsigned int number, unsigned int index)
{
   char c = '0';
   for (unsigned int i = 0; i < mVariables.size() - index - 1; i++)
   {
      number /= 2;
   }
   if ((number & 1) == 1)
   {
      c = '1';
   }
   return c;
}