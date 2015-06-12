// //if the square root of n is a whole number 
// double sqrtOfN = sqrt(n);
// if(sqrtOfN == (int)sqrtOfN)
// {

// }

// //or

// f (floor(foobar) == foobar)
//     cout << "It's whole";
// else
//     cout << "Not whole";

#include <iostream>
#include <math.h> 
#include <stdlib.h>
#include <string>
#include <bitset>

//JUST DO LOG_2 of the number if it is a power of 2 to get 2^n

using namespace std;

int isPowerOfTwo (int n)
{
	//keep the original
	const int original = n;

	//while n is even and greater than one
	while (((n & 1) == 0) && n > 1) 
		n >>= 1;
	if (n == 1)
	{
		//get the binary number
		bitset<32> binaryNum = original;
		cout << binaryNum << endl;

		return 1;
	}

	return -1;
}

int main(int argc, char** argv) 
{
	if (argc > 1) 
	{
		//tet
		cout << isPowerOfTwo(atoi(argv[1]));
	}
}