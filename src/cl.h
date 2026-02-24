#ifndef __CHAINED_LIST_H__
#define __CHAINED_LIST_H__ 1

struct sChainedCell {
	void * value;
	struct sChainedCell * next;
};
typedef struct sChainedCell * ChainedCell;

struct sChainedList {
	int entries;
	ChainedCell entry;
};
typedef struct sChainedList * ChainedList;

extern ChainedList cl_create();
extern ChainedList cl_append(ChainedList, void *);
extern int cl_remove(ChainedList, void *, int cmp(void *, void *), void destroy(void *));
extern int cl_get_nth(ChainedList, int, void *, size_t);
extern void cl_destroy(ChainedList *, void free(void *));
extern int cl_find(ChainedList, void *, size_t, int cmp(void *));
extern int cl_find_template(ChainedList, void *, size_t, void *, int cmp(void *, void *));

#endif
