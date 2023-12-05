CFLAGS = -std=c11 -Wall -Wextra -Werror=implicit-function-declaration -I$(UNITY) -I$(SRCDIR) -I. -g -DUNITY_INCLUDE_DOUBLE
CC = gcc
DEFINE = -DRELEASE

# Source code paths
SRCDIR := src
TESTSDIR := tests

# Build paths
BUILD := build
OBJDIR := build/objects
OBJDIR_TOOLS := build/objects/tools
DEPDIR := build/dependencies
RESULTSDIR := build/results
RESULTS := $(patsubst $(TESTSDIR)/test_%.c,$(RESULTSDIR)/test_%.txt,$(wildcard $(TESTSDIR)/test_*.c))
HEADERS := $(wildcard include/*.h)

BUILDPATHS := $(BUILD) $(OBJDIR) $(RESULTSDIR) $(DEPDIR) $(HEADERS) $(OBJDIR_TOOLS)

# Unity testing library
UNITY := unity/src

# Declare all source files and object files to be created
SOURCES := $(wildcard *.c) $(wildcard $(SRCDIR)/*.c)
OBJECTS := $(patsubst %.c, $(OBJDIR)/%.o, $(notdir $(SOURCES)))
OBJECTS_WITHOUT_MAIN := $(filter-out $(OBJDIR)/main.o,$(OBJECTS))
TESTSOURCES := $(wildcard *.c) $(wildcard $(TESTSDIR)/*.c)
TESTOBJECTS := $(patsubst %.c, $(OBJDIR)/test_%.o, $(notdir $(TESTSOURCES)))

# Name of the program
TARGET = IFJ.out

# Compile the program (without tests)
$(BUILD)/$(TARGET): $(OBJECTS) $(HEADERS)
	$(CC) $(CFLAGS) $(OBJECTS) -o $@

# Universal rule for source files
$(OBJDIR)/%.o: $(SRCDIR)/%.c | $(OBJDIR)
	$(CC) -c $(CFLAGS) $< -o $@ -Iinclude $(DEFINE)

# Make results
$(RESULTSDIR)/%.txt: $(BUILD)/%.out
	@-./$< > $@ 2>&1

# Universal rule for test source files
$(OBJDIR)/%.o: $(TESTSDIR)/%.c
	$(CC) -c $(CFLAGS) $< -o $@ -Iinclude $(DEFINE)

# Compile and link tests
$(BUILD)/test_%.out: $(OBJDIR)/test_%.o $(OBJDIR)/%.o $(OBJDIR)/unity.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) -o $@ $^

$(BUILD)/test_scanner.out: $(OBJDIR)/test_scanner.o $(OBJDIR)/scanner.o $(OBJDIR)/unity.o $(OBJDIR)/string.o $(OBJDIR)/symtable.o
	$(CC) -o $@ $^

# Compile Unity
$(OBJDIR)/%.o: $(UNITY)/%.c $(UNITY)/%.h
	$(CC) -c $(CFLAGS) $< -o $@

# Create dependencies
$(DEPDIR)/%.d: $(TESTSDIR)/%.c
	$(CC) -MM -MG -MF $@ $<

# Create tools
.PHONY: tools
tools: $(BUILD)/tokenizer $(BUILD)/ir_gen $(BUILD)/ast_gen

$(BUILD)/tokenizer: $(OBJDIR_TOOLS)/tokenizer.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) -o $@ $^

$(BUILD)/ir_gen: $(OBJDIR_TOOLS)/ir_gen.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) -o $@ $^

$(BUILD)/ast_gen: $(OBJDIR_TOOLS)/ast_gen.o $(OBJECTS_WITHOUT_MAIN)
	$(CC) -o $@ $^

$(OBJDIR_TOOLS)/%.o: tools/%.c
	$(CC) -c -o $@ $< -Iinclude $(DEFINE)

# Create directories
$(OBJDIR):
	@mkdir $(OBJDIR)
$(OBJDIR_TOOLS):
	@mkdir $(OBJDIR_TOOLS)
$(BUILD):
	@mkdir $(BUILD)
$(DEPDIR):
	@mkdir $(DEPDIR)
$(RESULTSDIR):
	@mkdir $(RESULTSDIR)

test: $(BUILDPATHS) $(RESULTS)
	@echo -e "\n-----------------------\n\033[0;31mFAILURES:\n\033[0m-----------------------\033[0;31m"
	@find build/results -type f -name "*.txt" -exec grep -H 'FAIL' {} \;
#@echo -e `head -n-4 $(RESULTSDIR)/*.txt | column -t -s: | awk '/FAIL/ {print, next}'`
#@echo -e "\n-----------------------\n\033[33mIGNORES:\033[0m\n-----------------------\033[33"
#@echo -e `grep -s IGNORE $(RESULTSDIR)/*.txt` "\033[0m" 
	$(eval p=$(shell head -n-4 $(RESULTSDIR)/*.txt | grep -sF "PASS" | wc -l))
	$(eval f=$(shell head -n-4 $(RESULTSDIR)/*.txt | grep -sF "FAIL" | wc -l))
	$(eval t=$(shell expr $(p) + $(f)))
	@echo -e "\n\033[0;32m$(p) Passed\033[0m,\033[0;31m $(f) Failed\033[0m, $(t) Total"

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
