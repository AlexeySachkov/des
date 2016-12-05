#include <iostream>
#include <bitset>

#include "des.hpp"

/*void test_lc_shift_01()
{
	const size_t size = 10;

	bitset<size> source("0110101110");
	bitset<size> reference("1101011100");

	bitset<size> res = lc_shift<size>(source, 1);
	cout << source << endl << res << endl << reference << endl;
}

void test_lc_shift_02()
{
	const size_t size = 10;

	bitset<size> source("0110101110");
	bitset<size> reference("1010111001");

	bitset<size> res = lc_shift<size>(source, 2);
	cout << source << endl << res << endl << reference << endl;
}

void test_S_01()
{
	bitset<6> t("101110");
	bitset<4> result;

	bitset<2> a;
	bitset<4> b;
	int o_offset = 0;
	int r_offset = 0;
	int it = 0;
	a[1] = t[o_offset + 0];
	a[0] = t[o_offset + 5];

	b[3] = t[o_offset + 1];
	b[2] = t[o_offset + 2];
	b[1] = t[o_offset + 3];
	b[0] = t[o_offset + 4];

	auto ai = a.to_ulong(), bi = b.to_ulong();

	bitset<4> c = S[it][ai][bi];

	result[r_offset + 0] = c[0];
	result[r_offset + 1] = c[1];
	result[r_offset + 2] = c[2];
	result[r_offset + 3] = c[3];

	cout << t << " " << a << "(" << a.to_ulong() << ") " << b << "(" << b.to_ulong() << ")" << endl;
	cout << result << " " << result.to_ulong() << endl;
}*/

int main()
{
	/*test_lc_shift_01();
	cout << endl;
	test_lc_shift_02();
	cout << endl;
	test_S_01();*/

	return 0;
}