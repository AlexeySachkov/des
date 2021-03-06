#include <cstdint>
#include <vector>
#include <algorithm>
#include <cassert>
#include <array>
#include <iostream>

using namespace std;

#define BLOCK_SIZE 64
#define HALF_BLOCK_SIZE (BLOCK_SIZE / 2)
#define THREE_QUARTER_BLOCK_SIZE (BLOCK_SIZE / 4 * 3)
#define KEY_SIZE 56
#define HALF_KEY_SIZE (KEY_SIZE / 2)

typedef array<int, BLOCK_SIZE> block_t;
typedef array<int, HALF_BLOCK_SIZE> half_block_t;
typedef array<int, THREE_QUARTER_BLOCK_SIZE> three_quarter_block_t;
typedef array<int, KEY_SIZE> key_t;
typedef array<int, HALF_KEY_SIZE> half_key_t;

typedef vector<block_t> blocks_t;
typedef vector<three_quarter_block_t> three_quarter_blocks_t;
typedef three_quarter_blocks_t keys_t;
typedef vector<int> permutation_table_t;
typedef vector<int> shift_table_t;
typedef vector<vector<int>> transform_table_t;
typedef vector<transform_table_t> transform_tables_t;


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

int xor_(int, int);

template<int Size>
array<int, Size> xor_(const array<int, Size> &a, const array<int, Size> &b)
{
    array<int, Size> res;
    for (size_t i = 0; i < Size; ++i)
    {
        res[i] = xor_(a[i], b[i]);
    }
    return res;
}

template<int From, int To>
array<int, To> permutate(const array<int, From> &block, const permutation_table_t permutation_table, int first_bit_index = 0)
{
	assert(To == permutation_table.size());
    array<int, To> result;

    for (size_t i = 0; i < permutation_table.size(); ++i)
    {
    	result[i] = block[permutation_table[i] - first_bit_index];
    }

    return result;
}

template<int Size>
array<int, Size> lc_shift(const array<int, Size> &block, int times)
{
    array<int, Size> t = block;

	for (int it = 0; it < times; ++it)
	{
		int l = t[0];
        for (size_t i = 0; i < Size - 1; ++i)
        {
            t[i] = t[i + 1];
        }
        t[Size - 1] = l;
	}

	return t;
}

template<int Size>
pair<array<int, Size / 2>, array<int, Size / 2>> split(array<int, Size> key)
{
	constexpr int half_size = Size / 2;
	array<int, half_size> l, r;
	for (int i = 0; i < half_size; ++i)
	{
		l[i] = key[i];
	}
	for (int i = 0; i < half_size; ++i)
	{
		r[i] = key[half_size + i];
	}

	return { l, r };
}

template<int Size>
array<int, Size> merge(pair<array<int, Size / 2>, array<int, Size / 2>> p)
{
	array<int, Size> result;
	constexpr int half_size = Size / 2;

	for (int i = 0; i < half_size; ++i)
	{
		result[i] = p.first[i];
	}
	for (int i = 0; i < half_size; ++i)
	{
		result[half_size + i] = p.second[i];
	}

	return result;
}

template<int Size>
array<int, Size> merge(array<int, Size / 2> l, array<int, Size / 2> r)
{
	return merge<Size>({ l, r });
}

half_block_t f(half_block_t block, three_quarter_block_t key);

keys_t generate_keys(const key_t &key);

block_t transform(block_t block, three_quarter_block_t key);

block_t i_transform(block_t block, three_quarter_block_t key);

block_t encrypt(const block_t source_data, const keys_t &key);

block_t decrypt(const block_t encrypted_data, const keys_t &key);

template<size_t N>
void print(const array<int, N> &a)
{
    for (auto &s: a)
    {
        cout << s;
    }
    cout << endl;
}