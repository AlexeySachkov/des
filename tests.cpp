#include <iostream>
#include <array>

#include "des.hpp"

int main()
{
	key_t key = {1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1};
	three_quarter_blocks_t keys1 = generate_keys(key);
	three_quarter_blocks_t keys2 = generate_keys(key);

	for (int i = 0; i < 16; ++i)
	{
		if (keys1[i] != keys2[i])
		{
			print(keys1[i]);
			print(keys2[i]);
			cout << i << endl;
		}
	}

	return 0;
}