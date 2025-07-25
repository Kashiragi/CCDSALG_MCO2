#include "globals.h"
typedef struct _vertex
{
	char *name;
	int adj_count;
	struct _vertex **adjacents;
} vertex;

typedef struct _stack
{
	int _NOMODIFY_COUNT;
	vertex **v;
	struct _stack *next;
} *stack;

typedef struct _marker {
	int count;
	vertex **marked;
} *marker;

// Vertex sort
/**
 * 	Swaps two vertices. Helper function to sort vertices.
 * 	@author Kurt Laguerta
 * 	@param a first vertex
 * 	@param b second vertex.
 */
void vptrswap(vertex **a, vertex **b)
{
	vertex *temp = *a;
	*a = *b;
	*b = temp;
}
/**
 * Sets to null/cleans the references of the specified vertex's
 * adjacent vertices
 * @author Kurt Laguerta
 * @param v pointer to the vertex whose adjacents should be cleaned/set to NULL
 */
void vnullref(vertex *v)
{
	int i, j, c;
	vertex **adjs;

	if (v->adjacents != NULL)
	{
		for (i = 0, c = 0; i < v->adj_count; i++)
			c += (NULL == v->adjacents[i]);

		adjs = malloc((v->adj_count - c) * sizeof(vertex *));
		for (i = 0, j = 0; i < v->adj_count; i++)
			if (v->adjacents[i] != NULL)
				adjs[j++] = v->adjacents[i]; else;

		v->adj_count -= c;
		free(v->adjacents);
		v->adjacents = NULL;
		if (v->adj_count > 0)
		{
			v->adjacents = malloc(v->adj_count * sizeof(vertex *));
			memcpy(v->adjacents, adjs, v->adj_count * sizeof(vertex *));
		}
		free(adjs);
	}
}

/**
 * 	Sorts the specified vertex's adjacent vertices or neighbor vertices.
 * 	@author Kurt Laguerta
 * 	@param v pointer to the pointer to the vertex whose adjacent list should be sorted
 */
void vsortadj(vertex **v)
{
	int i, j, min;
	vertex *p;
	
	p = *v;
	for (i = 0; i < p->adj_count - 1; i++)
	{
		min = i;
		for (j = i + 1; j < p->adj_count; j++)
			if (strcmp(p->adjacents[j]->name, p->adjacents[min]->name) < 0)
				min = j; else;
		vptrswap(&p->adjacents[min], &p->adjacents[i]);
	}
}

// Vertex operations
/**
 * Puts a new vertex in the graph
 * @author Kurt Laguerta
 * @param v Pointer to a vertex variable. Ensure it is an unitialized vertex
 * @param name Name that labels the vertex
 */
void vnew(vertex *v, char *name)
{
	int len = 1 + snprintf(NULL, 0, "%s", name);
	v->name = malloc(len);
	snprintf(v->name, len, "%s", name);
	v->adj_count = 0;
	v->adjacents = NULL;
}

/**
 * Checks if the given vertex key is adjacent to the given vertex v.
 * @author Kurt Laguerta
 * @param v Pointer to a vertex variable. Ensure it is an unitialized vertex
 * @param key the name to be searched
 * @return a number if found; otherwise, 0
 */
int vfindadj(vertex *v, vertex *key)
{
	int i, r;
	for (i = 0, r = 0; !r && i < v->adj_count; i++)
		r = v->adjacents[i] == key;
	return r;
}

/**
 * Undirectedly connects two vertices v1 and v2 together, regardless of parameter order.
 * @author Kurt Laguerta
 * @param v1 vertex 1 to connect to v2
 * @param v2 vertex 2 to connect to v1
 */
void vconnect(vertex *v1, vertex *v2)
{
	if (!vfindadj(v1, v2) && !vfindadj(v2, v1))
	{
		v1->adjacents = realloc(v1->adjacents, ++v1->adj_count * sizeof(vertex));
		v1->adjacents[v1->adj_count - 1] = v2;
		v2->adjacents = realloc(v2->adjacents, ++v2->adj_count * sizeof(vertex));
		v2->adjacents[v2->adj_count - 1] = v1;
		vsortadj(&v1);
		vsortadj(&v2);
	}
	else if (!vfindadj(v1, v2))
	{
		v1->adjacents = realloc(v1->adjacents, ++v1->adj_count * sizeof(vertex));
		v1->adjacents[v1->adj_count - 1] = v2;
		vsortadj(&v1);
	}
	else
	{
		v2->adjacents = realloc(v2->adjacents, ++v2->adj_count * sizeof(vertex));
		v2->adjacents[v2->adj_count - 1] = v1;
		vsortadj(&v2);
	}
}

/**
 * Deallocates a vertex
 * @author Kurt Laguerta
 * @param v1 the vertex to be removed
 */
