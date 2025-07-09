DEPS = ./ICUReadFile.cpp ./ICUMapping.cpp ./ICUNumberConverting.cpp \
		./ICUDictionary.cpp  ./SpecialCase.cpp  ./Address.cpp \
		./Math.cpp ./DateTime.cpp 

MAIN = main.cpp
TEST = test.cpp

main: $(DEPS) $(MAIN) makefile
	c++ -std=c++17 -o main $(MAIN) $(DEPS) \
		-lpthread -lm \
		-L../lib \
		-licudt -licuin -licuio -licuuc \

test: $(DEPS) $(TEST) makefile ./ICUHelper.h
	c++ -std=c++17 -o test $(TEST) $(DEPS) \
		-lpthread -lm \
		-L../lib \
		-licudt -licuin -licuio -licuuc \

clean:
	rm -f main log output.txt input.txt

