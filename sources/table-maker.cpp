//
// Generate a 256 place table of random values.
// The table of "random" values defeat simple exploits.
//

#include "common.h"
#include "ZRandom.h"

// Test for a "bad" table (though unlikely).
static bool table_is_good(unsigned* table, unsigned mask) {
	// A run of the same byte should not return zero.
	for (unsigned c=0; c<256; ++c) {
		unsigned v = 0;
		for (unsigned m=mask, n=1; m; m >>= 1, ++n) {
			v = (v << 1) ^ table[c];
			if (!(mask & v)) {
				printf("%d bytes of %d cause break\n", n, c);
				return false;
			}
		}
	}
	// TODO test multiple byte sequences...
	return true;
}

unsigned* table_prime(unsigned seed, unsigned mask) {
	unsigned* table = new unsigned[256];
	ZRandom generator(seed);
	for (int pass=0; pass<10; ++pass) {
		for (int i=0; i<256; ++i) {
			table[i] = generator.getValue();
		}
		if (table_is_good(table, mask)) {
			break;
		}
		printf("Bad table #%d!\n", pass);
	}
	return table;
}
