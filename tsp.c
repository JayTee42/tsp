#include <stdint.h>
#include <stdio.h>
#include <omp.h>

typedef uint32_t subset_t;
typedef uint8_t vertex_t;
typedef uint16_t cost_t;

#define COST_MAX UINT16_MAX

//Maximum: 32 actual nodes -> N <= 31
#define ACTUAL_N 27
#define N (ACTUAL_N - 1)

cost_t dist_matrix[ACTUAL_N][ACTUAL_N] =
{
	{ 0, 2, 0, 6, 1, 1, 0, 3, 4, 2, 8, 1, 4, 3, 6, 3, 4, 2, 2, 7, 4, 2, 3, 5, 4, 2, 5 },
	{ 1, 0, 4, 4, 2, 2, 4, 1, 3, 8, 4, 2, 0, 8, 1, 4, 8, 8, 3, 8, 1, 7, 8, 9, 3, 6, 8 },
	{ 5, 3, 0, 1, 5, 3, 3, 2, 5, 4, 0, 1, 2, 5, 7, 9, 4, 1, 1, 6, 5, 2, 2, 6, 1, 2, 4 },
	{ 4, 7, 2, 0, 1, 4, 5, 0, 3, 0, 4, 5, 7, 4, 9, 8, 5, 8, 7, 3, 5 ,4, 4, 2, 8, 9, 3 },
	{ 2, 6, 3, 6, 0, 1, 2, 4, 6, 4, 6, 2, 3, 6, 2, 8, 3, 1, 1, 7, 2, 6, 5, 8, 6, 3, 2 },
	{ 2, 4, 2, 0, 1, 0, 3, 2, 2, 3, 4, 4, 9, 9, 8, 7, 5, 6, 5, 2, 2, 6, 6, 3, 3, 5, 5 },
	{ 7, 0, 2, 4, 1, 1, 0, 2, 4, 1, 7, 6, 1, 0, 5, 6, 7, 8, 2, 5, 3, 2, 3, 6, 2, 3, 3 },
	{ 1, 0, 1, 1, 2, 6, 4, 0, 3, 0, 9, 2, 4, 2, 1, 4, 3, 1, 3, 8, 4, 8, 7, 2, 4, 8, 4 },
	{ 1, 1, 1, 1, 4, 6, 3, 3, 0, 5, 6, 4, 5, 9, 5, 8, 4, 8, 7, 7, 5 ,2, 4, 2, 9, 5, 7 },
	{ 6, 3, 1, 9, 3, 1, 6, 3, 1, 0, 3, 2, 1, 1, 6, 3, 3, 5, 4 ,1, 2, 5, 1, 5, 4, 9 ,8 },
	{ 4, 3, 4, 9, 2, 1, 6, 9, 0, 2, 0, 6, 5, 4, 2, 9, 2, 1, 5, 6, 7, 2, 6, 2, 2, 4, 3 },
	{ 6, 5, 1, 3, 3, 1, 3, 3, 1, 1, 3, 0, 3, 7, 8, 6, 6, 9, 5, 3, 3, 1, 7, 3, 8, 2, 4 },
	{ 4, 6, 1, 8, 4, 8, 6, 4, 8, 1, 2, 7, 0, 3, 2, 5, 0, 7, 2, 7, 3, 5, 9, 9, 5, 5, 5 },
	{ 6, 2, 1, 9, 3, 7, 6, 5, 2, 0, 7, 1, 1, 0, 4, 2, 2, 7, 8, 5, 5, 7, 8, 5, 3, 9, 3 },
	{ 9, 7, 2, 8, 3, 8, 1, 4, 6, 1, 4, 6, 7, 6, 0, 1, 0, 8, 5, 2, 3, 0, 5, 2, 2, 1, 3 },
	{ 5, 4, 3, 5, 6, 3, 2, 3, 5, 3, 3, 3, 2, 4, 4, 0, 3, 1, 1, 6, 3, 5, 5, 3, 6, 7, 2 },
	{ 3, 9, 2, 1, 2, 3, 2, 5, 1, 3, 3, 5, 1, 3, 3, 5, 0, 1, 5, 2, 9, 7, 7, 3, 4, 4, 5 },
	{ 4, 8, 8, 8, 1, 5, 6, 6, 8, 2, 6, 6, 7, 7, 2, 2, 5, 0, 2, 7, 3, 6, 7, 4, 3, 9, 3 },
	{ 7, 6, 8, 4, 6, 5, 7, 7, 9, 5, 0, 5, 6, 7, 6, 5, 2, 2, 0, 7, 4, 8, 8, 4, 9, 4, 7 },
	{ 8, 3, 4, 3, 2, 3, 1, 2, 9, 3, 1, 3, 9, 1, 2, 1, 3, 6, 4, 0, 4, 1, 3, 2, 2, 7, 5 },
	{ 8, 5, 8, 8, 6, 3, 4, 5, 8, 7, 3, 5, 3, 6, 9, 5, 4, 6, 9, 6, 0, 8, 6, 8, 1, 4, 2 },
	{ 9, 6, 6, 9, 7, 8, 2, 3, 3, 5, 2, 6, 5, 4, 1, 2, 5, 8, 9, 8, 4, 0, 3, 5, 5, 7, 2 },
	{ 0, 2, 2, 3, 4, 2, 1, 5, 2, 4, 8, 9, 8, 4, 3, 5, 4, 7, 6, 4, 4, 6, 0, 7, 3, 3, 3 },
	{ 9, 4, 6, 5, 7, 6, 7, 3, 6, 6, 8, 1, 2, 7, 5, 8, 5, 2, 3, 9, 1, 5, 6, 0, 8, 2, 7 },
	{ 1, 4, 3, 4, 7, 3, 6, 9, 9, 6, 7, 6, 2, 4, 6, 4, 8, 6, 7, 3, 5, 3, 3, 5, 0, 7, 3 },
	{ 2, 5, 3, 7, 6, 3, 6, 3, 6, 9, 9, 6, 8, 3, 6, 3, 4, 6, 3, 6, 1, 4, 9, 4, 6, 0, 3 },
	{ 5, 6, 4, 3, 4, 6, 2, 4, 1, 3, 5, 1, 2, 4, 7, 5, 8, 4, 3, 4, 8, 2, 4, 4, 5, 3, 0 }
};

