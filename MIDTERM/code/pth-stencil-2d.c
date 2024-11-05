#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include "utilities.h"
#include "timer.h"
#include "my_barrier.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))
#define BLOCK_LOW(id, p, n) ((id) * (n) / (p))
#define BLOCK_HIGH(id, p, n) (BLOCK_LOW((id) + 1, p, n) - 1)
#define BLOCK_SIZE(id, p, n) \
    (BLOCK_HIGH(id, p, n) - BLOCK_LOW(id, p, n) + 1)
#define BLOCK_OWNER(j, p, n) (((p) * ((j) + 1) - 1) / (n))
#define PTR_SIZE (sizeof(void *))
#define CEILING(i, j) (((i) + (j) - 1) / (j))

// Global variables for sharing data between threads
double **global_data;
double **global_temp;
int global_rows;
int global_cols;
int global_num_threads;
my_barrier_t global_barrier;

// Thread function to perform stencil operations
void *stencil_thread(void *arg)
{
    int thread_id = *((int *)arg);

    // Calculate start and end rows for this thread
    int start_row = BLOCK_LOW(thread_id, global_num_threads, global_rows);
    int end_row = BLOCK_HIGH(thread_id, global_num_threads, global_rows);

    // Only process interior points
    if (start_row == 0)
        start_row = 1;
    if (end_row == global_rows - 1)
        end_row = global_rows - 2;

    // Apply stencil to assigned rows
    for (int i = start_row; i <= end_row; i++)
    {
        for (int j = 1; j < global_cols - 1; j++)
        {
            global_temp[i][j] = (global_data[i - 1][j - 1] + global_data[i - 1][j] + global_data[i - 1][j + 1] +
                                 global_data[i][j + 1] + global_data[i + 1][j + 1] + global_data[i + 1][j] +
                                 global_data[i + 1][j - 1] + global_data[i][j - 1] + global_data[i][j]) /
                                9.0;
        }
    }

    // Wait for all threads to complete their computation
    my_pthread_barrier_wait(&global_barrier);

    return NULL;
}

