CXX := g++
CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -std=c++17

# Global settings
RESULTS_DIR = results
INPUTS_DIR = inputs
EASY_INPUT = inputs/easy
HARD_INPUT = inputs/hard

# checkpoint_1 settings
CP_1_DIR := checkpoint_1
CP_1_SRC := checkpoint_1.cpp
CP_1_EXE := checkpoint_1.exe
CP_1_OUT := checkpoint_1.out.txt
CP_1_OUT_HARD := checkpoint_1_hard.out.txt

# checkpoint_2 settings
CP_2_DIR := checkpoint_2
CP_2_SRC := checkpoint_2.cpp
CP_2_EXE := checkpoint_2.exe
CP_2_OUT := checkpoint_2.out.txt
CP_2_OUT_HARD := checkpoint_2_hard.out.txt

info:
	$(info all)
	$(info clean-checkpoint-1)
	$(info build-checkpoint-1)
	$(info run-checkpoint-1)
	$(info run-checkpoint-1-hard)
	$(info clean-checkpoint-2)
	$(info build-checkpoint-2)
	$(info run-checkpoint-2)
	$(info run-checkpoint-2-hard)

all: run-checkpoint-1 run-checkpoint-1-hard run-checkpoint-2 run-checkpoint-2-hard

clean-checkpoint-1:
	rm -f ./$(CP_1_DIR)/$(RESULTS_DIR)/$(CP_1_EXE)
	rm -f ./$(CP_1_DIR)/$(RESULTS_DIR)/$(CP_1_OUT)
	rm -f ./$(CP_1_DIR)/$(RESULTS_DIR)/$(CP_1_OUT_HARD)

build-checkpoint-1:
	cd $(CP_1_DIR) && $(CXX) $(CXX_FLAGS) $(CP_1_SRC) -o $(RESULTS_DIR)/$(CP_1_EXE)

run-checkpoint-1: build-checkpoint-1
	cd $(CP_1_DIR) && { time ./$(RESULTS_DIR)/$(CP_1_EXE) --folder ../$(EASY_INPUT) ; } 2>&1 | tee $(RESULTS_DIR)/$(CP_1_OUT)

run-checkpoint-1-hard: build-checkpoint-1
	cd $(CP_1_DIR) && { time ./$(RESULTS_DIR)/$(CP_1_EXE) --folder ../$(HARD_INPUT) ; } 2>&1 | tee $(RESULTS_DIR)/$(CP_1_OUT_HARD)

########################################################################################################################

clean-checkpoint-2:
	rm -f ./$(CP_2_DIR)/$(RESULTS_DIR)/$(CP_2_EXE)
	rm -f ./$(CP_2_DIR)/$(RESULTS_DIR)/$(CP_2_OUT)
	rm -f ./$(CP_2_DIR)/$(RESULTS_DIR)/$(CP_2_OUT_HARD)

build-checkpoint-2:
	cd $(CP_2_DIR) && $(CXX) $(CXX_FLAGS) $(CP_2_SRC) -o $(RESULTS_DIR)/$(CP_2_EXE)

run-checkpoint-2: build-checkpoint-2
	cd $(CP_2_DIR) && { time ./$(RESULTS_DIR)/$(CP_2_EXE) --folder ../$(EASY_INPUT) ; } 2>&1 | tee $(RESULTS_DIR)/$(CP_2_OUT)

run-checkpoint-2-hard: build-checkpoint-2
	cd $(CP_2_DIR) && { time ./$(RESULTS_DIR)/$(CP_2_EXE) --folder ../$(HARD_INPUT) ; } 2>&1 | tee $(RESULTS_DIR)/$(CP_2_OUT_HARD)