typedef struct ___cost_entry_t___
{
	cost_t cost;
	vertex_t last_vertex;
} cost_entry_t;

cost_entry_t cost_matrix[N][1U << N];

//Initialize the cost matrix from the dist matrix:
void init_cost_matrix()
{
	//For each subset { vi }, we insert C(vi, d(v*, vi)):
	for (vertex_t vert = 0; vert < N; vert++)
	{
		cost_matrix[vert][1U << vert] = (cost_entry_t){ .cost = dist_matrix[0][vert + 1] };
	}
}

//Handle a given subset:
void handle_subset(subset_t subset)
{
	//Iterate over the subset.
	//Check out all subsets where one of the vertices is removed.
	//For those, we have already solved the problem.
	for (vertex_t omitted_vertex = 0; omitted_vertex < N; omitted_vertex++)
	{
		//Mask out the relevant cases:
		subset_t mask = (1U << omitted_vertex);

		if (!(subset & mask))
		{
			continue;
		}

		//Remove the vertex we have to omit from the subset:
		subset_t curr_subset = subset & ~(mask);

		//For each vertex vi in the subset, we already know the minimum distance of the path
		// that starts at v*, ends at vi and passes along all the other vertices inside.
		//By adding the weight of the edge from vi to the omitted vertex, we can search for the new minimum.
		vertex_t best_end_vertex;
		cost_t best_cost = COST_MAX;

		for (vertex_t end_vertex = 0; end_vertex < N; end_vertex++)
		{
			//Mask out the relevant cases:
			if (!(curr_subset & (1U << end_vertex)))
			{
				continue;
			}

			//Get the current cost:
			cost_t curr_cost = cost_matrix[end_vertex][curr_subset].cost + dist_matrix[end_vertex + 1][omitted_vertex + 1];

			//Is it an improvement?
			if (curr_cost < best_cost)
			{
				best_end_vertex = end_vertex;
				best_cost = curr_cost;
			}
		}

		//Insert the new value:
		cost_matrix[omitted_vertex][subset] = (cost_entry_t){ .cost = best_cost, .last_vertex = best_end_vertex };
	}
}

