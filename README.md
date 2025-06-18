# CPU SCHEDULING ALGORITHMS SIMULATOR  

_Visualizing Process Scheduling, Optimizing CPU Utilization_  

![last commit today](https://img.shields.io/badge/last%20commit-today-brightgreen) ![C](https://img.shields.io/badge/language-C-blue) ![Lines of Code](https://img.shields.io/badge/lines%20of%20code-300+-lightgrey)  

---  

## Built With  

- **C** (C11 standard)  
- No external dependencies (uses standard library only)  

---  

## Table of Contents  

1. [Features](#features)  
2. [Prerequisites](#prerequisites)  
3. [Getting Started](#getting-started)  
4. [Usage](#usage)  
5. [Example Input/Output](#example-inputoutput)  
6. [Screenshots](#screenshots)  
7. [License](#license)  

---  

## Features  

- ✔️ Simulates **FCFS** (First-Come, First-Served)  
- ✔️ Simulates **SJF** (Shortest Job First) – non-preemptive  
- ✔️ Simulates **SRTF** (Shortest Remaining Time First) – preemptive  
- ✔️ Simulates **Priority Scheduling** – non-preemptive (lower number = higher priority)  
- ✔️ Simulates **Round Robin** – preemptive with customizable time quantum  
- ✔️ Calculates and displays **Completion Time**, **Turnaround Time**, and **Waiting Time** for each process  
- ✔️ Computes and prints **average Turnaround Time** and **average Waiting Time**  

---  

## Prerequisites  

- A C11-compatible compiler (e.g., `gcc`, `clang`)  
- (Windows / macOS / Linux)  
- No additional libraries required  

---  

## Getting Started  

1. **Clone the repo**  
   ```bash
   git clone https://github.com/your-username/cpu-scheduling-simulator.git
   cd cpu-scheduling-simulator
   ```

2. Compile
   ```bash
   # Linux / macOS
    gcc -std=c11 scheduler.c -o scheduler
   
    # Windows (MinGW)
    gcc -std=c11 scheduler.c -o scheduler.exe
   ```

3. Usage
   ```bash
   # Run the simulator
    ./scheduler        # (or scheduler.exe on Windows)
   ```
    Enter the number of processes.

    For each process, input:
    
    Arrival Time
    
    Burst Time
    
    Priority (integer; lower = higher priority)
    
    Select a scheduling algorithm (1–5).
    
    If Round Robin is chosen, enter a time quantum.
    
    View per-process metrics and overall averages.

4. Example Input/Output
   ```bash
   Enter number of processes: 3

    Process 1:
      Arrival Time: 0
      Burst Time:   5
      Priority (lower = higher): 2
    
    Process 2:
      Arrival Time: 1
      Burst Time:   3
      Priority (lower = higher): 1
    
    Process 3:
      Arrival Time: 2
      Burst Time:   8
      Priority (lower = higher): 3
    
    Choose scheduling algorithm:
    1. FCFS
    2. SJF (Non-preemptive)
    3. SRTF (Preemptive SJF)
    4. Priority (Non-preemptive)
    5. Round Robin
    Selection: 3
    
    --- SRTF Scheduling ---
    
    PID  AT  BT  PR  CT  TAT  WT
    1    0   5   2   5   5    0
    2    1   3   1   8   7    4
    3    2   8   3   16  14   6
    
    Average Turnaround Time = 8.67
    Average Waiting Time    = 3.33
   ```
    


