#include "dfsutils.c"
// #include "crtdbg.h"

void dfs(vertex *from, FILE *writeArea)
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
			fprintf(writeArea, "%s ", _->name);
			for (i = _->adj_count; i-- > 0;)
				spush(&s, _->adjacents[i]);
		}
	}

	sdest(&s);
	destroymarker(m);
}

/*
void dfs(vertex *from, pGraph g)
{
	int i, j=0, index;
	vertex *_;
	
	stack s;
	marker m;
	pHead curHead = g->heads;
	pHead h;
	pVertex curVer;
	// storing all vertices
	vertex *vertices = calloc(g->nV, sizeof(char[MAX_ID_LEN+1]));
	
	if((curHead = findHead(g, from->name, &index))==NULL || g->heads==NULL) return;
	
	// since each head is a vertice, store that
	while(curHead!=NULL){
		// create a new vertice, in sequence, using the name in the GDS
		vnew(&vertices[j], curHead->name);
		j++;
	}
	// reset values
	curHead = g->heads;
	j=0;
	// go through all heads and connect neighbors
	while(curHead!=NULL){
		curVer = curHead->list;
		while(curVer!=NULL){
			h = findHead(g, curVer->name,&index);
			if(h!=NULL)
				vconnect(&vertices[j], &vertices[index]);
			curVer = curVer->next;
		}
		curHead = curHead->nextHead;
		j++;
	}

	snew(&s);
	makemarker(&m);
	// == you can adjust this i just don't know what to do HUHU
	// since the input from main is string name of start vertex
	// search the Graph for the index of the head (Diana Bruce Clark => Bruce is index 1)
	h = findHead(g,from->name,&index);
	// push the vertice at that index. since we procesed it in sequence earlier, this should work
	spush(&s, &vertices[index]);
	
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
*/

/*
int main()
{
	stack s = NULL;
	marker m = NULL;
	vertex v1, v2, v3, v4;
	pGraph g = gcreate(4);
	pHead h1, h2, h3, h4;

	// vnew(&v1, "Bruce");
	// vnew(&v2, "Diana");
	// vnew(&v3, "Clark");
	// vnew(&v4, "Hal");

	// vconnect(&v1, &v2); // B->D
	// vconnect(&v2, &v3); // D->C
	// vconnect(&v3, &v4); // C->H
	// vconnect(&v4, &v2); // H->D
	// vconnect(&v2, &v1); // D->B

	h1 = addHead(g, "Diana");
	h2 = addHead(g, "Bruce");
	h3 = addHead(g, "Hal");
	h4 = addHead(g, "Clark");
	addAdjacent(h1, "Hal");
	addAdjacent(h1, "Bruce");
	addAdjacent(h1, "Clark");
	addAdjacent(h2, "Diana");
	addAdjacent(h3, "Clark");
	addAdjacent(h3, "Diana");
	addAdjacent(h4, "Hal");
	addAdjacent(h4, "Diana");
	
	dfs(&v3, g);

	vremove(&v1);
	vremove(&v2);
	vremove(&v3);
	vremove(&v4);

	// _CrtDumpMemoryLeaks();
}
*/