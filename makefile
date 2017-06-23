.PHONY: all obj_folder clean

EXECUTABLE	=	disemu
CC		=	g++
FLAGS		=	-std=c++17 -Wall -Wextra -ggdb
SRC_PATH	=	./src
OBJ_PATH	=	./obj

SOURCES		:=	$(wildcard $(SRC_PATH)/base/third-party/Adafruit-GFX-Library/*.cpp $(SRC_PATH)/base/third-party/*.cpp $(SRC_PATH)/base/*.cpp $(SRC_PATH)/*.cpp)
OBJECTS		:=	$(SOURCES:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)

LIBS		:=	-lstdc++ -lm -lGL -lGLU -lGLEW -lglfw -lX11 -lXxf86vm -lXrandr -lpthread -lXi -ldl -lXinerama -lXcursor

USER_SRC	:=	$(wildcard $(SRC_PATH)/user/*.cpp)
USER_OBJ	:=	$(USER_SRC:$(SRC_PATH)/%.cpp=$(OBJ_PATH)/%.o)
USER_EXE	:=	user_main

$(EXECUTABLE):	$(OBJECTS)
	@echo "Linking..."
	$(CC) -o $@ $(OBJECTS) $(LIBS) $(FLAGS) 
	@echo "Done."

$(OBJECTS): $(OBJ_PATH)/%.o: $(SRC_PATH)/%.cpp
	@echo "Compiling..."
	$(CC) -I src/base/third-party/ -I src/base/third-party/Adafruit-GFX-Library/ -fPIC -c -o $@ $< $(FLAGS)

$(USER_EXE): $(USER_SRC)
	@echo "Compiling user main..."
	$(CC) -I src/ -I src/base/third-party/ -I src/base/third-party/Adafruit-GFX-Library/ -fPIC -c -o obj/$@.o $(USER_SRC) $(LIBS) $(FLAGS)
	$(CC) -I src/ -I src/base/third-party/ -I src/base/third-party/Adafruit-GFX-Library/ $(OBJECTS) obj/$@.o -shared -o $@.so $(LIBS) $(FLAGS)
	@echo "Done."


.PHONY: clean
clean: obj_folder
	rm -rf obj/*.o
	rm -rf obj/*/*.o
	rm -rf obj/*/*/*.o
	rm -rf obj/*/*/*/*.o
	rm -f $(USER_EXE).so
	rm -f $(EXECUTABLE)
	@echo "Cleaned!"
	


obj_folder:
	mkdir -p obj/base/third-party/Adafruit-GFX-Library
	mkdir -p obj/base/third-party
	mkdir -p obj/base
	mkdir -p obj
