//
// 	This is a rolling hash that is or resembles a "BuzHash".
//	Aim is be efficient, above all else.
//

#include "common.h"
#include "ZRollingHash.h"

extern unsigned* table_prime(unsigned, unsigned);

void ZRollingHash::prime(unsigned seed, unsigned _mask) {
	table = table_prime(seed, _mask);
	mask = _mask;
}

int ZRollingHash::scanBuffer(char_p p0, int n0) {
	// Rather skeptical that "register" has impact on modern compilers.
	// Copying member variables into local did make a huge difference.
	register char_p p1 = p0;
	register char_p p2 = p0 + n0;
	register unsigned m = mask;
	register unsigned v = 0;
	register unsigned* t = table;
	while (p1 < p2) {
		v = (v << 1) ^ t[*p1++];
		if (0 == (m & v)) break;
	}
	return (int)(p1 - p0);
}
