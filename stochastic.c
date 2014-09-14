#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define SERIES_SIZE 12

struct series {
	unsigned index;
	unsigned elems [SERIES_SIZE];
};

struct chord {
	unsigned size;
	unsigned elems [SERIES_SIZE];
};

unsigned rand_range(unsigned min, unsigned max);
double exp_rnd(double mean);
void transpose(struct series *s, unsigned x);
void retrograde(struct series *s);
void invert(struct series *s);
void next_series(struct series *s);
void next_chord(struct chord *c, struct series *s, unsigned min_n,
                unsigned max_n);

int main(int argc, char** argv)
{
	srand(time(0));
	double total_dur = 360;
	double mean_sec_dur = 50;
	double mean_sec_dens = 10;
	struct series series = {0, {0,1,2,3,4,5,6,7,8,9,10,11}};
	unsigned min_n_chord = 1, max_n_chord = 6;
	struct chord chord;
	unsigned sec_n= 1;
	double sec_dur, mean_ev_dur, ev_dur;
	while(total_dur > 0) {
		sec_dur = exp_rnd(mean_sec_dur);
		mean_ev_dur = sec_dur / mean_sec_dens;
		printf("SEÇÃO %u:\n", sec_n);
		total_dur -= sec_dur;
		++sec_n;
		while(sec_dur > 0) {
			ev_dur = exp_rnd(mean_ev_dur);
			printf("Duração %.2f; alturas: ", ev_dur);
			next_chord(&chord, &series, min_n_chord, max_n_chord);
			for(unsigned i = 0; i < chord.size; ++i) {
				printf("%u ", chord.elems[i]);
			}
			puts("");
			sec_dur -= ev_dur;
		}
	}
	return 0;
}

unsigned rand_range(unsigned min, unsigned max)
{
	int r;
	const unsigned int range = max - min;
	const unsigned int buckets = RAND_MAX / range;
	const unsigned int limit = buckets * range;
	do {
		r = rand();
	} while (r >= limit);
	return min + (r / buckets);
}

double exp_rnd(double mean)
{
	double r = rand() / (double) RAND_MAX;
	return log(1 - r) / (-1 / mean);
}

void transpose(struct series *s, unsigned x)
{
	for(unsigned i = 0; i < SERIES_SIZE; ++i) {
		s->elems[i] = (s->elems[i] + x) % 12;
	}
}

void retrograde(struct series *s)
{
	unsigned tmp;
	unsigned *begin = s->elems, *end = s->elems + SERIES_SIZE;
	while(begin > end) {
		tmp = *begin;
		*begin = *end;
		*end = tmp;
	}
}

void invert(struct series *s)
{
	for(unsigned i = 0; i < SERIES_SIZE; ++i) {
		s->elems[i] = (12 - s->elems[i]) % 12;
	}
}

void next_series(struct series *s)
{
	switch(rand_range(0, 4)) {
	case 0:
		break;
	case 1:
		retrograde(s);
		break;
	case 2:
		invert(s);
		break;
	case 3:
		invert(s);
		retrograde(s);
		break;
	}
	transpose(s, rand_range(0, 12));
}

void next_chord(struct chord *c, struct series *s, unsigned min_n,
                unsigned max_n)
{
	c->size = rand_range(min_n, max_n);
	for(unsigned i = 0; i < c->size; ++i) {
		if(s->index == SERIES_SIZE) {
			next_series(s);
			s->index = 0;
		}
		c->elems[i] = s->elems[s->index++];
	}
}

