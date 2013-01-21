#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <assert.h>
#include <string.h>

double
stub_call(void)
{
	return 0.0;
}

double
stub_memset(void)
{
	struct timespec t;
	memset(&t, 0, sizeof(t));
	return t.tv_sec * 1e9 + t.tv_nsec;
}

double
now_gettimeofday(void)
{
	struct timeval t;
	gettimeofday(&t, NULL);
	return t.tv_sec * 1e9 + t.tv_usec * 1e3;
}

double
now_time(void)
{
	return time(NULL);
}

struct test {
	const char *name;
	clockid_t clk_id;
} tests[] = {

	{"CLOCK_REALTIME",  CLOCK_REALTIME},
#if defined(CLOCK_REALTIME_COARSE)
	{"CLOCK_REALTIME_COARSE", CLOCK_REALTIME_COARSE},
#endif /* defined(CLOCK_REALTIME_COARSE) */
#if defined(CLOCK_REALTIME_ALARM)
	{"CLOCK_REALTIME_ALARM", CLOCK_REALTIME_ALARM},
#endif /* defined(CLOCK_REALTIME_ALARM) */
#if defined(CLOCK_REALTIME_PRECISE)
	{"CLOCK_REALTIME_PRECISE", CLOCK_REALTIME_PRECISE},
#endif /* defined(CLOCK_REALTIME_PRECISE) */
#if defined(CLOCK_REALTIME_FAST)
	{"CLOCK_REALTIME_FAST", CLOCK_REALTIME_FAST},
#endif /* defined(CLOCK_REALTIME_FAST) */

	{"CLOCK_MONOTONIC", CLOCK_MONOTONIC},
#if defined(CLOCK_MONOTONIC_COARSE)
	{"CLOCK_MONOTONIC_COARSE", CLOCK_MONOTONIC_COARSE},
#endif /* defined(CLOCK_MONOTONIC_COARSE) */
#if defined(CLOCK_MONOTONIC_RAW)
	{"CLOCK_MONOTONIC_RAW", CLOCK_MONOTONIC_RAW},
#endif /* defined(CLOCK_MONOTONIC_RAW) */
#if defined(CLOCK_MONOTONIC_PRECISE)
	{"CLOCK_MONOTONIC_PRECISE", CLOCK_MONOTONIC_PRECISE},
#endif /* defined(CLOCK_MONOTONIC_PRECISE) */
#if defined(CLOCK_MONOTONIC_FAST)
	{"CLOCK_MONOTONIC_FAST", CLOCK_MONOTONIC_FAST},
#endif /* defined(CLOCK_MONOTONIC_FAST) */


#if defined(CLOCK_PROCESS_CPUTIME_ID)
	{"CLOCK_PROCESS_CPUTIME_ID", CLOCK_PROCESS_CPUTIME_ID},
#endif /* defined(CLOCK_PROCESS_CPUTIME_ID) */
#if defined(CLOCK_THREAD_CPUTIME_ID)
	{"CLOCK_THREAD_CPUTIME_ID", CLOCK_THREAD_CPUTIME_ID},
#endif /* defined(CLOCK_THREAD_CPUTIME_ID) */

#if defined(CLOCK_BOOTTIME)
	{"CLOCK_BOOTTIME", CLOCK_BOOTTIME},
#endif /* defined(CLOCK_BOOTTIME) */
#if defined(CLOCK_BOOTTIME_ALARM)
	{"CLOCK_BOOTTIME_ALARM", CLOCK_BOOTTIME_ALARM},
#endif /* defined(CLOCK_BOOTTIME_ALARM) */
#if defined(CLOCK_UPTIME_PRECISE)
	{"CLOCK_UPTIME_PRECISE", CLOCK_UPTIME_PRECISE},
#endif /* defined(CLOCK_UPTIME_PRECISE) */
#if defined(CLOCK_UPTIME_FAST)
	{"CLOCK_UPTIME_FAST", CLOCK_UPTIME_FAST},
#endif /* defined(CLOCK_UPTIME_FAST) */

#if defined(CLOCK_SECOND)
	{"CLOCK_SECOND", CLOCK_SECOND},
#endif /* defined(CLOCK_SECOND) */
#if defined(CLOCK_PROF)
	{"CLOCK_PROF", CLOCK_PROF},
#endif /* defined(CLOCK_SECOND) */
};

static clockid_t clock_id;

bool
check_clock_gettime(void)
{
	struct timespec t;
	return clock_gettime(clock_id, &t) == 0;
}

double
now_clock_gettime(void)
{
	struct timespec t;
	clock_gettime(clock_id, &t);
	return t.tv_sec * 1e9 + t.tv_nsec;
}

double
bench_begin()
{
	return now_gettimeofday();
}

void
bench_end(double start, const char *name, long count)
{
	double end = now_gettimeofday();
	fprintf(stderr, "%-24s %.2lf ns/call\n", name,
		(end - start) / count);
}

int
main(int argc, char *argv[])
{
	long count = 1000000;
	if (argc > 1) {
		count = atol(argv[1]);
	}

	fprintf(stderr, "Count: %lu\n", count);

	double start;

	start = bench_begin();
	for (int c = 0; c < count; c++) {
		stub_call();
	}
	bench_end(start, "call", count);

	start = bench_begin();
	for (int c = 0; c < count; c++) {
		stub_memset();
	}
	bench_end(start, "memset", count);

	start = bench_begin();
	for (int c = 0; c < count; c++) {
		now_gettimeofday();
	}
	bench_end(start, "gettimeofday", count);

	start = bench_begin();
	for (int c = 0; c < count; c++) {
		now_time();
	}
	bench_end(start, "time", count);

	/* clock_gettime benchmarks */
	for (size_t i = 0; i < sizeof(tests) / sizeof(*tests); i++) {
		clock_id = tests[i].clk_id;
		if (!check_clock_gettime())
			continue;

		start = bench_begin();
		for (int c = 0; c < count; c++) {
			now_clock_gettime();
		}
		bench_end(start, tests[i].name, count);

	}

	return 0;
}
