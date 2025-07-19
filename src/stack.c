#include "graph.c"
#include "stdlib.h"

typedef struct _stack
{
	int _NOMODIFY_COUNT;
	pHead vertex;
	struct _stack *next;
} *stack;

void snew(stack *s) { *s = calloc(1, sizeof(stack)); }

int snone(stack s) { return 0 == s->_NOMODIFY_COUNT; }

void spush(stack *s, pHead h)
{
	stack d, u;
	if (s != NULL && *s != NULL && h != NULL)
	{
		d = *s;
		if (snone(d))
		{
			d->_NOMODIFY_COUNT++;
			d->vertex = h;
		}
		else
		{
			snew(&u);
			u->_NOMODIFY_COUNT = d->_NOMODIFY_COUNT + 1;
			u->vertex = h;
			u->next = d;
			*s = u;
		}
	}
}

void spop(stack *s, pHead *o)
{
	stack d, t;
	if (s != NULL && *s != NULL)
	{
		d = *s;
		if (!snone(d))
		{
			if (1 == d->_NOMODIFY_COUNT)
			{
				t = d->next;
				*o = d->vertex;
				free(d);
				*s = t;
			}
		}
	}
}

void sdest(stack *s)
{
	pHead _;
	if (s != NULL && *s != NULL)
	{
		while (!snone(*s))
			spop(s, &_);
		free(*s);
		*s = NULL;
		s = NULL;
	}
}
