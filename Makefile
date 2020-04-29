CC = gcc
CFLAGS = -Wall -O2
EXE = main
OBJS = main.o
RUN = run.o

INPUT = OS_PJ1_Test
STDIN = stdin
ARG1 = FIFO_1
ARG2 = PSJF_2
ARG3 = RR_3
ARG4 = SJF_4
ARG5 = TIME_MEASUREMENT
OUTPUT = output
STDOUT = stdout
DMESG = dmesg

all: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) > $(OUTPUT)/$(STDIN)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(STDIN)_$(DMESG).txt
	sudo dmesg -c
	
run:
	make run1
	make run2
	make run3
	make run4
	make run5

run1: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) < $(INPUT)/$(ARG1).txt > $(OUTPUT)/$(ARG1)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(ARG1)_$(DMESG).txt
	sudo dmesg -c

run2: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) < $(INPUT)/$(ARG2).txt > $(OUTPUT)/$(ARG2)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(ARG2)_$(DMESG).txt
	sudo dmesg -c

run3: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) < $(INPUT)/$(ARG3).txt > $(OUTPUT)/$(ARG3)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(ARG3)_$(DMESG).txt
	sudo dmesg -c

run4: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) < $(INPUT)/$(ARG4).txt > $(OUTPUT)/$(ARG4)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(ARG4)_$(DMESG).txt
	sudo dmesg -c

run5: $(EXE)
	sudo dmesg -c
	clear
	sudo ./$(EXE) < $(INPUT)/$(ARG5).txt > $(OUTPUT)/$(ARG5)_$(STDOUT).txt
	dmesg | grep Project1 > $(OUTPUT)/$(ARG5)_$(DMESG).txt
	sudo dmesg -c

allfile:
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/FIFO_1.txt > $(OUTPUT)/FIFO_1_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/FIFO_1_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/FIFO_2.txt > $(OUTPUT)/FIFO_2_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/FIFO_2_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/FIFO_3.txt > $(OUTPUT)/FIFO_3_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/FIFO_3_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/FIFO_4.txt > $(OUTPUT)/FIFO_4_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/FIFO_4_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/FIFO_5.txt > $(OUTPUT)/FIFO_5_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/FIFO_5_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_1.txt > $(OUTPUT)/PSJF_1_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_1_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_1.txt > $(OUTPUT)/PSJF_1_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_1_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_2.txt > $(OUTPUT)/PSJF_2_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_2_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_3.txt > $(OUTPUT)/PSJF_3_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_3_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_4.txt > $(OUTPUT)/PSJF_4_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_4_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/PSJF_5.txt > $(OUTPUT)/PSJF_5_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/PSJF_5_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/RR_1.txt > $(OUTPUT)/RR_1_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/RR_1_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/RR_2.txt > $(OUTPUT)/RR_2_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/RR_2_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/RR_3.txt > $(OUTPUT)/RR_3_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/RR_3_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/RR_4.txt > $(OUTPUT)/RR_4_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/RR_4_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/RR_5.txt > $(OUTPUT)/RR_5_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/RR_5_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/SJF_1.txt > $(OUTPUT)/SJF_1_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/SJF_1_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/SJF_2.txt > $(OUTPUT)/SJF_2_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/SJF_2_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/SJF_3.txt > $(OUTPUT)/SJF_3_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/SJF_3_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/SJF_4.txt > $(OUTPUT)/SJF_4_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/SJF_4_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/SJF_5.txt > $(OUTPUT)/SJF_5_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/SJF_5_$(DMESG).txt
	sudo dmesg -c
	sudo ./$(EXE) < $(INPUT)/TIME_MEASUREMENT.txt > $(OUTPUT)/TIME_MEASUREMENT_stdout.txt
	dmesg | grep Project1 > $(OUTPUT)/TIME_MEASUREMENT_$(DMESG).txt
	sudo dmesg -c
	clear

$(EXE): $(OBJS) $(RUN)
	$(CC) $(CFLAGS) -o $(EXE) $(OBJS) $(RUN)

%.o: %.c
	$(CC) $(CFLAGS) -c -o $@ $<

.PHONY clean: clean-o clean-exe

clean-o:
	rm -rf *.o

clean-exe:
	rm -rf $(EXE)
