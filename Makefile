fuzz_runs = 2048

.PHONY: fft sa test

test: sa fft
	rm fuzz.out
	rm **/fuzz

sa:
	cd suffix_array && $(MAKE) test
	./suffix_array/fuzz -runs=$(fuzz_runs) 2>> fuzz.out
fft:
	cd fft && $(MAKE) test
	./fft/fuzz -runs=$(fuzz_runs) 2>> fuzz.out