void vremove(vertex *v1)
{
	int i, j;
	if (v1->adj_count > 0)
	{
		for (i = 0; i < v1->adj_count; i++)
		{
			if (v1->adjacents[i] != v1)
			{
				for (j = 0; j < v1->adjacents[i]->adj_count; j++)
					if (v1->adjacents[i]->adjacents[j] == v1)
						v1->adjacents[i]->adjacents[j] = NULL; else;
				vnullref(v1->adjacents[i]);
			}
		}
		free(v1->adjacents);		
	}
	free(v1->name);
	v1->adj_count = 0;
	v1->adjacents = NULL;
	v1->name = NULL;
}

// Stack operations
// Derived from MCO1
// @author Kurt Laguerta

/* Creates a stack */
void snew(stack *s) { *s = calloc(1, sizeof(struct _stack)); }

/* Check if stack is empty or not */
int snone(stack s) { return 0 == s->_NOMODIFY_COUNT; }

/* Push vertex h to top of stack*/
void spush(stack *s, vertex *h)
{
	stack d, u;
	if (s != NULL && *s != NULL)
	{
		d = *s;
		if (snone(d))
		{
			d->v = malloc(sizeof(vertex *));
			*d->v = h;
			d->_NOMODIFY_COUNT++;
		}
		else
		{
			snew(&u);
			u->v = malloc(sizeof(vertex *));
			*u->v = h;
			u->_NOMODIFY_COUNT = d->_NOMODIFY_COUNT + 1;
			u->next = d;
			*s = u;
		}
	}
}

/* Pop vertex from stack and store in o*/
void spop(stack *s, vertex **o)
{
	stack d, t;
	if (s != NULL && *s != NULL)
	{
		d = *s;
		if (!snone(d))
		{
			if (1 == d->_NOMODIFY_COUNT)
			{
				*o = *d->v;
				free(d->v);
				d->_NOMODIFY_COUNT--;
				d->next = NULL;
			}
			else
			{
				t = d->next;
				*o = *d->v;
				free(d->v);
				free(d);
				*s = t;
			}
		}
	}
}

/* Destroy stack */
void sdest(stack *s)
{
	vertex *_;
	if (s != NULL && *s != NULL)
	{
		while (!snone(*s))
			spop(s, &_);
		free(*s);
		*s = NULL;
	}
}

// Marker operations
// @author Kurt Laguerta
/** 
 * Create marker 
 * @author Kurt Laguerta
 * @param m pointer to the marker
*/
void makemarker(marker *m)
{
	*m = calloc(1, sizeof(struct _marker));
	(*m)->count = 0;
	(*m)->marked = NULL;
}

/**
 * Marks vertex as visited
 * @author Kurt Laguerta
 * @param m pointer to the marker
 * @param v vertex to be marked visited
 */
void addmark(marker *m, vertex *v)
{
	void *ra;
	marker md = *m;
	ra = realloc(md->marked, ++md->count * sizeof(vertex *));
	md->marked = !ra ? md->marked : ra;
	md->marked[md->count - 1] = v;
}

/**
 * Checks if vertex has been marked visited
 * @author Kurt Laguerta
 * @param m marker with information of visited
 * @param v vertex to be checked
 * @returns a number if marked; otherwise, 0
 */
int marked(marker m, vertex *v)
{
	int i, r = 0;
	if (m->marked != NULL)
		for (i = 0; !r && i < m->count; i++)
			r = m->marked[i] == v;
	return r;
}

/**
 * Destroy marker
 * @author Kurt Laguerta
 * @param m marker to be destroyed
 */
void destroymarker(marker m)
{
	vertex **arr = m->marked;
	free(m);
	free(arr);
}

/**
 * Converts information from pGraph to vertex
 * @author Kurt Laguerta
 */
int varrfind(vertex *arr, int count, char *key)
{
	int i, f;
	for (i = 0, f = 0; !f && i < count; i++)
		f = 0 == strcmp(arr[i].name, key);
	f = 0 == f ? -1 : i - 1;
	return f;
}

/**
 * Converts a graph concept into a vertex concept.
 * @author Kurt Laguerta
 * @param v Where the set of vertices from the graph is stored into. Ensure this parameter is a variable assigned to NULL.
 * @param g the graph to convert
 */
void vfromgraph(vertex **v, pGraph g)
{
	int count, head_index, adjacent_index;
	pHead head;
	pVertex adjacent;
	if (g != NULL)
	{
		count = 0;
		head = g->heads;
		*v = malloc(g->nV * sizeof(vertex));
		while (head != NULL)
		{
			head_index = varrfind(*v, count, head->name);
			adjacent = head->list;
			if (-1 == head_index)
				vnew(&(*v)[count++], head->name), head_index = count - 1; else;
			while (adjacent != NULL)
			{
				adjacent_index = varrfind(*v, count, adjacent->name);
				if (-1 == adjacent_index)
					vnew(&(*v)[count++], adjacent->name), adjacent_index = count - 1; else;
				vconnect(&(*v)[head_index], &(*v)[adjacent_index]);
				adjacent = adjacent->next;
			}
			head = head->nextHead;
		}
	}
}
