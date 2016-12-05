#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

#include "des.hpp"

using namespace std;

template<size_t N>
void print(array<int, N>&a)
{
	for (auto &s: a)
	{
		cout << s;
	}
	cout << endl;
}

int main(int argc, char* argv[])
{
    block_t source;
    source.fill(0);
    source[63] = 1;
    key_t key = {1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1,1,1,1,0,1,1,1};

    print(source);
    print(key);

    cout << "encrypt: " << endl;
    block_t encrypted = encrypt(source, key);
    cout << "decrypt: " << endl;
    block_t decrypted = decrypt(encrypted, key);

    cout << endl;
    cout << endl;
    print(source);
    cout << endl;
    print(encrypted);
    cout << endl;
    print(decrypted);

    return 0;
}