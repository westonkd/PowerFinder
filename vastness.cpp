/*************************************************************************
 * Program:
 *    Vastness -- Exploring the unknown expanse of algorithmic power.
 *
 * Author:
 *    Weston Dransfield
 *
 * Summary:
 *    An exciting program to experiment with power of 2 prefixes.
 *    Compile like: g++ vastnessCLI.cpp vastness.cpp -lmpfr -lgmp
 *************************************************************************/

#include <cstdlib>
#include <cstring>
#include <iostream>
#include <fstream>
#include <vector>
#include <string>
#include <initializer_list>
#include <stdio.h>
#include <gmp.h>
#include <mpfr.h>
#include <iostream>
#include <stdarg.h>
#include <math.h>
#include <sys/time.h>
#include <stdlib.h>

 using namespace std;

string pb = "\n\n"; // used by the whatIlearned script

/****************************************************************
 * Class with methods to find the elusive power of two beginging
 * with the user-specified string of digits.
 ****************************************************************/
 class PowerFinder
 {
 public:
   PowerFinder(long double prefix, long double secondsToWait)
   {
      initialize(prefix, secondsToWait); 
   }

   ~PowerFinder()
   {
    //clear GMP floating point vars
    mpfr_clear(lowerBound);
    mpfr_clear(upperBound);
    mpfr_clear(logTenTwo);
   } 

  //use mathmematical wizardry to find the correct number
 unsigned int findPowerOfTwo();

  //run test file
 void runFileTest(string fileName);

private:
  bool isPowerOfTwo();
  void toFractionalPart(mpfr_t & number);
  void initialize(long double prefix, long double secondsToWait);
  long double prefix;
  mpfr_t lowerBound;
  mpfr_t upperBound;
  mpfr_t logTenTwo;
  long double secondsToWait;
};

/****************************************************************
 * isPowerOfTwo
 *
 * check if the prefix is a power of two.
 ****************************************************************/
 bool PowerFinder::isPowerOfTwo()
 {
  //get the prefix
  int n = prefix;

  //while prefix is even and greater than one
  while (((n & 1) == 0) && n > 1) 
    n >>= 1;
 if (n == 1)
    return true;
 return false;
}

/****************************************************************
 * initialize
 *  
 * Initialize member variables.
 *
 * SET logTenTwo to log_10(2)
 * SET lowerBound to log_10(prefix)
 * SET uppwerBound to log_10(prefix + 1)
 ****************************************************************/
void PowerFinder::initialize(long double prefix, long double secondsToWait)
{
   this->prefix = prefix;
   this->secondsToWait = secondsToWait;

   //set the default precision
   mpfr_set_default_prec(128);

   //initialize GMP floating point vars
   mpfr_init(lowerBound);
   mpfr_init(upperBound);
   mpfr_init(logTenTwo);

   //set the correct values
   mpfr_set_d(logTenTwo, 2.0, MPFR_RNDN);
   mpfr_log10 (logTenTwo, logTenTwo, MPFR_RNDN);

   //set the lower limit to log_10 of prefix
   mpfr_set_d(lowerBound, prefix, MPFR_RNDN);
   mpfr_log10 (lowerBound, lowerBound, MPFR_RNDN);
   toFractionalPart(lowerBound);

   //set the upper bound to log_10 of prefix + 1
   mpfr_set_d(upperBound, prefix + 1, MPFR_RNDN);
   mpfr_log10 (upperBound, upperBound, MPFR_RNDN);
   toFractionalPart(upperBound);
}

