#include "dfsutils.c"
#include "crtdbg.h"

void dfs(vertex *from)
{
	int i;
	vertex *_;

	stack s;
	marker m;

	snew(&s);
	makemarker(&m);

	spush(&s, from);
	while (!snone(s) && s != NULL && m != NULL)
	{
		spop(&s, &_);
		if (!marked(m, _))
		{
			addmark(&m, _);
			printf("%s\n", _->name);
			for (i = _->adj_count; i-- > 0;)
				spush(&s, _->adjacents[i]);
		}
	}

	sdest(&s);
	destroymarker(m);
}

/*
int main()
{
	stack s = NULL;
	marker m = NULL;
	vertex v1, v2, v3, v4;

	vnew(&v1, "Bruce");
	vnew(&v2, "Diana");
	vnew(&v3, "Clark");
	vnew(&v4, "Hal");

	vconnect(&v1, &v2); // B->D
	vconnect(&v2, &v3); // D->C
	vconnect(&v3, &v4); // C->H
	vconnect(&v4, &v2); // H->D

	dfs(&v3);

	vremove(&v1);
	vremove(&v2);
	vremove(&v3);
	vremove(&v4);

	_CrtDumpMemoryLeaks();
}
*/