//Generate the next subset from a given one:
subset_t next_subset(subset_t curr_subset)
{
	//Get the next permutation with the same number of one bits.
	//Thanks to: https://graphics.stanford.edu/~seander/bithacks.html#NextBitPermutation
#ifdef USE_CTZ_MACRO
	subset_t temp = curr_subset | (curr_subset - 1);
	return (temp + 1) | (((~temp & -~temp) - 1) >> (__builtin_ctz(curr_subset) + 1));  
#else
	subset_t temp = (curr_subset | (curr_subset - 1)) + 1;
	return temp | ((((temp & -temp) / (curr_subset & -curr_subset)) >> 1) - 1);
#endif
}

//Handle all subsets [start_set, end_set].
//Offset and step allow parallelization by interleaving the subsets.
void handle_subsets_in_range(subset_t start_set, subset_t end_set, int offset, int step)
{
	//First, consume the offset:
	for (int i = 0; i < offset; i++)
	{
		if (start_set == end_set)
		{
			return;
		}

		start_set = next_subset(start_set);
	}

	//Now start the actual work:
	while (1)
	{
		//Handle the current subset:
		handle_subset(start_set);

		//Perform step(s):
		for (int i = 0; i < step; i++)
		{
			if (start_set == end_set)
			{
				return;
			}

			start_set = next_subset(start_set);
		}
	}
}

//Handle all the subsets in order:
void handle_subsets()
{
	//By iterating over the number of vertices per set, we can make sure
	// all dependencies have already been satisfied.
	//That allows to iterate in parallel.
	for (int number_of_vertices = 2; number_of_vertices <= N; number_of_vertices++)
	{
		//Generate the limits for this step:
		subset_t start_set = (1U << number_of_vertices) - 1;
		subset_t end_set = start_set << (N - number_of_vertices);

		printf("|V| = %d ...\n", number_of_vertices);

		//Handle the subsets in an interleaved manner.
		//Every thread uses its thread number as offset.
#pragma omp parallel
		handle_subsets_in_range(start_set, end_set, omp_get_thread_num(), omp_get_num_threads());
	}
}

int main(void)
{
	//Initialize the cost matrix using the distance matrix:
	init_cost_matrix();

	//Handle all the subsets step by step.
	//Each step uses the knowledge retrieved on the previous step.
	handle_subsets();

	//Okay, all subsets have been handled.
	//Now search for the minimum including the last edge:
	vertex_t best_end_vertex;
	cost_t best_cost = COST_MAX;
	subset_t subset = (subset_t)(1U << N) - 1;

	for (vertex_t end_vertex = 0; end_vertex < N; end_vertex++)
	{
		//Get the current cost:
		cost_t curr_cost = cost_matrix[end_vertex][subset].cost + dist_matrix[end_vertex + 1][0];

		//Is it an improvement?
		if (curr_cost < best_cost)
		{
			best_end_vertex = end_vertex;
			best_cost = curr_cost;
		}
	}

	//Start printing the path:
	printf("Roundtrip: 0");

	if (best_cost < COST_MAX)
	{
		while (1)
		{
			//Print the best end vertex:
			printf(" <- %d", best_end_vertex + 1);

			//If the subset only contains the best end vertex, we are done.
			//Otherwise, we trace for the next one:
			subset_t mask = (1U << best_end_vertex);

			if (subset == mask)
			{
				break;
			}

			best_end_vertex = cost_matrix[best_end_vertex][subset].last_vertex;

			//Remove the best end vertex from the set:
			subset &= ~mask;
		}

		printf(" <- 0");
	}
	else
	{
		best_cost = 0;
	}

	printf("\n");

	//Print the total cost:
	printf("Total cost: %d\n", (int)best_cost);

	return 0;
}
