#include <iostream>
#include <math.h>
#include <stdlib.h>  

using namespace std;

bool isPowerOfTwo (int n)
{
	//keep the original
	const int original = n;

	//while n is even and greater than one
	while (((n & 1) == 0) && n > 1) 
		n >>= 1;
	if (n == 1)
		return true;

	return false;
}

int findPower(const int prefix)
{
	//if the number is a power of 2
	if (isPowerOfTwo(prefix)) {
		cout << "Easy find: " << log2(prefix) << endl;
		return 0;
	}

	const float lowerBound = log10(prefix) - ((int) log10(prefix));
	const float upperBound = log10(prefix + 1) - ((int) log10(prefix + 1));
	const float logTwo = log10(2);

	//cout << lowerBound << ", " << upperBound << endl;
	int count = 1;
	while(true)
	{
		int whole = count * logTwo;
		float frac = count * logTwo - whole;

		if (frac > lowerBound && frac < upperBound)
			break;

		count++;
	}

	cout << "found: " <<  count <<  " -> " << pow(2,count) << endl;

	return 0;
}

int main(int argc, char* argv[])
{

	if (argc > 1) 
	{
		findPower(atoi(argv[1]));
	}

	return 0;
}
