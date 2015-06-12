/*************************************************************************
 * Program:
 *    Vastness -- Exploring the unknown expanse of algorithmic power.
 *
 * Author:
 *    Brother Neff
 *
 * Summary:
 *    A program to experiment with power of 2 prefixes.
 *************************************************************************/
#include <cstdlib>
#include <string>
using namespace std;

/*************************************************************************
 * Function prototypes
 *************************************************************************/
void learned();
void usage(const char *);
void run(string);

/****************************************************************
 * getArgs
 *
 *  gets command-line arguments and
 *  consolidates the information into one string,
 *  which is returned.
 ****************************************************************/
string getArgs(int argc, const char* argv[]);

/**************************************************************
 * main looks at its command-line parameters.
 * If none, it calls two functions in order, namely
 *   learned
 *   usage
 * Otherwise it calls getArgs to parse parameters into one
 * string which is then passed to run.
 ***************************************************************/
int main(int argc, const char* argv[])
{
   if (argc == 1)
   {
      learned();
      usage(argv[0]);
   }
   else
   {
      run(getArgs(argc, argv));
   }
   return 0;
}   