int main(int argc, char *argv[])
{
    if (argc < 5 || argc > 7)
    {
        fprintf(stderr, "Usage: %s <num_iterations> <input_file> <output_file> <debug_level> [num_threads] [all_iterations_file]\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    int iterations = atoi(argv[1]);
    char *input_file = argv[2];
    char *output_file = argv[3];
    int debug_level = atoi(argv[4]);
    global_num_threads = (argc > 5) ? atoi(argv[5]) : 1;
    char *all_iterations_file = (argc > 6) ? argv[6] : NULL;

    double start_total, finish_total, elapsed_total;
    double start_comp, finish_comp, elapsed_comp;

    GET_TIME(start_total);

    // Read input data
    FILE *fp = fopen(input_file, "rb");
    if (!fp)
    {
        fprintf(stderr, "Error opening input file\n");
        exit(EXIT_FAILURE);
    }

    fread(&global_rows, sizeof(int), 1, fp);
    fread(&global_cols, sizeof(int), 1, fp);

    if (debug_level >= 1)
    {
        printf("Reading input file: %s\n", input_file);
        printf("Matrix dimensions: %d rows x %d columns\n", global_rows, global_cols);
        printf("Number of iterations: %d\n", iterations);
        printf("Number of threads: %d\n", global_num_threads);
        if (all_iterations_file)
        {
            printf("Writing all iterations to: %s\n", all_iterations_file);
        }
        printf("\n");
    }

    global_data = allocate_2d_array(global_rows, global_cols);
    global_temp = allocate_2d_array(global_rows, global_cols);

    for (int i = 0; i < global_rows; i++)
    {
        fread(global_data[i], sizeof(double), global_cols, fp);
    }
    fclose(fp);

    // Initialize all iterations file if specified
    FILE *all_fp = NULL;
    if (all_iterations_file)
    {
        all_fp = fopen(all_iterations_file, "wb");
        if (!all_fp)
        {
            fprintf(stderr, "Error opening all iterations file\n");
            exit(EXIT_FAILURE);
        }
        fwrite(&global_rows, sizeof(int), 1, all_fp);
        fwrite(&global_cols, sizeof(int), 1, all_fp);

        // Write initial state
        for (int i = 0; i < global_rows; i++)
        {
            fwrite(global_data[i], sizeof(double), global_cols, all_fp);
        }
    }

    // Initialize thread resources
    pthread_t *threads = malloc(global_num_threads * sizeof(pthread_t));
    int *thread_ids = malloc(global_num_threads * sizeof(int));

    // Initialize barrier
    my_pthread_barrier_init(&global_barrier, 0, global_num_threads);

    if (debug_level >= 1)
    {
        long data_size = global_rows * global_cols * sizeof(double) + 2 * sizeof(int);
        printf("Input file size: %ld bytes\n", data_size);
        printf("Starting computation...\n\n");
    }

    GET_TIME(start_comp);

    // Main iteration loop
    for (int iter = 0; iter < iterations; iter++)
    {
        // Create threads for computation
        for (int i = 0; i < global_num_threads; i++)
        {
            thread_ids[i] = i; // Each thread needs a unique ID
            pthread_create(&threads[i], NULL, stencil_thread, &thread_ids[i]);
        }

        // Wait for all threads to complete
        for (int i = 0; i < global_num_threads; i++)
        {
            pthread_join(threads[i], NULL);
        }

        // Copy boundary conditions
        for (int i = 0; i < global_rows; i++)
        {
            global_temp[i][0] = global_data[i][0];
            global_temp[i][global_cols - 1] = global_data[i][global_cols - 1];
        }
        for (int j = 0; j < global_cols; j++)
        {
            global_temp[0][j] = global_data[0][j];
            global_temp[global_rows - 1][j] = global_data[global_rows - 1][j];
        }

        // Swap arrays
        double **swap = global_data;
        global_data = global_temp;
        global_temp = swap;

        // Write current state to all iterations file if specified
        if (all_fp)
        {
            for (int i = 0; i < global_rows; i++)
            {
                fwrite(global_data[i], sizeof(double), global_cols, all_fp);
            }
        }

        // Print debug information if requested
        if (debug_level == 2)
        {
            printf("\nAfter iteration %d:\n", iter + 1);
            print_2d_array(global_data, global_rows, global_cols);
        }
    }

    GET_TIME(finish_comp);
    elapsed_comp = finish_comp - start_comp;

    // Write final state
    FILE *out_fp = fopen(output_file, "wb");
    if (!out_fp)
    {
        fprintf(stderr, "Error opening output file\n");
        exit(EXIT_FAILURE);
    }

    fwrite(&global_rows, sizeof(int), 1, out_fp);
    fwrite(&global_cols, sizeof(int), 1, out_fp);
    for (int i = 0; i < global_rows; i++)
    {
        fwrite(global_data[i], sizeof(double), global_cols, out_fp);
    }
    fclose(out_fp);
    if (all_fp)
        fclose(all_fp);

    if (debug_level >= 1)
    {
        printf("Computation completed.\n");
        printf("Final state written to: %s\n\n", output_file);
    }

    // Clean up
    my_pthread_barrier_destroy(&global_barrier);
    free(threads);
    free(thread_ids);
    free_2d_array(global_data, global_rows);
    free_2d_array(global_temp, global_rows);

    GET_TIME(finish_total);
    elapsed_total = finish_total - start_total;

    if (debug_level >= 0)
    {
        printf("Time overall = %e seconds\n", elapsed_total);
        printf("Time computation = %e seconds\n", elapsed_comp);
        printf("Time other = %e seconds\n", elapsed_total - elapsed_comp);
    }

    return 0;
}