CC = g++ -std=c++11
DOMAIN_FILES = main.cpp bolinha.cpp linha.cpp aviao.cpp projetil.cpp bomba.cpp
TINYXML_FOLDER = tinyxml
TINYXML_FILES = $(TINYXML_FOLDER)/tinyxml.cpp 		\
				$(TINYXML_FOLDER)/tinyxmlparser.cpp \
				$(TINYXML_FOLDER)/tinyxmlerror.cpp 	\
				$(TINYXML_FOLDER)/tinystr.cpp
OUTPUT_FILE = trabalhocg
FLAGS = -lGL -lGLU -lglut -lm

all: g++
	@echo "Done. To run it, execute './trabalhocg [path to config.xml folder]'"

g++:
	@$(CC) $(DOMAIN_FILES) $(TINYXML_FILES) -o $(OUTPUT_FILE) $(FLAGS)

clean:
	@rm -f $(OUTPUT_FILE)
