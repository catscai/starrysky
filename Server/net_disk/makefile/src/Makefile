# g++ make

TARGET=app
INCLUDE=-I../include
LIBS=-L../lib
CXXSRC=$(wildcard *.cpp)
CSRC=$(wildcard *.c)
SRC=$(CXXSRC) $(CSRC)
DES=$(patsubst %.c,%.o,$(CSRC)) $(patsubst %.cpp,%.o,$(CXXSRC))
CXX=g++
CXXFLAGS=-c

$(TARGET):$(DES)
	$(CXX) $(DES) $(LIBS) -lpthread -lmysqlcppconn -lcrypto -o $@ 

%.o:%.c
	$(CXX) $< $(INCLUDE)  $(CXXFLAGS)
%.o:%.cpp
	$(CXX) $< $(INCLUDE)  $(CXXFLAGS)

.PHONY:clean print 
clean:
	-rm -f *.o

print:
	@echo "SRC: $(SRC)"
	@echo "DES: $(DES)"

