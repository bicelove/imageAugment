
# compiler
CXX :=g++ -Wall -w -O3 -msse2 -fPIC -ggdb -pthread -std=c++11
CC :=gcc -lm -Wall -w -O3 -msse2 -fPIC -ggdb -pthread -std=c++11

#CXX :=g++ -fPIC 
#CC :=gcc -fPIC 

#shared library
SFLAG := -fPIC -shared

# sources
APICOLORLEVEL_DIR = ./src/API_colorLevel
APICOLORLEVEL_INC = -I$(APICOLORLEVEL_DIR)
APICOLORLEVEL_SRC = $(APICOLORLEVEL_DIR)/API_colorLevel.cpp

APICONTRAST_DIR = ./src/API_imgContrast
APICONTRAST_INC = -I$(APICONTRAST_DIR)
APICONTRAST_SRC = $(APICONTRAST_DIR)/API_imgContrast.cpp

APIBRIGHTNESS_DIR = ./src/API_imgBrightness
APIBRIGHTNESS_INC = -I$(APIBRIGHTNESS_DIR)
APIBRIGHTNESS_SRC = $(APIBRIGHTNESS_DIR)/API_imgBrightness.cpp

# include && lib
INCLUDE = -I/usr/local/include/opencv -I/usr/local/include/opencv2 -I/usr/local/include -I/usr/local/cuda/include
LIBARAY = -L./bin -L/usr/local/lib -L/usr/local/lib64 -L/usr/local/cuda/lib64 -L/usr/lib -L/usr/lib64

# flags
CFLAGS =  -fopenmp  -DHAVE_JPEG -DHAVE_PNG
CXXFLAGS = -fopenmp  -DHAVE_JPEG -DHAVE_PNG \
	 $(APICOLORLEVEL_INC) $(APICONTRAST_INC) $(APIBRIGHTNESS_INC) $(INCLUDE) 
#$(APIEXPAND_INC)

LDFLAGS  = -lm -lpthread -lopencv_core -lopencv_highgui -lopencv_imgproc -lopencv_ml
LDFLAGS  += -lcublas_device -lcublas -lcudadevrt -lcudart  -lcufft -lcufftw -lculibos -lcurand -lcusparse -lnppc -lnppi -lnpps
LDFLAGS  += -ldl -lrt -ljpeg -lpng  -lz  -lstdc++ -lglog -lboost_system

# -lopenblas
# -lcurl

SRCS = $(APICOLORLEVEL_SRC) $(APICONTRAST_SRC) $(APIBRIGHTNESS_SRC)
OBJS = $(SRCS:%.cpp=%.o)
OBJECTS := $(OBJS)

# BIN && LIB
BIN = ./bin
SRC = ./src

###### generation rules
#.SUFFIXES: .cpp .c .o .so .a .d
.c.o:
	@echo ----------------------------------------------
	$(CC) $(CFLAGS) -c $< -o $@
	@echo ----------------------------------------------

.cpp.o:
	@echo ----------------------------------------------
	$(CXX) $(CXXFLAGS) -c $< -o $@
	@echo ----------------------------------------------


###### main
#.PHONY : all 
all: lib Demo_expandImg

#deps: $(addprefix $(OBJSPATH),$(DEPS))
Demo_expandImg:$(BIN) $(OBJECTS)
	@echo ----------------------------------------------
	$(CXX) $(CXXFLAGS) $(SRC)/Demo_expandImg.cpp  -o $(BIN)/Demo_expandImg -lmainboby $(LIBARAY) $(LDFLAGS)
	@echo ----------------------------------------------

lib: $(OBJECTS)
	@echo ----------------------------------------------
	$(CXX) $(CXXFLAGS) -shared -o -fPIC -o $(BIN)/libmainboby.so $(OBJECTS) $(LIBARAY) $(LDFLAGS)
	@echo ----------------------------------------------

clean:
	rm -f $(OBJECTS)
	rm -f $(TARGET)
	rm $(BIN)/libmainboby
	rm -f *.o 
	rm $(BIN)/Demo_expandImg

rebuild: clean all
install:
	install -T $(TARGET) $(INSTALL_PATH)/bin/$(TARGET)
