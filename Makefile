CFLAGS = -Wall -Werror -O0 -ggdb

.PHONY: all run clean disas

all: svm
	$(MAKE) -C hello all
	$(MAKE) -C dhrystone all

clean:
	$(RM) svm
	$(MAKE) -C hello clean
	$(MAKE) -C dhrystone clean

svm: kvm.cpp mem.cpp cpu.cpp main.cpp helper.cpp
	$(CXX) $(CFLAGS) -o $@ $^

