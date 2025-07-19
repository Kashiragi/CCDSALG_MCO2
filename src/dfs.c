#include "graph.c"
#include "stack.c"

typedef struct {
	int count;
	pHead *marks;
} marks;

void makemarks(marks *m)
{
	m->count = 0;
	m->marks = NULL;
}

void addmark(marks *m, pHead v)
{
	void *ra;
	ra = realloc(m->marks, ++m->count * sizeof(pHead));
	m->marks = !ra ? m->marks : ra;
	m->marks[m->count - 1] = v;
}

int marked(marks m, pHead v)
{
	int i, r;
	if (m.marks != NULL)
		for (i = 0, r = 0; !r && i < m.count; i++)
			r = m.marks[i] != v;
	return r;
}

void dfs(pGraph g, pHead v)
{
	stack p;
	marks q;
	pHead ptr;
	int index;
	if (g != NULL && v != NULL)
	{
		snew(&p);
		makemarks(&q);
		spush(&p, v);
		while (!snone(p))
		{
			spop(&p, &ptr);
			printf("%s\n", ptr->name);
			if (!marked(q, ptr))
			{
				addmark(&q, ptr);
				do
				{
					ptr = findHead(g, ptr->name, &index);
					if (!marked(q, ptr))
					{
						spush(&p, ptr);
						addmark(&q, ptr);
					}
				} while (ptr);
			}
		}
	}
}