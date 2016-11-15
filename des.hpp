#include <cstdint>
#include <vector>
#include <algorithm>
#include <cassert>
#include <bitset>

using namespace std;

typedef uint64_t block_t;
typedef uint64_t my_key_t;
typedef uint32_t half_block_t;
typedef vector<my_key_t> keys_t;
typedef vector<int> permutation_table_t;
typedef vector<int> shift_table_t;
typedef vector<vector<int>> transform_table_t;
typedef vector<transform_table_t> transform_tables_t;

#define GET_BIT(block, pos) ( ( ( block ) & ( 1ull << ( pos ) ) ) != 0 )
#define UPD_BIT(block, bit, pos) ( ( ( bit ) == 0 ) ? CLR_BIT( block, pos ) : SET_BIT( block, pos ) )
#define CLR_BIT(block, pos) ( ( block ) &= ( ~ ( 1ull << ( pos ) ) ) )
#define SET_BIT(block, pos) ( ( block ) |= ( 1ull << ( pos ) ) )

#define HI(block) ( ( half_block_t ) ( ( block ) >> 32 ) )
#define LO(block) ( ( half_block_t ) ( block ) )
#define MAKE_BLOCK(hi, lo) ( ( block_t ) ( ( ( ( block_t ) ( hi ) ) << 32 ) | ( ( block_t ) ( lo ) ) ) )

