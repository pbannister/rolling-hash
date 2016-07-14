//
// Generate hash lists for tests of de-duplicated storage.
// First iteration - time candidate rolling hash functions.
//

#include "common.h"
#include "ZRandom.h"
#include <getopt.h>
#include <time.h>

#include "ZRollingHash.h"
#include "ZTimer.h"

static int cbBuffer;
static char_p pBuffer;

static void buffer_prime(int cb = (1 << 28)) {
	printf("Prime buffer (%d MB)...\n", cb >> 20);

	ZTimer timer;

	cbBuffer = cb & ~3; // enforce multiple of 32-bits.
	pBuffer = new char_v[cbBuffer];

	ZRandom r;
	char_p p1 = pBuffer;
	char_p p2 = pBuffer + cbBuffer;
	while (p1 < p2) {
		unsigned v = r.getValue();
		*p1++ = 255 & v;
		v >>= 8;
		*p1++ = 255 & v;
		v >>= 8;
		*p1++ = 255 & v;
		v >>= 8;
		*p1++ = 255 & v;
	}
	int dt = timer.split();
	printf("%8d (ms) - prime buffer\n", dt);
}

static inline int min(int a, int b) {
	return (a < b) ? a : b;
}

template <class T> class ZScanner {
private:
	int nChunkMax;
	char_p pChunk;
	char_p pScan;
	char_p pEOB;
	T rh;

public:
	void prime(int nMax = (1 << 24)) {
		nChunkMax = nMax;
		rh.prime();
	}
	void start(char_p pBuffer, int cbBuffer) {
		pEOB = pBuffer + cbBuffer;
		pScan = pBuffer;
		pChunk = pScan;
	}
	int scan() {
		pChunk = pScan;
		int nLeft = (int)(pEOB - pScan);
		int nWant = min(nChunkMax, nLeft);
		int n1 = rh.scanBuffer(pScan, nWant);
		pScan += n1;
		return n1;
	}

public:
	bool more() {
		return pScan < pEOB;
	}

};

template <class T> void rate() {
	ZScanner<T> scanner;
	scanner.prime();

	printf("Scan buffer for at least 30 seconds...\n");
	int nPasses = 0;
	int nChunks = 0;
	long nChunksTotal = 0;

	time_t t1 = time(0);
	time_t t2 = t1;
	ZTimer timer;
	while (t2 < (t1 + 30)) {
		++nPasses;
		scanner.start(pBuffer, cbBuffer);
		while (scanner.more()) {
			int n = scanner.scan();
			++nChunks;
			nChunksTotal += n;
		}
		t2 = time(0);
	}

	int dt = timer.split();
	long rate = ((1000 * nChunksTotal) >> 20) / dt;
	printf("%8d (ms) - passes: %6d chunks: %6d total: %14ld average: %8ld rate: %6ld MB/s\n", dt, nPasses, nChunks, nChunksTotal, (nChunksTotal / nChunks), rate);
}

int main(int ac, char** av) {
	printf("sizeof char: %ld unsigned: %ld long: %ld char*: %ld\n", sizeof(char), sizeof(unsigned), sizeof(long), sizeof(char*));

	buffer_prime();

	printf("Prime rolling hash 1...\n");
	rate<ZRollingHash>();

//	printf("Prime rolling hash 2...\n");
//	rate<ZRollingHash2>();

	return 0;
}
