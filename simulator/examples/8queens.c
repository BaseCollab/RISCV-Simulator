#include <stdio.h>
#include <stdlib.h>

typedef int bool;

#define TRUE    1
#define FALSE   0
#define MAXCANDIDATES   100		/* max possible next extensions */
#define NMAX            100		/* maximum solution size */

typedef char *data;

int solution_count;

bool finished = FALSE;

void process_solution(int a[], int k)
{
	int i;				/* counter */

	solution_count++;
}

int is_a_solution(int a[], int k, int n)
{
	return (k == n);
}

int construct_candidates(int a[], int k, int n, int c[], int *ncandidates)
{
	int i, j;
	bool legal_move;

	*ncandidates = 0;
	for (i=1; i<=n; i++) {
		legal_move = TRUE;

		for (j = 1; j < k; j++) {
			if (abs((k) - j) == abs(i - a[j]))
				legal_move = FALSE;
			if (i == a[j])
				legal_move = FALSE;
		}

		if (legal_move == TRUE) {
			c[*ncandidates] = i;
			*ncandidates = *ncandidates + 1;
		}
	}
}

void backtrack(int a[], int k, data input)
{
        int c[MAXCANDIDATES];
        int ncandidates;
        int i;

        if (is_a_solution(a, k, input))
            process_solution(a, k, input);
        else {
            k = k + 1;
            construct_candidates(a, k, input, c, &ncandidates);
            for (i = 0; i < ncandidates; i++) {
                a[k] = c[i];
                backtrack(a, k, input);

                if (finished)
                    return;
            }
        }
}

int main()
{
	int a[NMAX] = {0};
	int i = 0;
	int compare[]={1, 0, 0, 2, 10, 4, 40, 92};

	for (i = 1; i <= 8; i++) {
		solution_count = 0;
		backtrack(a, 0, i);
		if (compare[i - 1] != solution_count)
			return 1;

	}
	return 0;
}


