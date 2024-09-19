#ifndef TRAP_H
#define TRAP_H

#define DATA_MSG           0
#define PROMPT_MSG         1
#define RESPONSE_MSG       2
#define OPEN_FILE_ERROR    -1
#define MALLOC_ERROR       -2
#define TYPE_ERROR         -3
#define MIN(a,b)           ((a)<(b)?(a):(b))
#define BLOCK_LOW(id,p,n)  ((id)*(n)/(p))
#define BLOCK_HIGH(id,p,n) (BLOCK_LOW((id)+1,p,n)-1)
#define BLOCK_SIZE(id,p,n) \
                     (BLOCK_HIGH(id,p,n)-BLOCK_LOW(id,p,n)+1)
#define BLOCK_OWNER(j,p,n) (((p)*((j)+1)-1)/(n))
#define PTR_SIZE           (sizeof(void*))
#define CEILING(i,j)       (((i)+(j)-1)/(j))

/* Function prototypes */
void parse_input(int argc, char* argv[], int my_rank, double* a_p, double* b_p, int* n_p, int* f_p);
double Trap(double left_endpt, double right_endpt, int trap_count, double base_len, int func_choice, int rank);
double f1(double x);
double f2(double x);
double f3(double x);

#endif
