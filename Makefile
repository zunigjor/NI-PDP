# Global settings
CXX := g++
# Results and inputs
RESULTS_DIR := results
INPUTS_DIR := inputs
EASY_INPUT := inputs/easy
MEDIUM_INPUT := inputs/medium
HARD_INPUT := inputs/hard
# Profiler settings
PROFILER := /usr/lib/libprofiler.so
PROFILER_FILE := profiler.prof
PROFILER_FREQ := 100000
PROFILER_INPUT := inputs/hard/graf_23_20.txt
PROFILER_PDF := profiler.pdf

clean: clean-sequential clean-parallel-task clean-checkpoint-3

########################################################################################################################
# Sequential solution settings
SEQ_CXX_FLAGS := -pedantic -Wall -Wextra -O3 -std=c++17
SEQ := sequential
SEQ_SRC := $(SEQ).cpp
SEQ_EXE := $(SEQ).exe
SEQ_OUT_EASY := $(SEQ)_easy.out.txt
SEQ_OUT_MEDIUM := $(SEQ)_medium.out.txt
SEQ_OUT_HARD := $(SEQ)_hard.out.txt
# Sequential solution targets
clean-sequential:
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_EXE)
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_EASY) ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_HARD)
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(SEQ)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-sequential:
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_EXE)
	cd $(SEQ) && $(CXX) $(SEQ_CXX_FLAGS) $(SEQ_SRC) -o $(RESULTS_DIR)/$(SEQ_EXE)

run-sequential-easy: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_EASY)
	cd $(SEQ) && ./$(RESULTS_DIR)/$(SEQ_EXE) --folder ../$(EASY_INPUT) | tee $(RESULTS_DIR)/$(SEQ_OUT_EASY)

run-sequential-medium: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_MEDIUM)
	cd $(SEQ) && ./$(RESULTS_DIR)/$(SEQ_EXE) --folder ../$(MEDIUM_INPUT) | tee $(RESULTS_DIR)/$(SEQ_OUT_MEDIUM)

run-sequential-hard: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_HARD)
	cd $(SEQ) && ./$(RESULTS_DIR)/$(SEQ_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(SEQ_OUT_HARD)

profile-sequential: build-sequential
	rm -f $(SEQ)/$(RESULTS_DIR)/$(PROFILER_FILE) $(SEQ)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(SEQ)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(SEQ_EXE) --file ../../$(PROFILER_INPUT)
	cd $(SEQ)/$(RESULTS_DIR) && pprof -pdf ./$(SEQ_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
########################################################################################################################
# Parallel task settings
PT_CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -O3 -std=c++17
PT := parallel_task
PT_SRC := $(PT).cpp
PT_EXE := $(PT).exe
PT_OUT_EASY := $(PT)_easy.out.txt
PT_OUT_MEDIUM := $(PT)_medium.out.txt
PT_OUT_HARD := $(PT)_hard.out.txt
# Parallel task  targets
clean-parallel-task:
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_EXE)
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_EASY) ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_HARD)
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(PT)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-parallel-task:
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_EXE)
	cd $(PT) && $(CXX) $(PT_CXX_FLAGS) $(PT_SRC) -o $(RESULTS_DIR)/$(PT_EXE)

run-parallel-task-easy: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_EASY)
	cd $(PT) && ./$(RESULTS_DIR)/$(PT_EXE) --folder ../$(EASY_INPUT) | tee $(RESULTS_DIR)/$(PT_OUT_EASY)

run-parallel-task-medium: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_MEDIUM)
	cd $(PT) && ./$(RESULTS_DIR)/$(PT_EXE) --folder ../$(MEDIUM_INPUT) | tee $(RESULTS_DIR)/$(PT_OUT_MEDIUM)

run-parallel-task-hard: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_HARD)
	cd $(PT) && ./$(RESULTS_DIR)/$(PT_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(PT_OUT_HARD)

profile-parallel-task: build-parallel-task
	rm -f $(PT)/$(RESULTS_DIR)/$(PROFILER_FILE) $(PT)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(PT)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(PT_EXE) --file ../../$(PROFILER_INPUT)
	cd $(PT)/$(RESULTS_DIR) && pprof -pdf ./$(PT_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
########################################################################################################################
# checkpoint_3 settings
CP_3_CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -O3 -std=c++17
CP_3 := checkpoint_3
CP_3_SRC := $(CP_3).cpp
CP_3_EXE := $(CP_3).exe
CP_3_OUT_EASY := $(CP_3)_easy.out.txt
CP_3_OUT_MEDIUM := $(CP_3)_medium.out.txt
CP_3_OUT_HARD := $(CP_3)_hard.out.txt
# Checkpoint 3 targets
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

run-checkpoint-3-medium: build-checkpoint-3
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_MEDIUM)
	cd $(CP_3) && ./$(RESULTS_DIR)/$(CP_3_EXE) --folder ../$(MEDIUM_INPUT) | tee $(RESULTS_DIR)/$(CP_3_OUT_MEDIUM)

run-checkpoint-3-hard: build-checkpoint-3
	rm -f ./$(CP_3)/$(RESULTS_DIR)/$(CP_3_OUT_HARD)
	cd $(CP_3) && ./$(RESULTS_DIR)/$(CP_3_EXE) --folder ../$(HARD_INPUT) | tee $(RESULTS_DIR)/$(CP_3_OUT_HARD)

profile-checkpoint-3: build-checkpoint-3
	rm -f $(CP_3)/$(RESULTS_DIR)/$(PROFILER_FILE) $(CP_3)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(CP_3)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(CP_3_EXE) --file ../../$(PROFILER_INPUT)
	cd $(CP_3)/$(RESULTS_DIR) && pprof -pdf ./$(CP_3_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
########################################################################################################################
