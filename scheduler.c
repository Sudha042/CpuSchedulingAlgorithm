#include <stdio.h>
#include <stdlib.h>
#include <limits.h>

typedef struct {
    int pid;
    int arrival;
    int burst;
    int remaining;
    int priority;
    int completion;
    int turnaround;
    int waiting;
} Process;

// Utility to copy processes array
Process* copy_processes(Process *p, int n) {
    Process *cp = malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) cp[i] = p[i];
    return cp;
}

// Print and compute averages
void print_stats(Process *p, int n) {
    double total_tat = 0, total_wt = 0;
    printf("PID\tAT\tBT\tPR\tCT\tTAT\tWT\n");
    for (int i = 0; i < n; i++) {
        total_tat += p[i].turnaround;
        total_wt += p[i].waiting;
        printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n",
               p[i].pid, p[i].arrival, p[i].burst, p[i].priority,
               p[i].completion, p[i].turnaround, p[i].waiting);
    }
    printf("\nAverage Turnaround Time = %.2f\n", total_tat / n);
    printf("Average Waiting Time    = %.2f\n", total_wt / n);
}

// First-Come, First-Served (non-preemptive)
void fcfs(Process *orig, int n) {
    Process *p = copy_processes(orig, n);
    // sort by arrival time
    for (int i = 0; i < n - 1; i++)
        for (int j = i + 1; j < n; j++)
            if (p[i].arrival > p[j].arrival)
                { Process tmp = p[i]; p[i] = p[j]; p[j] = tmp; }

    int time = 0;
    for (int i = 0; i < n; i++) {
        if (time < p[i].arrival)
            time = p[i].arrival;
        time += p[i].burst;
        p[i].completion = time;
        p[i].turnaround = p[i].completion - p[i].arrival;
        p[i].waiting = p[i].turnaround - p[i].burst;
    }
    printf("\n--- FCFS Scheduling ---\n");
    print_stats(p, n);
    free(p);
}

// Shortest Job First (non-preemptive)
void sjf_non_preemptive(Process *orig, int n) {
    Process *p = copy_processes(orig, n);
    int completed = 0, time = 0;
    int *done = calloc(n, sizeof(int));

    while (completed < n) {
        int idx = -1, min_bt = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time && p[i].burst < min_bt) {
                min_bt = p[i].burst;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done[idx] = 1;
        completed++;
    }
    printf("\n--- SJF (Non-preemptive) Scheduling ---\n");
    print_stats(p, n);
    free(p);
    free(done);
}

// Shortest Remaining Time First (preemptive SJF)
void srtf(Process *orig, int n) {
    Process *p = copy_processes(orig, n);
    int completed = 0, time = 0;
    int *done = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    while (completed < n) {
        int idx = -1, min_rem = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time && p[i].remaining < min_rem) {
                min_rem = p[i].remaining;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        p[idx].remaining--;
        time++;
        if (p[idx].remaining == 0) {
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            done[idx] = 1;
            completed++;
        }
    }
    printf("\n--- SRTF Scheduling ---\n");
    print_stats(p, n);
    free(p);
    free(done);
}

// Priority Scheduling (non-preemptive, lower number => higher priority)
void priority_non_preemptive(Process *orig, int n) {
    Process *p = copy_processes(orig, n);
    int completed = 0, time = 0;
    int *done = calloc(n, sizeof(int));

    while (completed < n) {
        int idx = -1, min_pr = INT_MAX;
        for (int i = 0; i < n; i++) {
            if (!done[i] && p[i].arrival <= time && p[i].priority < min_pr) {
                min_pr = p[i].priority;
                idx = i;
            }
        }
        if (idx == -1) {
            time++;
            continue;
        }
        time += p[idx].burst;
        p[idx].completion = time;
        p[idx].turnaround = p[idx].completion - p[idx].arrival;
        p[idx].waiting = p[idx].turnaround - p[idx].burst;
        done[idx] = 1;
        completed++;
    }
    printf("\n--- Priority Scheduling (Non-preemptive) ---\n");
    print_stats(p, n);
    free(p);
    free(done);
}

// Round Robin (preemptive)
void round_robin(Process *orig, int n, int quantum) {
    Process *p = copy_processes(orig, n);
    int time = 0, completed = 0;
    int *queue = malloc(n * sizeof(int));
    int front = 0, rear = 0, count = 0;
    int *in_queue = calloc(n, sizeof(int));
    for (int i = 0; i < n; i++) p[i].remaining = p[i].burst;

    // enqueue processes with arrival == 0
    for (int i = 0; i < n; i++) {
        if (p[i].arrival == 0) {
            queue[rear++] = i;
            in_queue[i] = 1;
            count++;
        }
    }

    while (completed < n) {
        if (count == 0) {
            time++;
            // enqueue newly arrived
            for (int i = 0; i < n; i++) {
                if (!in_queue[i] && p[i].arrival <= time) {
                    queue[rear++] = i;
                    in_queue[i] = 1;
                    count++;
                }
            }
            continue;
        }
        int idx = queue[front++];
        count--;
        int exec = p[idx].remaining < quantum ? p[idx].remaining : quantum;
        p[idx].remaining -= exec;
        time += exec;
        // enqueue new arrivals during execution
        for (int i = 0; i < n; i++) {
            if (!in_queue[i] && p[i].arrival <= time) {
                queue[rear++] = i;
                in_queue[i] = 1;
                count++;
            }
        }
        if (p[idx].remaining > 0) {
            queue[rear++] = idx;
            count++;
        } else {
            p[idx].completion = time;
            p[idx].turnaround = p[idx].completion - p[idx].arrival;
            p[idx].waiting = p[idx].turnaround - p[idx].burst;
            completed++;
        }
    }
    printf("\n--- Round Robin Scheduling (Quantum=%d) ---\n", quantum);
    print_stats(p, n);
    free(p);
    free(queue);
    free(in_queue);
}

int main() {
    int n;
    printf("Enter number of processes: ");
    scanf("%d", &n);

    Process *p = malloc(n * sizeof(Process));
    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;
        printf("\nProcess %d:\n", p[i].pid);
        printf("  Arrival Time: "); scanf("%d", &p[i].arrival);
        printf("  Burst Time:   "); scanf("%d", &p[i].burst);
        printf("  Priority (lower = higher): "); scanf("%d", &p[i].priority);
    }

    printf("\nChoose scheduling algorithm:\n");
    printf("1. FCFS\n2. SJF (Non-preemptive)\n3. SRTF (Preemptive SJF)\n4. Priority (Non-preemptive)\n5. Round Robin\nSelection: ");
    int choice; scanf("%d", &choice);

    int quantum;
    if (choice == 5) {
        printf("Enter time quantum: ");
        scanf("%d", &quantum);
    }

    switch (choice) {
        case 1: fcfs(p, n); break;
        case 2: sjf_non_preemptive(p, n); break;
        case 3: srtf(p, n); break;
        case 4: priority_non_preemptive(p, n); break;
        case 5: round_robin(p, n, quantum); break;
        default: printf("Invalid choice\n"); break;
    }

    free(p);
    return 0;
}
