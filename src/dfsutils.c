// #include "stdio.h"
// #include "stdlib.h"
// #include "string.h"
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

typedef struct {
	int count;
	vertex **marked;
} *marker;

// Vertex sort

void ptrswap(vertex **a, vertex **b)
{
	vertex *temp = *a;
	*a = *b;
	*b = temp;
}

void vsortadj(vertex **v)
{
	int i, j, min;
	vertex *p, **temp;
	
	p = *v;
	for (i = 0; i < p->adj_count - 1; i++)
	{
		min = i;
		for (j = i + 1; j < p->adj_count; j++)
			if (strcmp(p->adjacents[j]->name, p->adjacents[min]->name) < 0)
				min = j;
		ptrswap(&p->adjacents[min], &p->adjacents[i]);
	}
}

// Vertex operations

void vnew(vertex *v, char *name)
{
	int len = 1 + snprintf(NULL, 0, "%s", name);
	v->name = malloc(len);
	snprintf(v->name, len, "%s", name);
	v->adj_count = 0;
	v->adjacents = NULL;
}

int vfindadj(vertex *v, vertex *key)
{
	int i, r;
	for (i = 0, r = 0; !r && i < v->adj_count; i++)
		r = v->adjacents[i] == key;
	return r;
}

void vconnect(vertex *v1, vertex *v2)
{
	int i;
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

// Stack operations

void snew(stack *s) { *s = calloc(1, sizeof(struct _stack)); }

int snone(stack s) { return 0 == s->_NOMODIFY_COUNT; }

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

void makemarker(marker *m)
{
	*m = calloc(1, sizeof(*m));
	(*m)->count = 0;
	(*m)->marked = NULL;
}

void addmark(marker *m, vertex *v)
{
	void *ra;
	marker md = *m;
	ra = realloc(md->marked, ++md->count * sizeof(vertex *));
	md->marked = !ra ? md->marked : ra;
	md->marked[md->count - 1] = v;
}

int marked(marker m, vertex *v)
{
	int i, r = 0;
	if (m->marked != NULL)
		for (i = 0; !r && i < m->count; i++)
			r = m->marked[i] == v;
	return r;
}
