#include <iostream>
#include <iomanip>
#include <vector>
#include <bitset>

#include "des.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    block_t source = 1ull;
    my_key_t key = 1ull;//0xABACABA;

    cout << bitset<64>(source) << endl;
    cout << bitset<64>(key) << endl;

    cout << "encrypt: " << endl;
    block_t encrypted = encrypt(source, key);
    cout << "decrypt: " << endl;
    block_t decrypted = decrypt(encrypted, key);

    cout << endl;
    cout << encrypted;
    cout << endl;
    cout << decrypted << endl;

    return 0;
}
