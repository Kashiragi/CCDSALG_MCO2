/**
 * @file dfs.c
 * @details Contains functions and structs for the graph representation
 * which in this case is an adjacency list
 * @author Kurt Laguerta
 * @tester: Kurt Laguerta, Queenie Salao, Kei Saguin
 */
#include "dfsutils.c"

/**
 *  Goes through the graph's vertices and traverses it using Depth First Search from the 
 *  designated starting vertex.
 *  @author Kurt Laguerta
 *  @param from the name of the starting vertex
 *  @param writeArea the file to output the result of the DFS traversal
 */
void dfs(vertex *from, FILE *writeArea)
{
	int i;
	vertex *_;
	stack s;
	marker m;
	// create stack and marker
	snew(&s);
	makemarker(&m);
	// Push first vertex to stack
	spush(&s, from);
	// continue until stack is empty and all are marked
	while (!snone(s) && s != NULL && m != NULL)
	{
		// Pop top of stack to variable
		spop(&s, &_);
		// If unvisited/unmarked
		if (!marked(m, _))
		{
			// Mark visited
			addmark(&m, _);
			// Output to txt file
			fprintf(writeArea, "%s ", _->name);
			// Push all neighbors of THIS vertex to the stack
			// in sequence of appearance in adjacent list
			for (i = _->adj_count; i-- > 0;)
				spush(&s, _->adjacents[i]);
		}
	}
	// Cleanup
	sdest(&s);
	destroymarker(m);
}