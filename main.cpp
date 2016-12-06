#include <iostream>
#include <iomanip>
#include <vector>
#include <array>

#include "des.hpp"

using namespace std;

int main(int argc, char* argv[])
{
    block_t source;
    source.fill(0);
    source[14] = 1;
    source[28] = 1;
    source[63] = 1;
    const key_t key = {1,1,1,0,1,1,1,1,1,1,0,1,1,0,1,1,1,0,1,1,0,1,1,1,0,1,1,1,1,1,1,0,1,1,1,0,1,1,0,1,1,1,1,1,1,0,1,0,1,1,1,1,0,1,0,1};

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