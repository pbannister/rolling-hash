#ifndef ZROLLINGHASH_H_
#define ZROLLINGHASH_H_

//
//	Very simple rolling hash.
//	Aim is to be efficient, above all else.
//

class ZRollingHash {
private:
	unsigned* table;
	unsigned mask;

public:
	void prime(unsigned seed = 20160710, unsigned _mask = 0x3FFFFF);

public:
	// Returns the number of bytes scanned.
	int scanBuffer(char_p, int);
};

#endif /* ZROLLINGHASH_H_ */