const shift_table_t TL = {
    1, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const shift_table_t TR = {
    0, 1, 2, 2, 2, 2, 2, 2, 1, 2, 2, 2, 2, 2, 2, 1
};

const permutation_table_t IP = {
    58, 50, 42, 34, 26, 18, 10, 2, 60, 52, 44, 36, 28, 20, 12, 4,
    62, 54, 46, 38, 30, 22, 14, 6, 64, 56, 48, 40, 32, 24, 16, 8,
    57, 49, 41, 33, 25, 17, 9, 1, 59, 51, 43, 35, 27, 19, 11, 3,
    61, 53, 45, 37, 29, 21, 13, 5, 63, 55, 47, 39, 31, 23, 15, 7
};

const permutation_table_t IPI = {
    40, 8, 48, 16, 56, 24, 64, 32, 39, 7, 47, 15, 55, 23, 63, 31,
    38, 6, 46, 14, 54, 22, 62, 30, 37, 5, 45, 13, 53, 21, 61, 29,
    36, 4, 44, 12, 52, 20, 60, 28, 35, 3, 43, 11, 51, 19, 59, 27,
    34, 2, 42, 10, 50, 18, 58, 26, 33, 1, 41, 9, 49, 17, 57, 25
};

const permutation_table_t E = {
    32, 1, 2, 3, 4, 5,
    4, 5, 6, 7, 8, 9,
    8, 9, 10, 11, 12, 13,
    12, 13, 14, 15, 16, 17,
    16, 17, 18, 19, 20, 21,
    20, 21, 22, 23, 24, 25,
    24, 25, 26, 27, 28, 29,
    28, 29, 30, 31, 32, 1
};

const permutation_table_t P = {
    16, 7, 20, 21, 29, 12, 28, 17,
    1, 15, 23, 26, 5, 18, 31, 10,
    2, 8, 24, 14, 32, 27, 3, 9,
    19, 13, 30, 6, 22, 11, 4, 25
};

const permutation_table_t K = {
    57, 49, 41, 33, 25, 17, 9, 1, 58, 50, 42, 34, 26, 18,
    10, 2, 59, 51, 43, 35, 27, 19, 11, 3, 60, 52, 44, 36,
    63, 55, 47, 39, 31, 23, 15, 7, 62, 54, 46, 38, 30, 22,
    14, 6, 61, 53, 45, 37, 29, 21, 13, 5, 28, 20, 12, 4
};

const permutation_table_t KP = {
    14, 17, 11, 24, 1, 5, 3, 28, 15, 6, 21, 10, 23, 19, 12, 4,
    26, 8, 16, 7, 27, 20, 13, 2, 41, 52, 31, 37, 47, 55, 30, 40,
    51, 45, 33, 48, 44, 49, 39, 56, 34, 53, 46, 42, 50, 36, 29, 32
};

const transform_tables_t S = {
    {
        { 14, 4, 13, 1, 2, 15, 11, 8, 3, 10, 6, 12, 5, 9, 0, 7 },
        { 0, 15, 7, 4, 14, 2, 13, 1, 10, 6, 12, 11, 9, 5, 3, 8 },
        { 4, 1, 14, 8, 13, 6, 2, 11, 15, 12, 9, 7, 3, 10, 5, 0 },
        { 15, 12, 8, 2, 4, 9, 1, 7, 5, 11, 3, 14, 10, 0, 6, 13 }
    },
    {
        { 15, 1, 8, 14, 6, 11, 3, 4, 9, 7, 2, 13, 12, 0, 5, 10 },
        { 3, 13, 4, 7, 15, 2, 8, 14, 12, 0, 1, 10, 6, 9, 11, 5 },
        { 0, 14, 7, 11, 10, 4, 13, 1, 5, 8, 12, 6, 9, 3, 2, 15 },
        { 13, 8, 10, 1, 3, 15, 4, 2, 11, 6, 7, 12, 0, 5, 14, 9 }
    },
    {
        { 10, 0, 9, 14, 6, 3, 15, 5, 1, 13, 12, 7, 11, 4, 2, 8 },
        { 13, 7, 0, 9, 3, 4, 6, 10, 2, 8, 5, 14, 12, 11, 15, 1 },
        { 13, 6, 4, 9, 8, 15, 3, 0, 11, 1, 2, 12, 5, 10, 14, 7 },
        { 1, 10, 13, 0, 6, 9, 8, 7, 4, 15, 14, 3, 11, 5, 2, 12 }
    },
    {
        { 7, 13, 14, 3, 0, 6, 9, 10, 1, 2, 8, 5, 11, 12, 4, 15 },
        { 13, 8, 11, 5, 6, 15, 0, 3, 4, 7, 2, 12, 1, 10, 14, 9 },
        { 10, 6, 9, 0, 12, 11, 7, 13, 15, 1, 3, 14, 5, 2, 8, 4 },
        { 3, 15, 0, 6, 10, 1, 13, 8, 9, 4, 5, 11, 12, 7, 2, 14 }
    },
    {
        { 2, 12, 4, 1, 7, 10, 11, 6, 8, 5, 3, 15, 13, 0, 14, 9 },
        { 14, 11, 2, 12, 4, 7, 13, 1, 5, 0, 15, 10, 3, 9, 8, 6 },
        { 4, 2, 1, 11, 10, 13, 7, 8, 15, 9, 12, 5, 6, 3, 0, 14 },
        { 11, 8, 12, 7, 1, 14, 2, 13, 6, 15, 0, 9, 10, 4, 5, 3 }
    },
    {
        { 12, 1, 10, 15, 9, 2, 6, 8, 0, 13, 3, 4, 14, 7, 5, 11 },
        { 10, 15, 4, 2, 7, 12, 9, 5, 6, 1, 13, 14, 0, 11, 3, 8 },
        { 9, 14, 15, 5, 2, 8, 12, 3, 7, 0, 4, 10, 1, 13, 11, 6 },
        { 4, 3, 2, 12, 9, 5, 15, 10, 11, 14, 1, 7, 6, 0, 8, 13 }
    },
    {
        { 4, 11, 2, 14, 15, 0, 8, 13, 3, 12, 9, 7, 5, 10, 6, 1 },
        { 13, 0, 11, 7, 4, 9, 1, 10, 14, 3, 5, 12, 2, 15, 8, 6 },
        { 1, 4, 11, 13, 12, 3, 7, 14, 10, 15, 6, 8, 0, 5, 9, 2 },
        { 6, 11, 13, 8, 1, 4, 10, 7, 9, 5, 0, 15, 14, 2, 3, 12 }
    },
    {
        { 13, 2, 8, 4, 6, 15, 11, 1, 10, 9, 3, 14, 5, 0, 12, 7 },
        { 1, 15, 13, 8, 10, 3, 7, 4, 12, 5, 6, 11, 0, 14, 9, 2 },
        { 7, 11, 4, 1, 9, 12, 14, 2, 0, 6, 10, 13, 15, 3, 5, 8 },
        { 2, 1, 14, 7, 4, 10, 8, 13, 15, 12, 9, 0, 3, 5, 6, 11 }
    }
};

struct data
{
    size_t size;
    vector<char> _data;
};

int make_number(char a, char b)
{
    int result = 0;
    UPD_BIT(result, a, 1);
    UPD_BIT(result, b, 0);
    return result;
}

int make_number(char a, char b, char c, char d)
{
    int result = 0;
    UPD_BIT(result, a, 3);
    UPD_BIT(result, b, 2);
    UPD_BIT(result, c, 1);
    UPD_BIT(result, d, 0);
    return result;
}

block_t permutate(block_t block, const permutation_table_t permutation_table)
{
    block_t result = 0;

    for (size_t i = 0; i < permutation_table.size(); ++i)
    {
        UPD_BIT(result, GET_BIT(block, permutation_table[i] - 1), i);
    }

    return result;
}

block_t permutate(half_block_t block, const permutation_table_t permutation_table)
{
    block_t result = 0;

    for (size_t i = 0; i < permutation_table.size(); ++i)
    {
        UPD_BIT(result, GET_BIT(block, permutation_table[i] - 1), i);
    }

    return result;
}
half_block_t permutate_half(half_block_t block, const permutation_table_t permutation_table)
{
    half_block_t result = 0;

    for (size_t i = 0; i < permutation_table.size(); ++i)
    {
        UPD_BIT(result, GET_BIT(block, permutation_table[i] - 1), i);
    }

    return result;
}

half_block_t f(half_block_t block, my_key_t key)
{
    half_block_t result = 0;
    block_t t = permutate(block, E);
    t ^= key;

    for (int it = 0, i = 47, j = 31; it < 8; ++it, i -= 6, j -= 4)
    {
        int a = make_number(GET_BIT(t, i - 0), GET_BIT(t, i - 5));
        int b = make_number(GET_BIT(t, i - 1), GET_BIT(t, i - 2), GET_BIT(t, i - 3), GET_BIT(t, i - 4));
        int c = S[it][a][b];
        UPD_BIT(result, GET_BIT(c, 3), j - 0);
        UPD_BIT(result, GET_BIT(c, 2), j - 1);
        UPD_BIT(result, GET_BIT(c, 1), j - 2);
        UPD_BIT(result, GET_BIT(c, 0), j - 3);
    }

    result = permutate_half(result, P);
    return result;
}

my_key_t shift_key_left(my_key_t key, int shift)
{
    for (int i = 0; i < shift; ++i)
    {
        key <<= 1;
        UPD_BIT(key, GET_BIT(key, 56), 0);
    }
    return key;
}

my_key_t shift_key_right(my_key_t key, int shift)
{
    for (int i = 0; i < shift; ++i)
    {
        UPD_BIT(key, GET_BIT(key, 0), 56);
        key >>= 1;
    }
    return key;
}

keys_t get_keys(my_key_t key)
{
    my_key_t t = permutate(key, K);
    cout << "fkp: " << bitset<64>(t) << endl;
    keys_t result(16);
    for (int i = 0; i < 16; ++i)
    {
        t = shift_key_left(t, TL[i]);
        result[i] = permutate(t, KP);
    }

    return result;
}

keys_t i_get_keys(my_key_t key)
{
    my_key_t t = permutate(key, K);
    keys_t result(16);
    for (int i = 0; i < 16; ++i)
    {
        t = shift_key_right(t, TL[i]);
        result[i] = permutate(t, KP);
    }

    return result;
}

block_t transform(block_t block, my_key_t key)
{
    half_block_t lo = LO(block);
    half_block_t hi = HI(block);

    return MAKE_BLOCK(lo, hi ^ f(lo, key));
}

block_t i_transform(block_t block, my_key_t key)
{
    half_block_t lo = LO(block);
    half_block_t hi = HI(block);

    return MAKE_BLOCK(lo ^ f(hi, key), hi);
}
keys_t keys;
block_t encrypt(const block_t source_data, my_key_t key)
{
    keys = get_keys(key);
    block_t data = source_data;
    data = permutate(data, IP);
    cout << "IP: " << bitset<64>(data) << endl;
    for (int i = 0; i < 16; ++i)
    {
        cout << i << ": " << hex << keys[i] << " " << bitset<64>(keys[i]) << endl;
        data = transform(data, keys[i]);
    }
    data = permutate(data, IPI);
    return data;
}

block_t decrypt(const block_t encrypted_data, my_key_t key)
{
    //keys_t keys = i_get_keys(key);
    block_t data = encrypted_data;
    reverse(keys.begin(), keys.end());
    data = permutate(data, IP);
    for (int i = 0; i < 16; ++i)
    {
        cout << i << ": " << hex << keys[i] << " " << bitset<64>(keys[i]) << endl;
        data = transform(data, keys[i]);
    }
    data = permutate(data, IPI);
    return data;
}
