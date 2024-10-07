#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>

void pattern_one(int num_of_processes) {
    printf("Pattern 1\n");
    for ( int p1 = 1; p1 <= num_of_processes; p1++) {
        pid_t pid = fork();
        if (pid == 0) {
            printf("Process %d PID: %d created by main PID: %d\n", p1, getpid(), getppid());
            exit(0);            
        }
    }
    while (wait(NULL) > 0);
}

void pattern_two(int num_of_processes) {
    printf("Pattern 2\n");
    pid_t pid = fork();
    for (int p2 = 1; p2 <= num_of_processes; p2++) {
        if (pid == 0) {
            printf("Process %d PID: %d created by PID: %d\n", p2, getpid(), getppid());
            sleep(rand() % 8 + 1);
            if (p2 == num_of_processes) {
                printf("Process %d PID: %d exiting\n", p2, getpid());
            }            
        } else if (pid > 0) {
            wait(NULL);
            break;
        } else {
            perror("fork failed\n");
            exit(1);
        }
    }
}

void pattern_three(int current_process, int num_of_processes) {
    printf("Pattern 3\n");
    if (current_process > num_of_processes) {
        return;
    }
    pid_t left;
    if (left == 0) {
        printf("Process %d (PID: %d) created by PID: %d\n", current_process, getpid(), getppid());
        sleep(rand() % 8 + 1);
        pattern_three(current_process * 2, num_of_processes);
        printf("Process %d (PID: %d) exiting\n", current_process, getpid());
        exit(0);
    } else if (left > 0) {
        wait(NULL);
        pid_t right = fork();
        if (right == 0) {
            printf("Process %d (PID: %d) create by PID: %d\n", current_process + 1, getpid(), getppid());
            sleep(rand() % 8 + 1);
            pattern_three(current_process * 2 + 1, num_of_processes);
            printf("Process %d (PID: %d) exiting\n", current_process + 1, getpid());
            exit(0);
        } else if (right > 0) {
            wait(NULL);
        } else {
            perror("fork failed");
            exit(1);
        }                
    }
}


int main(int argc, char *argv[])
{
    int num_of_processes = atoi(argv[1]);
    int pattern_num = atoi(argv[2]);

    if (argc < 3)
    {
        printf("Insufficient arguments. Usage: %s <num_things> <pattern_num>\n", argv[0]);
        return 1;
    }
    if (num_of_processes < 1 || num_of_processes > 256)
    {
        printf("Number of things value must be between 1 - 256.\n");
        return 1;
    }

    switch (pattern_num)
    {
    case 1:
        pattern_one(num_of_processes);
        break;

    case 2:
        pattern_two(num_of_processes);
        break;

    case 3:
        pattern_three(1, num_of_processes);
        break;


    default:
        printf("You didn't choose a valid pattern. Please choose 1, 2, or 3 for Y.\n./forky ' X ' ' Y '\n");
        return 1;
    }
    return 0;
}

// TODO: Pattern 3. not properly forking.