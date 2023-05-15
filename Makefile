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
PROFILER_INPUT := inputs/medium/graf_23_20.txt
PROFILER_PDF := profiler.pdf

clean: clean-sequential clean-parallel-task clean-parallel-data clean-mpi

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
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_EASY) ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_MEDIUM) ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_HARD)
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(SEQ)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-sequential:
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_EXE)
	cd $(SEQ) && $(CXX) $(SEQ_CXX_FLAGS) $(SEQ_SRC) -o $(RESULTS_DIR)/$(SEQ_EXE)

run-sequential-easy: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_EASY)
	cd $(SEQ)/$(RESULTS_DIR) && ./$(SEQ_EXE) --folder ../../$(EASY_INPUT) | tee ./$(SEQ_OUT_EASY)

run-sequential-medium: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_MEDIUM)
	cd $(SEQ)/$(RESULTS_DIR) && ./$(SEQ_EXE) --folder ../../$(MEDIUM_INPUT) | tee ./$(SEQ_OUT_MEDIUM)

run-sequential-hard: build-sequential
	rm -f ./$(SEQ)/$(RESULTS_DIR)/$(SEQ_OUT_HARD)
	cd $(SEQ)/$(RESULTS_DIR) && ./$(SEQ_EXE) --folder ../../$(HARD_INPUT) | tee ./$(SEQ_OUT_HARD)

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
# Parallel task targets
clean-parallel-task:
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_EXE)
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_EASY) ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_MEDIUM) ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_HARD)
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(PT)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-parallel-task:
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_EXE)
	cd $(PT) && $(CXX) $(PT_CXX_FLAGS) $(PT_SRC) -o $(RESULTS_DIR)/$(PT_EXE)

run-parallel-task-easy: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_EASY)
	cd $(PT)/$(RESULTS_DIR) && ./$(PT_EXE) --folder ../../$(EASY_INPUT) | tee ./$(PT_OUT_EASY)

run-parallel-task-medium: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_MEDIUM)
	cd $(PT)/$(RESULTS_DIR) && ./$(PT_EXE) --folder ../../$(MEDIUM_INPUT) | tee ./$(PT_OUT_MEDIUM)

run-parallel-task-hard: build-parallel-task
	rm -f ./$(PT)/$(RESULTS_DIR)/$(PT_OUT_HARD)
	cd $(PT)/$(RESULTS_DIR) && ./$(PT_EXE) --folder ../../$(HARD_INPUT) | tee ./$(PT_OUT_HARD)

profile-parallel-task: build-parallel-task
	rm -f $(PT)/$(RESULTS_DIR)/$(PROFILER_FILE) $(PT)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(PT)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(PT_EXE) --file ../../$(PROFILER_INPUT)
	cd $(PT)/$(RESULTS_DIR) && pprof -pdf ./$(PT_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
########################################################################################################################
# Parallel data settings
PD_CXX_FLAGS := -pedantic -Wall -Wextra -fopenmp -O3 -std=c++17
PD := parallel_data
PD_SRC := $(PD).cpp
PD_EXE := $(PD).exe
PD_OUT_EASY := $(PD)_easy.out.txt
PD_OUT_MEDIUM := $(PD)_medium.out.txt
PD_OUT_HARD := $(PD)_hard.out.txt
# Parallel data targets
clean-parallel-data:
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_EXE)
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_EASY) ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_MEDIUM) ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_HARD)
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PROFILER_FILE) ./$(PD)/$(RESULTS_DIR)/$(PROFILER_PDF)

build-parallel-data:
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_EXE)
	cd $(PD) && $(CXX) $(PD_CXX_FLAGS) $(PD_SRC) -o $(RESULTS_DIR)/$(PD_EXE)

run-parallel-data-easy: build-parallel-data
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_EASY)
	cd $(PD)/$(RESULTS_DIR) && ./$(PD_EXE) --folder ../$(EASY_INPUT) | tee ./$(PD_OUT_EASY)

run-parallel-data-medium: build-parallel-data
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_MEDIUM)
	cd $(PD)/$(RESULTS_DIR) && ./$(PD_EXE) --folder ../../$(MEDIUM_INPUT) | tee ./$(PD_OUT_MEDIUM)

run-parallel-data-hard: build-parallel-data
	rm -f ./$(PD)/$(RESULTS_DIR)/$(PD_OUT_HARD)
	cd $(PD)/$(RESULTS_DIR) && ./$(PD_EXE) --folder ../../$(HARD_INPUT) | tee ./$(PD_OUT_HARD)

profile-parallel-data: build-parallel-data
	rm -f $(PD)/$(RESULTS_DIR)/$(PROFILER_FILE) $(PD)/$(RESULTS_DIR)/$(PROFILER_PDF)
	cd $(PD)/$(RESULTS_DIR) && LD_PRELOAD=$(PROFILER) CPUPROFILE=$(PROFILER_FILE) CPUPROFILE_FREQUENCY=$(PROFILER_FREQ) ./$(PD_EXE) --file ../../$(PROFILER_INPUT)
	cd $(PD)/$(RESULTS_DIR) && pprof -pdf ./$(PD_EXE) $(PROFILER_FILE) > $(PROFILER_PDF)
########################################################################################################################
# MPI compile
MPI_CXX := mpic++
MPI_CXX_FLAGS := -pedantic -Wall -Wextra -lmpi -O3 -std=c++17
# MPI Settings
MPI := mpi
MPI_SRC := $(MPI).cpp
MPI_EXE := $(MPI).exe
# MPI targets
clean-mpi:
	rm -f ./$(MPI)/$(RESULTS_DIR)/$(MPI_EXE)

build-mpi:
	cd $(MPI) && $(MPI_CXX) $(MPI_CXX_FLAGS) $(MPI_SRC) -o $(RESULTS_DIR)/$(MPI_EXE)
########################################################################################################################
