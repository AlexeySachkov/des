#include "des.hpp"
#include <iostream>
#include <bitset>
using namespace std;

three_quarter_blocks_t keys;

int xor_(int a, int b)
{
    return (a == b) ? 0 : 1;
}

half_block_t f(half_block_t block, three_quarter_block_t key)
{
	half_block_t result;
	three_quarter_block_t t = permutate<HALF_BLOCK_SIZE, THREE_QUARTER_BLOCK_SIZE>(block, E, 1);
	t = xor_<THREE_QUARTER_BLOCK_SIZE>(t, key);

	for (int it = 0; it < 8; ++it)
	{
		int o_offset = it * 6;
		int r_offset = it * 4;
		array<int, 2> a;
		array<int, 4> b;
		a[0] = t[o_offset + 0];
		a[1] = t[o_offset + 5];

		b[0] = t[o_offset + 1];
		b[1] = t[o_offset + 2];
		b[2] = t[o_offset + 3];
		b[3] = t[o_offset + 4];

		int ai = a[1] + a[0] * 2;
		int bi = b[3] + b[2] * 2 + b[1] * 4 + b[0] * 8;

		bitset<4> c = S[it][ai][bi];

		result[r_offset + 0] = c[3];
		result[r_offset + 1] = c[2];
		result[r_offset + 2] = c[1];
		result[r_offset + 3] = c[0];
	}

	result = permutate<HALF_BLOCK_SIZE, HALF_BLOCK_SIZE>(result, P, 1);
	return result;
}

three_quarter_blocks_t generate_keys(const key_t &key)
{
	block_t _key;
	_key.fill(0);
	for (size_t i = 0, j = 0; i < BLOCK_SIZE; ++i)
	{
		if ((i + 1) % 8 == 0)
		{
			continue;
		}
		else
		{
			_key[i] = key[j];
			++j;
		}
	}
	key_t t = permutate<BLOCK_SIZE, KEY_SIZE>(_key, K, 1);
	three_quarter_blocks_t result(16);

	for (int i = 0; i < 16; ++i)
	{
		auto p = split<KEY_SIZE>(t);
		p.first = lc_shift<HALF_KEY_SIZE>(p.first, TL[i]);
		p.second = lc_shift<HALF_KEY_SIZE>(p.second, TL[i]);
		t = merge<KEY_SIZE>(p);
		
		result[i] = permutate<KEY_SIZE, THREE_QUARTER_BLOCK_SIZE>(t, KP, 1);
	}

	return result;
}

block_t transform(block_t block, three_quarter_block_t key)
{
	auto p = split<BLOCK_SIZE>(block);
	return merge<BLOCK_SIZE>(p.second, xor_<HALF_BLOCK_SIZE>(p.first, f(p.second, key)));
}

block_t i_transform(block_t block, three_quarter_block_t key)
{
	auto p = split<BLOCK_SIZE>(block);
	return merge<BLOCK_SIZE>(xor_<HALF_BLOCK_SIZE>(p.second, f(p.first, key)), p.first);
}

block_t encrypt(const block_t source_data, const key_t &key)
{
	keys = generate_keys(key);
	block_t data = source_data;

	data = permutate<BLOCK_SIZE, BLOCK_SIZE>(data, IP, 1);
	for (int i = 0; i < 16; ++i)
	{
		data = transform(data, keys[i]);
	}
	data = permutate<BLOCK_SIZE, BLOCK_SIZE>(data, IPI, 1);
	return data;
}

block_t decrypt(const block_t encrypted_data, const key_t &key)
{
	keys = generate_keys(key);
	block_t data = encrypted_data;

	data = permutate<BLOCK_SIZE, BLOCK_SIZE>(data, IP, 1);
	for (int i = 15; i >= 0; --i)
	{
		data = i_transform(data, keys[i]);
	}
	data = permutate<BLOCK_SIZE, BLOCK_SIZE>(data, IPI, 1);
	return data;
}
