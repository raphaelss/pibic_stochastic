import random
import sys

def transpose(s, n):
    for i in range(len(s)):
        s[i] = (s[i] + n) % 12

def retrograde(s):
    s.reverse()

def invert(s):
    for i in range(len(s)):
        s[i] = (12 - s[i]) % 12

def retrograde_invert(s):
    invert(s)
    retrograde(s)

def next_series(s):
    r = random.randint(0,3)
    if r == 1:
        retrograde(s)
    elif r == 2:
        invert(s)
    elif r == 3:
        retrograde_invert(s)
    transpose(s, random.randint(0,11))

class series_handler:
    def __init__(self, s, min_n, max_n):
        self.series = s
        next_series(self.series)
        self.min_n = min_n
        self.max_n = max_n
        self.i = 0
        self.n = len(series)

    def get_pitches(self):
        n = random.randint(self.min_n, self.max_n)
        pitches = []
        for i in range(n):
            if self.i == self.n:
                next_series(self.series)
                self.i = 0
            pitches.append(self.series[self.i])
            self.i = self.i + 1
        return pitches

def make_dur_list(target_dur, mean):
    t = 0
    dur_list = []
    while t < target_dur:
        dur = random.expovariate(1/mean)
        dur_list.append(dur)
        t = t + dur
    return dur_list

if len(sys.argv) <  7:
    exit()
total_dur = float(sys.argv[1])
mean_sec_dur = float(sys.argv[2])
mean_sec_dens = float(sys.argv[3])
min_n = int(sys.argv[4])
max_n = int(sys.argv[5])
series = []
for i in sys.argv[6:]:
    series.append(int(i))
print(series)
sections = make_dur_list(total_dur, mean_sec_dur)
sh = series_handler(series, min_n, max_n)
n = 1
for sec in sections:
    print("SEÇÃO {} - dur {}:".format(n, sec))
    events = make_dur_list(sec, sec / mean_sec_dens)
    for ev in events:
        chord = sh.get_pitches()
        print("- Duração: {}; acorde: {}".format(ev, chord))
    print("\n---\n")
    n = n + 1
