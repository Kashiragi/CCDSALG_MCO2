#include "globals.h"

/**
 * 	Checks if the vertex (specifically, the names) have been visited
 * 	@author Kei Saguin
 * 	@return true if visited; otherwise, false
 */
bool isVisited(char visited[][MAX_ID_LEN+1], int visitedCount, char* name){
    for(int i = 0; i<visitedCount;i++){
        if(strcmp(visited[i], name)==0){
            return true;
        }
    }
    return false;
}

//Queue related
//@author Kurt Laguerta
typedef struct _queue
{
	bool parent;
	char *name;
	struct _queue *behind, *front, *tail;
} *queue;

//create queue
void qnew(queue *qalloc)
{
	assert(*qalloc = calloc(1, sizeof(struct _queue)));
	(*qalloc)->parent = true;
}

//enqueue
void nq(queue *qalloc, queue *qtailref, char *name)
{
	int n;
	queue q, t;
	// Preliminaries.
	// Head of the queue and tail reference pointers both must not be NULL.
	// Head of the queue must never point to NULL and must refer to the parent of the queue.
	if (qalloc != NULL && *qalloc != NULL && qtailref != NULL && (*qalloc)->parent)
	{
		// Dereference parameters to simplify the code.
		q = *qalloc;
		t = *qtailref;
		n = snprintf(NULL, 0, "%s", name) + 1;
		// The name of the head of the queue would be null iff. the queue is empty.
		//
		// If the head of the queue is not null, i.e. there is a name stored within it...
		if (q->name != NULL)
		{
			// Allocate the tail reference and assign the name to it.
			assert(t = calloc(1, sizeof(struct _queue)));
			assert(t->name = calloc(n, sizeof(char)));
			snprintf(t->name, n, "%s", name);
			// The front of the new tail would be the tail prior to this tail being allocated.
			t->front = q->tail;
			// Update the tail reference from the head of the queue and the element in front of the tail.
			// The caveat would be that the middle elements of the queue may no longer have a reference to the tail anymore.
			// Only the head of the queue, the front of the tail, and the tail itself will have a reference pointer to the end of the queue.
			t->front->behind = q->tail = t->front->tail = t->tail = t;
			// In case the new tail is the second element of the queue, the reference pointer behind the head of the queue would be the new tail.
			q->behind = NULL == q->behind ? q->tail : q->behind;
		}
		// Otherwise if the queue is empty, i.e. head name is null...
		else
		{
			// Simply allocate the name into the head of the queue.
			assert(q->name = calloc(n, sizeof(char)));
			snprintf(q->name, n, "%s", name);
			// Set the tail reference to itself.
			q->tail = t = q;
			// Since the element would be alone, there is no reference to the behind pointer as of this moment.
			q->behind = q->front = NULL;
		}
	}
}

//dequeue
void dq(queue *qalloc, char* out)
{
	queue b, t;
	// printf("\nEntry dq\n");
	if (*qalloc != NULL)
	{
		b = (*qalloc)->behind;
		t = (*qalloc)->tail;

		strcpy(out, (*qalloc)->name);
		// printf("Out:%s",out);
		// free((*qalloc)->name);
		//free(*qalloc);
		if (b != NULL)
		{
			free(*qalloc);
			*qalloc = b;
			(*qalloc)->front = NULL;
			(*qalloc)->tail = (NULL == (*qalloc)->behind) ? b : t;
			(*qalloc)->parent = true;
		}
		else
		{
			(*qalloc)->behind = NULL;
			(*qalloc)->front = NULL;
			(*qalloc)->name = NULL;
			(*qalloc)->parent = true;
			(*qalloc)->tail = NULL;
		}
	}
}

//destroy queue
void qdestroy(queue *qalloc)
{
	char out[MAX_ID_LEN+1];
	while ((*qalloc)->name != NULL)
		dq(qalloc, out);
	qalloc = NULL;
}