/****************************************************************
 * findPowerOfTwo
 *
 * Finds the power of two that begins with prefix.
 *
 * IF prefix is a power of two
 *    RETURN log_2(prefix)
 * SET correctPower to prefix / 2
 * IF log_10(prefix + 1) != 0
 *    WHILE true
 *        IF over time limit
 *            RETURN -1
 *        SET fractionalPart to the mantissa of log_10(2) * correctPower
 *        If fractionalPart is between lowerBound and upperBound
 *            RETURN correctPower
 *        SET correctPower to correctPower + 1     
 * ELSE
 *    Do same as above without upper bound check
 ****************************************************************/
 unsigned int PowerFinder::findPowerOfTwo()
 {
  //if the prefix is a power of two, the answer is trivial
  if (isPowerOfTwo())
    return log2(prefix);

  //the number to return (used as a counter as well).
 unsigned int correctPower = prefix / 2;

  //the number to compare against
 mpfr_t fractionalPart;
 mpfr_init(fractionalPart);

  //begin timing
 clock_t start = clock();
 clock_t end = clock();

  //if !log_10(n + 1) = 0
 if (mpfr_get_flt(upperBound, MPFR_RNDN) > 0)
 {
   //while the correct power of two has not been found
   while (true)
   {
      //if we have gone over time
      if ((end - start) / (double) CLOCKS_PER_SEC > secondsToWait)
         return -1;

      //get the next factor of log_10(2)
      mpfr_mul_si(fractionalPart, logTenTwo, correctPower, MPFR_RNDN);
      
      //get its fractional part
      toFractionalPart(fractionalPart);
      
      //cout <<  mpfr_get_ld(lowerBound, MPFR_RNDN) << " " << mpfr_get_ld(fractionalPart, MPFR_RNDN) << " " << mpfr_get_ld(upperBound, MPFR_RNDN) << endl;
      
      //if (frac > lowerBound && frac < upperBound)
      if (mpfr_cmp(fractionalPart, lowerBound) >= 0 && mpfr_cmp(upperBound, fractionalPart) > 0)
         break;
      
      correctPower++;

      //update the time
      end = clock();
   } 
}
else
{
   //while the correct power of two has not been found
   while (true)
   {
      //if we have gone over time
      if ((end - start) / (double) CLOCKS_PER_SEC > secondsToWait)
         return -1;

      //get the next factor of log_10(2)
      mpfr_mul_si(fractionalPart, logTenTwo, correctPower, MPFR_RNDN);
      
      //get its fractional part
      toFractionalPart(fractionalPart);
      
      //if (frac > lowerBound)
      if (mpfr_cmp(fractionalPart, lowerBound) >= 0)
         break;
      
      correctPower++;

      //update the time
      end = clock();
   } 
}

mpfr_clear(fractionalPart);
return correctPower;
}

/****************************************************************
 * toFractionalPart
 * Removes the integer part of a number.
 ****************************************************************/
 void PowerFinder::toFractionalPart(mpfr_t & number)
 {
  //get the integer part
  int intPart = mpfr_get_d(number, MPFR_RNDN);

  //subtract the integer part
  mpfr_sub_si(number, number, intPart, MPFR_RNDN);
}

 /****************************************************************
 * runFileTest
 * Run a test for each number in the file
 ****************************************************************/
 void PowerFinder::runFileTest(string fileName)
 {
   ifstream fin(fileName.c_str());
   if (fin.fail())
      cerr << "Error opening " << fileName << ".\n";

   long double temp;

   //for every number in the file
   while(fin >> temp)
   {
      //output the number
      cout << fixed << temp << ",";

      //set the new test value
      initialize(temp, 3600);

      //output the time and answer
      clock_t start = clock();
      cout << findPowerOfTwo() << "," ;
      clock_t finish = clock();
      cout << (finish - start) / (double) CLOCKS_PER_SEC << endl;
   }

   fin.close();
}

/****************************************************************
 * A singleton Args class for encapsulated global access.
 ****************************************************************/
 class Args
 {
 public:
   long double prefix;
   long double secondsToWait;

   Args()
   {
      prefix = 2;
      secondsToWait = 300;
   }
} Args;

/****************************************************************
 * getArgs
 *
 *  gets command-line arguments and
 *  consolidates the information into one string,
 *  which is returned.
 ****************************************************************/
 string getArgs(int argc, const char* argv[])
 {
   long double prefix = 0;
   long double secondsToWait = 300;

   if (argc == 2)
   {
      prefix = strtod(argv[1], NULL);
   }
   else if (argc == 3)
   {
      prefix = strtod(argv[1], NULL);
      secondsToWait = strtod(argv[2], NULL);
   }

   Args.prefix = prefix;
   Args.secondsToWait = secondsToWait;

   return string("useArgs");
}

/**************************************************************************
 * run
 *
 *  finds the power of two that has a user-supplied prefix
 *  long double version
 *************************************************************************/
 void run(string params)
 {
   long double prefix = Args.prefix;
   long double secondsToWait = Args.secondsToWait;

   cout << "Finding the smallest power of 2 that begins with " << prefix << endl << endl;

   PowerFinder powerFinder(prefix, secondsToWait);

   clock_t start = clock();
   int answer = powerFinder.findPowerOfTwo();
   clock_t finish = clock();

   cout << "The answer is: " << "2 ^ " << answer << endl;
   cout << (finish - start) / (double) CLOCKS_PER_SEC << " seconds to calculate.\n\n";
}

/*************************************************************************
 * usage
 *
 * Tells the user how to use the program.
 *************************************************************************/
 void usage(const char * programName)
 {
   cout << "Ussage is: \n";
   cout << programName << " <prefix_to_find> <seconds_willing_to_wait>\n";
}

/*************************************************************************
 * learned
 *
 * Tells interested parties what you learned.
 *************************************************************************/
 void learned()
 {
   cout << "Lots!"; 
}
