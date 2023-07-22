src = $(wildcard  ./*.cpp)
exe = bwtsearch

myArgs = -Wall -g -O3 -std=c++11

$(exe): $(src)
	g++ $(myArgs) $(src) -o $(exe)

.PHONY: clean

clean:
	rm -f $(exe)