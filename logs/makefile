test:test.cc util.hpp level.hpp message.hpp format.hpp sink.hpp logger.hpp
	g++ -g -o $@ $^ -std=c++11 -lpthread

.PHONY:clean
clean:
	rm -f test
	rm -rf logfile