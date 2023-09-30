CFLAGS = -std=c11 -Wall -Wextra -I$(UNITY) -I$(SRCDIR) -I.
CC = gcc

# Source code paths
SRCDIR := src
TESTSDIR := tests

# Build paths
BUILD := build
OBJDIR := build/objects
DEPDIR := build/dependencies
RESULTSDIR := build/results
RESULTS := $(patsubst $(TESTSDIR)/test_%.c,$(RESULTSDIR)/test_%.txt,$(wildcard $(TESTSDIR)/test_*.c))

BUILDPATHS := $(BUILD) $(OBJDIR) $(RESULTSDIR) $(DEPDIR)

# Unity testing library
UNITY := unity/src

# Declare all source files and object files to be created
SOURCES := $(wildcard *.c) $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SOURCES)))
TESTSOURCES := $(wildcard *.c) $(wildcard $(TESTSDIR)/*.c)
TESTOBJECTS := $(patsubst %.c, $(OBJDIR)/test_%.o, $(notdir $(TESTSOURCES)))

# Name of the program
TARGET = IFJ.out

# Compile the program (without tests)
$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) $^ -o $(BUILD)/$@

# Universal rule for source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@

# Make results
$(RESULTSDIR)/%.txt: $(BUILD)/%.out
	@-./$< > $@ 2>&1

# Universal rule for test source files
$(OBJDIR)/%.o: $(TESTSDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@

# Compile and link tests
$(BUILD)/test_%.out: $(OBJDIR)/test_%.o $(OBJDIR)/%.o $(OBJDIR)/unity.o
	$(CC) -o $@ $^

# Compile Unity
$(OBJDIR)/%.o: $(UNITY)/%.c $(UNITY)/%.h
	$(CC) -c $(CFLAGS) $< -o $@ 

# Create dependencies
$(DEPDIR)/%.d: $(TESTSDIR)/%.c
	$(CC) -MM -MG -MF $@ $<

# Create directories
$(OBJDIR):
	@mkdir $(OBJDIR)
$(BUILD):
	@mkdir $(BUILD)
$(DEPDIR):
	@mkdir $(DEPDIR)
$(RESULTSDIR):
	@mkdir $(RESULTSDIR)

test: $(BUILDPATHS) $(RESULTS)
	@echo -e "\n-----------------------\n\033[0;31mFAILURES:\n\033[0m-----------------------\033[0;31m"
	@find build/results -type f -name "*.txt" -exec grep -H 'FAIL' {} \; | head -n -1
#@echo -e `head -n-4 $(RESULTSDIR)/*.txt | column -t -s: | awk '/FAIL/ {print, next}'`
#@echo -e "\n-----------------------\n\033[33mIGNORES:\033[0m\n-----------------------\033[33"
#@echo -e `grep -s IGNORE $(RESULTSDIR)/*.txt` "\033[0m" 
	$(eval p=$(shell head -n-4 $(RESULTSDIR)/*.txt | grep -sF "PASS" | wc -l))
	$(eval f=$(shell head -n-4 $(RESULTSDIR)/*.txt | grep -sF "FAIL" | wc -l))
	$(eval t=$(shell head -n-4 $(RESULTSDIR)/*.txt | wc -l))
	@echo -e "\n\033[0;32m $(p) Passed\033[0m, \033[0;31m $(f) Failed\033[0m, $(t) Total"

run: $(BUILDPATHS) $(TARGET)
	$(BUILD)/$(TARGET)

clean:
	rm -f $(OBJDIR)/*.o $(BUILD)/*.out $(RESULTSDIR)/*.txt $(DEPDIR)/*.d 

print-%  : ; @echo $* = $($*) # for Makefile debugging - prints variable specified after print-

default: run

.PHONY: clean

.PRECIOUS: $(BUILD)/test_%.out
.PRECIOUS: $(DEPDIR)/%.d
.PRECIOUS: $(OBJDIR)/%.o
.PRECIOUS: $(RESULTSDIR)/%.txt
