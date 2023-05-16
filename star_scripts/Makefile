# Star scripts
# run commands
run-sequential-jobs:
	cd sequential; qrun 20c 1 pdp_serial serial_job_seq_23_20.sh
	cd sequential; qrun 20c 1 pdp_serial serial_job_seq_24_23.sh
	cd sequential; qrun 20c 1 pdp_serial serial_job_seq_25_26.sh

run-parallel-task-jobs:
# 1 thread
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_1_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_1_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_1_26_25.sh
# 2 threads
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_2_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_2_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_2_26_25.sh
# 4 threads
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_4_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_4_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_4_26_25.sh
# 8 threads
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_8_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_8_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_8_26_25.sh
# 16 threads
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_16_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_16_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_16_26_25.sh
# 20 threads
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_20_23_20.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_20_24_23.sh
	cd parallel_task; qrun 20c 1 pdp_serial serial_job_pt_20_26_25.sh

run-parallel-data-jobs:
# 1 thread
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_1_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_1_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_1_26_25.sh
# 2 threads
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_2_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_2_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_2_26_25.sh
# 4 threads
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_4_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_4_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_4_26_25.sh
# 8 threads
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_8_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_8_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_8_26_25.sh
# 16 threads
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_16_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_16_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_16_26_25.sh
# 20 threads
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_20_23_20.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_20_24_23.sh
	cd parallel_data; qrun 20c 1 pdp_serial serial_job_pd_20_26_25.sh

run-mpi-jobs:
# 2 cores
	cd mpi; qrun 20c 2 pdp_fast parallel_job_mpi_2_20_23_20.sh
	cd mpi; qrun 20c 2 pdp_fast parallel_job_mpi_2_20_24_23.sh
	cd mpi; qrun 20c 2 pdp_long parallel_job_mpi_2_20_26_25.sh
# 3 cores
	cd mpi; qrun 20c 3 pdp_fast parallel_job_mpi_3_20_23_20.sh
	cd mpi; qrun 20c 3 pdp_long parallel_job_mpi_3_20_24_23.sh
	cd mpi; qrun 20c 3 pdp_long parallel_job_mpi_3_20_26_25.sh
# 4 cores
	cd mpi; qrun 20c 4 pdp_long parallel_job_mpi_4_20_23_20.sh
	cd mpi; qrun 20c 4 pdp_long parallel_job_mpi_4_20_24_23.sh
	cd mpi; qrun 20c 4 pdp_long parallel_job_mpi_4_20_26_25.sh
