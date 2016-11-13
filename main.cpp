#include <iostream>
#include <vector>

#include "des.hpp"

using namespace std;

int main(int argc, char* argv[])
{
	block_t source = 0xDEADBEEF;
	key_t key = 0xABACABA;

	block_t encrypted = encrypt(source, key);
	block_t decrypted = decrypt(encrypted, key);

	cout << source << endl;
	cout << key << endl;
	cout << endl;
	cout << encrypted;
	cout << endl;
	cout << decrypted << endl;

	return 0;
}