
#include <iostream>
#include <cstdlib>
#include <string>
#include <vector>
#include <map>
#include <algorithm>

#include "Utilities/General/interface/MapVector.hh"

using std::cout;
using std::cerr;
using std::endl;

int main()
{
	typedef int Key;
	typedef std::vector<double> Data;
	typedef MapVector<Key,Data> Map;

	Map x;

	x[5].push_back(1.1);
	x[5].push_back(1.2);
	x[5].push_back(1.3);

	x[2].push_back(2.1);
	x[2].push_back(2.2);
	x[2].push_back(2.3);

	x[5].push_back(1.4);
	x[2].push_back(2.4);

	int size = x.size();
	if(size!=2)
	{
		std::cerr << "wrong size for map (" << size << ")" << std::endl;
		return -1;
	}

	int s5 = x[5].size();
	if(s5!=4)
	{
		std::cerr << "wrong size for vector at key=5 (" << s5 << ")" << std::endl;
		return -1;
	}

	int s2 = x[2].size();
	if(s2!=4)
	{
		std::cerr << "wrong size for vector at key=2 (" << s2 << ")" << std::endl;
		return -1;
	}

	if(x[5].at(1)!=1.2)
	{
		std::cerr << "wrong value for vector at key=5" << std::endl;
		return -1;
	}
	if(x[2].at(1)!=2.2)
	{
		std::cerr << "wrong value for vector at key=5" << std::endl;
		return -1;
	}

	return 0;
}

