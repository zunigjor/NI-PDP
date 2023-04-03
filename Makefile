# Global settings
CXX := g++
# Results and inputs
RESULTS_DIR := results
INPUTS_DIR := inputs
EASY_INPUT := inputs/easy
HARD_INPUT := inputs/hard
# Profiler settings
PROFILER := /usr/lib/libprofiler.so
PROFILER_FILE := profiler.prof
PROFILER_FREQ := 100000
PROFILER_INPUT := inputs/hard/graf_20_16.txt
PROFILER_PDF := profiler.pdf

# checkpoint_1 settings
CP_1_CXX_FLAGS := -pedantic -Wall -Wextra -O3 -std=c++17
CP_1 := checkpoint_1
CP_1_SRC := $(CP_1).cpp
CP_1_EXE := $(CP_1).exe
CP_1_OUT_EASY := $(CP_1)_easy.out.txt
CP_1_OUT_HARD := $(CP_1)_hard.out.txt

# checkpoint_2 settings
CP_2_CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -O3 -std=c++17
CP_2 := checkpoint_2
CP_2_SRC := $(CP_2).cpp
CP_2_EXE := $(CP_2).exe
CP_2_OUT_EASY := $(CP_2)_easy.out.txt
CP_2_OUT_HARD := $(CP_2)_hard.out.txt

# checkpoint_3 settings
CP_3_CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -O3 -std=c++17
CP_3 := checkpoint_3
CP_3_SRC := $(CP_3).cpp
CP_3_EXE := $(CP_3).exe
CP_3_OUT_EASY := $(CP_3)_easy.out.txt
CP_3_OUT_HARD := $(CP_3)_hard.out.txt

clean: clean-checkpoint-1 clean-checkpoint-2 clean-checkpoint-3

########################################################################################################################
# Checkpoint 1 targets
########################################################################################################################
clean-checkpoint-1:
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_EXE)
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_OUT_EASY) ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_OUT_HARD)
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(CP_1)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-checkpoint-1:
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_EXE)
	cd $(CP_1) && $(CXX) $(CP_1_CXX_FLAGS) $(CP_1_SRC) -o $(RESULTS_DIR)/$(CP_1_EXE)

run-checkpoint-1-easy: build-checkpoint-1
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_OUT_EASY)
	cd $(CP_1) && ./$(RESULTS_DIR)/$(CP_1_EXE) --folder ../$(EASY_INPUT) | tee $(RESULTS_DIR)/$(CP_1_OUT_EASY)

run-checkpoint-1-hard: build-checkpoint-1
	rm -f ./$(CP_1)/$(RESULTS_DIR)/$(CP_1_OUT_HARD)
	cd $(CP_1) && ./$(RESULTS_DIR)/$(CP_1_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(CP_1_OUT_HARD)

profile-checkpoint-1: build-checkpoint-1
	rm -f $(CP_1)/$(RESULTS_DIR)/$(PROFILER_FILE) $(CP_1)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(CP_1)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(CP_1_EXE) --file ../../$(PROFILER_INPUT)
	cd $(CP_1)/$(RESULTS_DIR) && pprof -pdf ./$(CP_1_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)

########################################################################################################################
# Checkpoint 2 targets
########################################################################################################################
clean-checkpoint-2:
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_EXE)
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_OUT_EASY) ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_OUT_HARD)
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(CP_2)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-checkpoint-2:
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_EXE)
	cd $(CP_2) && $(CXX) $(CP_2_CXX_FLAGS) $(CP_2_SRC) -o $(RESULTS_DIR)/$(CP_2_EXE)

run-checkpoint-2-easy: build-checkpoint-2
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_OUT_EASY)
	cd $(CP_2) && ./$(RESULTS_DIR)/$(CP_2_EXE) --folder ../$(EASY_INPUT) | tee $(RESULTS_DIR)/$(CP_2_OUT_EASY)

run-checkpoint-2-hard: build-checkpoint-2
	rm -f ./$(CP_2)/$(RESULTS_DIR)/$(CP_2_OUT_HARD)
	cd $(CP_2) && ./$(RESULTS_DIR)/$(CP_2_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(CP_2_OUT_HARD)

profile-checkpoint-2: build-checkpoint-2
	rm -f $(CP_2)/$(RESULTS_DIR)/$(PROFILER_FILE) $(CP_2)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(CP_2)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(CP_2_EXE) --file ../../$(PROFILER_INPUT)
	cd $(CP_2)/$(RESULTS_DIR) && pprof -pdf ./$(CP_2_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)

########################################################################################################################
# Checkpoint 3 targets
########################################################################################################################
clean-checkpoint-3:
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_EXE)
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_EASY) ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_HARD)
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(CP_3)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-checkpoint-3:
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_EXE)
	cd $(CP_3) && $(CXX) $(CP_3_CXX_FLAGS) $(CP_3_SRC) -o $(RESULTS_DIR)/$(CP_3_EXE)

run-checkpoint-3-easy: build-checkpoint-3
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_EASY)
	cd $(CP_3) && ./$(RESULTS_DIR)/$(CP_3_EXE) --folder ../$(EASY_INPUT) | tee $(RESULTS_DIR)/$(CP_3_OUT_EASY)

run-checkpoint-3-hard: build-checkpoint-3
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_HARD)
	cd $(CP_3) && ./$(RESULTS_DIR)/$(CP_3_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(CP_3_OUT_HARD)

profile-checkpoint-3: build-checkpoint-3
	rm -f $(CP_3)/$(RESULTS_DIR)/$(PROFILER_FILE) $(CP_3)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(CP_3)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(CP_3_EXE) --file ../../$(PROFILER_INPUT)
	cd $(CP_3)/$(RESULTS_DIR) && pprof -pdf ./$(CP_3_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
