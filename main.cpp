#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>

#include "des.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    block_t source = 1ull;
    key_t key = 0xABACABA;

    cout << source << endl;
    cout << key << endl;

    cout << "encrypt: " << endl;
    block_t encrypted = encrypt(source, key);
    cout << "decrypt: " << endl;
    block_t decrypted = decrypt(encrypted, key);

    cout << endl;
    cout << encrypted.to_ullong() << endl;
    cout << encrypted;
    cout << endl;
    cout << decrypted.to_ullong() << endl;
    cout << decrypted << endl;

    return 0;
}