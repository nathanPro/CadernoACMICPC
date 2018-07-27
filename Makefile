fuzz_time = 10

.PHONY: fft sa test

test: sa fft

sa:
	cd suffix_array && $(MAKE) test
	./suffix_array/fuzz -max_total_time=$(fuzz_time)
fft:
	cd fft && $(MAKE) test
	./fft/fuzz -max_total_time=$(fuzz_time)
