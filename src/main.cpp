#include <iostream>
#include <fstream>
#include <string>
#include <array>

#include "des.hpp"

using namespace std;

enum mode_t { ENCRYPT, DECRYPT, UNDEF };

struct arguments
{
    mode_t mode;
    string source_filename;
    string result_filename;
    string key;

    arguments()
        : mode(UNDEF)
        , source_filename("")
        , result_filename("")
        , key("")
    {};
};

bool parse_arguments(int, char* [], arguments &);
blocks_t read_file(const string &);
void write_file(const string &, const blocks_t &);
key_t extract_key_from_string(const string &);

int main(int argc, char* argv[])
{
    arguments args;

    if (!parse_arguments(argc, argv, args))
    {
        return -1;
    }

    key_t key = extract_key_from_string(args.key);
    keys_t keys = generate_keys(key);

    if (args.mode == ENCRYPT)
    {
        blocks_t blocks = read_file(args.source_filename);
        blocks_t encrypted(blocks.size());

        for (size_t i = 0; i < blocks.size(); ++i)
        {
            encrypted[i] = encrypt(blocks[i], keys);
        }

        write_file(args.result_filename, encrypted);
        cout << "encrypted!" << endl;
    }
    else if (args.mode == DECRYPT)
    {
        blocks_t blocks = read_file(args.source_filename);
        blocks_t decrypted(blocks.size());

        for (size_t i = 0; i < blocks.size(); ++i)
        {
            decrypted[i] = decrypt(blocks[i], keys);
        }

        write_file(args.result_filename, decrypted);
        cout << "decrypted!" << endl;
    }

    return 0;
}

bool parse_arguments(int argc, char* argv[], arguments &args)
{
    for (int i = 1; i < argc; ++i)
    {
        string current = argv[i];

        if (current == "-k" || current == "--key")
        {
            if (i + 1 < argc)
            {
                args.key = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Key is missed!" << endl;
                return false;
            }
        }
        else if (current == "-o" || current == "--output")
        {
            if (i + 1 < argc)
            {
                args.result_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Output file is missed!" << endl;
                return false;
            }
        }
        else if (current == "-s" || current == "--source")
        {
            if (i + 1 < argc)
            {
                args.source_filename = argv[i + 1];
                ++i;
            }
            else
            {
                cout << "Source file is missed!" << endl;
                return false;
            }
        }
        else if (current == "-m" || current == "--mode")
        {
            if (i + 1 < argc)
            {
                string _mode = argv[i + 1];
                if (_mode == "encrypt")
                {
                    args.mode = ENCRYPT;
                }
                else if (_mode == "decrypt")
                {
                    args.mode = DECRYPT;
                }
                else
                {
                    cout << "Unknown mode " << _mode << endl;
                    return false;
                }
                ++i;   
            }
            else
            {
                cout << "Mode is missed!" << endl;
                return false;
            }
        }
        else
        {
            cout << "Unknown argument " << argv[i] << endl;
            return false;
        }
    }

    return true;
}

blocks_t read_file(const string &filename)
{
    ifstream file(filename.c_str(), fstream::in | fstream::binary);

    file.seekg(0, file.end);
    size_t length = file.tellg();
    file.seekg(0, file.beg);

    constexpr auto SYMBOLS_IN_BLOCK = BLOCK_SIZE / 8;
    size_t blocks_count = length / SYMBOLS_IN_BLOCK + (length % SYMBOLS_IN_BLOCK > 0);
    blocks_t blocks(blocks_count);

    char* buffer = new char[length];

    file.read(buffer, length);

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        for (size_t j = i * SYMBOLS_IN_BLOCK, index = 0; j < min(i * SYMBOLS_IN_BLOCK + SYMBOLS_IN_BLOCK, length); ++j)
        {
            for (int pos = 0; pos < 8; ++pos, ++index)
            {
                blocks[i][index] = ((buffer[j] & (1 << (8 - pos - 1))) != 0);
            }
        }
    }

    // close file
    file.close();
    delete[] buffer;

    return blocks;
}

void write_file(const string &filename, const blocks_t &blocks)
{
    ofstream file(filename.c_str(), fstream::binary);

    constexpr auto SYMBOLS_IN_BLOCK = BLOCK_SIZE / 8;
    size_t length = SYMBOLS_IN_BLOCK * blocks.size();
    char* buffer = new char[length];
    for (size_t i = 0; i < length; ++i)
    {
        buffer[i] = 0;
    }

    for (size_t i = 0; i < blocks.size(); ++i)
    {
        for (size_t j = i * SYMBOLS_IN_BLOCK, index = 0; j < i * SYMBOLS_IN_BLOCK + SYMBOLS_IN_BLOCK; ++j)
        {
            for (int pos = 0; pos < 8; ++pos, ++index)
            {
                if (blocks[i][index] != 0)
                {
                    buffer[j] |= (1 << (8 - pos - 1));
                }
            }
        }
    }

    file.write(buffer, length);

    file.close();
    delete[] buffer;

    return;
}


key_t extract_key_from_string(const string &string_key)
{
    constexpr auto SYMBOLS_IN_KEY = KEY_SIZE / sizeof(char);
    key_t key;

    for (size_t j = 0, index = 0; j < min(SYMBOLS_IN_KEY, string_key.size()); ++j)
    {
        for (int pos = 0; pos < 8; ++pos, ++index)
        {
            key[index] = ((string_key[j] & (1 << (8 - pos - 1))) != 0);
        }
    }

    return key;
}