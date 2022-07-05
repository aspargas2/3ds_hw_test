#include "smalllib.h"

#include <limits.h>

size_t strlen(const char* str) {
	return strnlen(str, INT_MAX);
}

size_t strnlen(const char* str, size_t maxlen) {
	size_t len = 0;

	while (len < maxlen && str[len] != '\0') len++;

	return len;
}

char* strchr(const char* str, int c) {
	for (char ch = *str; ch != '\0'; ch = *(++str))
		if (ch == c)
			return (char*) str;

	return NULL;
}

int memcmp(const void* ptr1, const void* ptr2, size_t num) {
	const u8* p1 = ptr1;
	const u8* p2 = ptr2;

	for (; num != 0; num--) {
		u8 diff = *p1++ - *p2++;

		if (diff != 0)
			return diff;
	}

	return 0;
}

void utoahex(u32 x, char* buf) {
	bool started = false;

	for (size_t i = 0; i < sizeof(x) * 2; i++) {
		const u8 dig = x >> 28;

		if (started || dig != 0) {
			*buf++ = ((dig < 10) ? '0' : ('a' - 0xA)) + dig;
			started = true;
		}

		x <<= 4;
	}

	if (!started)
		*buf++ = '0';

	*buf = '\0';
}

// Compile for arm so __aeabi_uidiv_whatever can get inlined and optimized
void __attribute__((noinline, target("arm"))) utoadec(u32 x, char* buf) {
	bool started = false;
	u32 pow_10 = 1000000000UL;

	while (pow_10 != 0) {
		char dig = '0';

		while (true) {
			const u32 temp = x - pow_10;
			if (temp > x)
				break;
			x = temp;
			dig++;
		}

		if (started || dig != '0') {
			*buf++ = dig;
			started = true;
		}

		pow_10 /= 10;
	}

	if (!started)
		*buf++ = '0';

	*buf = '\0';
}

void itoadec(s32 x, char* buf) {
	if (x < 0) {
		*buf++ = '-';
		x = -x;
	}

	utoadec(x, buf);
}

int __attribute__((naked, target("arm"))) clz(u32 __attribute__((unused)) x) {
	asm("clz r0, r0");
	asm("bx lr");
}