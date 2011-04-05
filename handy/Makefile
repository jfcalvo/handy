UNAME := $(shell uname)
OS = os_issues
EXEC = tests/handytests
DEBUG = -DDEBUG
HANDYTESTS_OBJECTS = tests/handytests.o
HANDY_OBJECTS = handy_allograph.o handy_collection.o handy_normalize.o handy_serialize.o handy_distance.o handy_recognize.o
HANDY_HEADERS = src/handy_types.h src/handy_consts.h
HANDY_LIB = libhandy.so
LOGGING_HEADERS = ../logging/src
LOGGING_OBJECTS = ../logging/logging.o

all: $(OS) $(EXEC)

# No Debug Mode
ifeq ($(debug), false)
DEBUG = -DNODEBUG
endif

os_issues: 
# Operating System issues
ifeq ($(UNAME), Linux)
SONAME = -soname,$(HANDY_LIB)
endif
ifeq ($(UNAME), Darwin)
SONAME = -install_name,@rpath/$(HANDY_LIB)
endif

# Handy Tests

$(EXEC): $(HANDYTESTS_OBJECTS) $(HANDY_LIB)
	gcc -Wl,-rpath,.. -L. -lhandy -lm $(HANDYTESTS_OBJECTS) $(LOGGING_OBJECTS) -o $(EXEC)

$(HANDYTESTS_OBJECTS): tests/handytests.c
	gcc $(DEBUG) -c -Isrc -I$(LOGGING_HEADERS) tests/handytests.c -o $(HANDYTESTS_OBJECTS)

# Handy

$(HANDY_LIB): $(HANDY_HEADERS) $(HANDY_OBJECTS)
	gcc $(DEBUG) -shared -Wl,$(SONAME) -o libhandy.so $(HANDY_OBJECTS) $(LOGGING_OBJECTS)

handy_allograph.o: $(HANDY_HEADERS) src/handy_allograph.c src/handy_allograph.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_allograph.c -o handy_allograph.o
	
handy_collection.o: $(HANDY_HEADERS) src/handy_collection.c src/handy_collection.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_collection.c -o handy_collection.o
	
handy_normalize.o: $(HANDY_HEADERS) src/handy_normalize.c src/handy_normalize.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_normalize.c -o handy_normalize.o
	
handy_serialize.o: $(HANDY_HEADERS) src/handy_serialize.c src/handy_serialize.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_serialize.c -o handy_serialize.o

handy_distance.o: $(HANDY_HEADERS) src/handy_distance.c src/handy_distance.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_distance.c -o handy_distance.o
	
handy_recognize.o: $(HANDY_HEADERS) src/handy_recognize.c src/handy_recognize.h
	gcc $(DEBUG) -c -I$(LOGGING_HEADERS) -fPIC src/handy_recognize.c -o handy_recognize.o
	
# Utilities
	
clean:
	rm -f $(HANDY_LIB)
	rm -f $(HANDY_OBJECTS)
	rm -f $(HANDYTESTS_OBJECTS)
	rm -f $(EXEC)

