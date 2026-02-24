#include <stdlib.h>
#include <string.h>
#include "cl.h"

ChainedList cl_create() {
	return calloc(1, sizeof(struct sChainedList));
}

void cl_destroy(ChainedList * cl, void free_callback(void *)) {
	ChainedCell cell = (*cl)->entry;

	while (cell != NULL) {
		free_callback(cell->value);
		ChainedCell next = cell->next;

		free(cell);
		cell = next;
	}
	free(*cl);
	*cl = NULL;
}

static ChainedCell create_cell(void * val) {
	ChainedCell cell;
	if ((cell = malloc(sizeof(struct sChainedCell))) == NULL) return NULL;

	cell->next = NULL;
	cell->value = val;

	return cell;
}

ChainedList cl_append(ChainedList cl, void * val) {
	ChainedCell new;
	if ((new = create_cell(val)) == NULL) return NULL;

	if (cl->entries == 0) {
		cl->entries++;
		cl->entry = new;
		return cl;
	}
	ChainedCell before = cl->entry;
	ChainedCell cell = cl->entry;

	while (cell != NULL) {
		before = cell;
		cell = cell->next;	
	}

	cl->entries++;
	before->next = new;
	return cl;
}
int cl_remove(ChainedList cl, void * element, int cmp(void *, void *), void destroy_callback(void *)) {
	ChainedCell before = cl->entry;
	ChainedCell cell = cl->entry;

	while (cell != NULL) {
		if (cmp(cell->value, element)) {
			destroy_callback(cell->value);
			cl->entries--;

			before->next = cell->next;
			free(cell);
			return 1;
		} else {
			before = cell;
			cell = cell->next;
		}
	}

	return 0;
}

int cl_get_nth(ChainedList cl, int n, void * p, size_t t) {
	if (n > cl->entries) return 0;

	ChainedCell cell = cl->entry;
	int i = 0;
	while (i++ < n) {
		cell = cell->next;
	}

	memcpy(p, cell->value, t);
	return 1;
}

int cl_find(ChainedList cl, void * p, size_t t, int cmp(void *)) {
	int i = 0;
	ChainedCell cell = cl->entry;

	while (cell != NULL) {
		if (cmp(cell->value)) {
			memcpy(p, cell->value, t);
			return i;
		} 
		i++;
		cell = cell->next;
	}

	return -1;
}
int cl_find_template(ChainedList cl, void *p, size_t t, void * template, int cmp(void *, void *)) {
	int i = 0;
	ChainedCell cell = cl->entry;

	while (cell != NULL) {
		if (cmp(cell->value, template)) {
			memcpy(p, cell->value, t);
			return i;
		}
		i++;
		cell = cell->next;
	}

	return -1;
}
