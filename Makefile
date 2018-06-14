# Linux

COMPILER = g++

RMDIR = rm -rf
RM = rm -f

DEP_FLAGS = -M -MT $@ -MT $(BIN_PATH)/$(*F).o -MP -MF $@

LIBS = -lSDL2 -lSDL2_image -lSDL2_mixer -lSDL2_ttf -lm

INC_PATHS = -I$(INC_PATH) $(addprefix -I,$(SDL_INC_PATH))

FLAGS = -std=c++11 -Wall -pedantic -Wextra -Wno-unused-parameter -Werror=init-self

DFLAGS = -ggdb -O0 -DDEBUG

RFLAGS = -03 -mtune=native

INC_PATH = include
SRC_PATH = src
BIN_PATH = bin
DEP_PATH = dep

CPP_FILES = $(wildcard $(SRC_PATH)/*.cpp)
INC_FILES = $(wildcard $(INC_PATH)/*.h)
FILE_NAMES = $(sort $(notdir $(CPP_FILES:.cpp=)) $(notdir $(INC_FILES:.h=)))
DEP_FILES = $(addprefix $(DEP_PATH)/, $(addprefix .d,$(FILE_NAMES)))
OBJ_FILES = $(addprefix $(BIN_PATH)/,$(notdir $(CPP_FILES:.cpp=.o)))


EXEC = 140039082_FabioMarques

# Caso for Windows

ifeq ($(OS), Windows_NT)

RMDIR = rd /s /q
RM = del /q

SDL_PATH = C:/SDL2-2.0.8

SDL_INC_PATH += $(addsuffix /include, $(SDL_PATH))
LINK_PATH = $(addprefix -L, $(addsuffix /lib, $(SDL_PATH)))
FLAGS += -mwindows
DFLAGS += -mconsole
LIBS := -lmingw32 -lSDL2main $(LIBS)


EXEC := $(EXEC).exe

else
UNAME_S := $(shell uname -s)

# Caso for Mac

ifeq ($(UNAME_S), Darwin)

LIBS = -lm -framework SDL2 -framework SDL2_image -framework SDL2_mixer -framework SDL2_ttf

endif
endif


# Regras de compilação

all: $(EXEC)

$(EXEC): $(OBJ_FILES)
	$(COMPILER) -o $@ $^ $(LINK_PATH) $(LIBS) $(FLAGS)


# Criação dos arquivos obejto caso eles não existam, um para cada arquivo "cpp"
$(BIN_PATH)/%.o: $(DEP_PATH)/%.d | folders
	$(COMPILER) $(INC_PATHS) $(addprefix $(SRC_PATH)/, $(notdir $(<:.d=.cpp))) -c $(FLAGS) -o $@
	
$(DEP_PATH)/%.d: $(SRC_PATH)/%.cpp | folders
	$(COMPILER) $(INC_PATHS) $< $(DEP_FLAGS) $(FLAGS)

#ifeq ($(OS), Windows_NT)
#	@if not exist $(DEP_PATH) @mkdir $(DEP_PATH)
#	@if not exist $(BIN_PATH) @mkdir $(BIN_PATH)
#else
#	@mkdir -p $(DEP_PATH) $(BIN_PATH)
#endif
#
#	$(CC) $(DEP_FLAGS) -c -o $@ $< $(DIRECTIVES)

-include $$(DEP_FILES)

clean:
	$(RMDIR) $(DEP_PATH)
	$(RMDIR) $(BIN_PATH)
	$(RM) $(EXEC)

.PRECIOUS: $(DEP_FILES)

.PHONY: debug clean release folders help

debug: FLAGS += $(DFLAGS)
debug: $(EXEC)

release: FLAGS += $(RFLAGS)
release: $(EXEC)

folders:
ifeq ($(OS), Windows_NT)
	@if NOT exist $(DEP_PATH) (mkdir $(DEP_PATH))
	@if NOT exist $(BIN_PATH) (mkdir $(BIN_PATH))
	@if NOT exist $(INC_PATH) (mkdir $(INC_PATH))
	@if NOT exist $(SRC_PATH) (mkdir $(SRC_PATH))
else
	@mkdir -p $(DEP_PATH) $(BIN_PATH) $(INC_PATH) $(SRC_PATH)
endif
	
print-% : ; @echo $* = $($*)

help:
ifeq ($(OS), Windows_NT)
	@echo.
endif
	@echo Avaiable targets:
	@echo - release: Builds the release version
	@echo - debug: Builds the debug version
	@echo - clean : Cleans generated files
	@echo - folders: Generates project directories
	@echo - help: Shows this help
ifeq ($(OS), Windows_NT)
	@echo.
endif