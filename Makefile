CFLAGS = -Wall -Werror -O0 -ggdb

.PHONY: all run clean disas

all: svm
	$(MAKE) -C hello
	$(MAKE) -C dhrystone

clean:
	$(RM) svm
	$(MAKE) -C hello
	$(MAKE) -C dhrystone

svm: kvm.cpp mem.cpp cpu.cpp main.cpp helper.cpp
	$(CXX) $(CFLAGS) -o $@ $^

