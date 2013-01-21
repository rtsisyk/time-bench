#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

double
test_call(void)
{
	return 0.0;
}

double
test_memset(void)
{
	struct timespec t;
	memset(&t, 0, sizeof(t));
	return t.tv_sec * 1e9 + t.tv_nsec;
}

#define TEST_CLOCK(name, clockid)                                          \
double                                                                         \
test_clock_##name(void)                                                        \
{                                                                              \
	struct timespec t;                                                     \
	clock_gettime(clockid, &t);                                            \
	return t.tv_sec * 1e9 + t.tv_nsec;                                     \
}
TEST_CLOCK(realtime,         CLOCK_REALTIME)
TEST_CLOCK(monotonic,        CLOCK_MONOTONIC)
TEST_CLOCK(process_cpu,      CLOCK_PROCESS_CPUTIME_ID)
TEST_CLOCK(thread_cpu,       CLOCK_THREAD_CPUTIME_ID)
TEST_CLOCK(monotonic_raw,    CLOCK_MONOTONIC_RAW)
TEST_CLOCK(realtime_coarse,  CLOCK_REALTIME_COARSE)
TEST_CLOCK(monotonic_coarse, CLOCK_MONOTONIC_COARSE)
TEST_CLOCK(boottime,         CLOCK_BOOTTIME)
TEST_CLOCK(realtime_alarm,   CLOCK_REALTIME_ALARM)
TEST_CLOCK(boottime_alarm,   CLOCK_BOOTTIME_ALARM)
#undef TEST_CLOCK

double
test_gettimeofday(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 1e9 + t.tv_usec * 1e3;
}

double
test_time(void)
{
	return time(NULL);
}

struct test {
	const char *name;
	double (*fun)(void);
} TESTS[] = {
	{"CALL", test_call},
	{"MEMSET", test_memset},
	{"CLOCK_REALTIME", test_clock_realtime},
	{"CLOCK_MONOTONIC", test_clock_monotonic},
	{"CLOCK_PROCESS_CPUTIME_ID", test_clock_process_cpu},
	{"CLOCK_THREAD_CPUTIME_ID", test_clock_thread_cpu},
	{"CLOCK_MONOTONIC_RAW", test_clock_monotonic_raw},
	{"CLOCK_REALTIME_COARSE", test_clock_realtime_coarse},
	{"CLOCK_MONOTONIC_COARSE", test_clock_monotonic_coarse},
	{"CLOCK_BOOTTIME", test_clock_boottime},
	{"CLOCK_REALTIME_ALARM", test_clock_realtime_alarm},
	{"CLOCK_BOOTTIME_ALARM", test_clock_boottime_alarm},
	{"gettimeofday", test_gettimeofday},
	{"time", test_time},
};

int
main(int argc, char *argv[])
{
	long count = 1000000;
	if (argc > 1) {
		count = atol(argv[1]);
	}

	fprintf(stderr, "Count: %lu\n", count);
	for (size_t i = 0; i < sizeof(TESTS) / sizeof(*TESTS); i++) {
		double start = test_gettimeofday(), end;

		for (int c = 0; c < count; c++) {
			TESTS[i].fun();
		}

		end = test_gettimeofday();

		fprintf(stderr, "%-24s %.2lf ns/call\n", TESTS[i].name,
			(end - start) / count);
	}

	return 0;
}
