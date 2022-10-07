#include "htable.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <assert.h>


#define NOT_FOUND -1


struct node {
	struct key_val *data;
	struct node *next;
};

struct forward_list {
	struct node *head;
};


static uint64_t djb2_hash(const char *str);
static void node_init(struct node **node, const struct key_val *data);
static void forward_list_push_front(struct forward_list *list, const struct key_val *data);
static void forward_list_pop_front(struct forward_list *list);
static int forward_list_find(const struct forward_list *list, const char *key);
static void forward_list_free(struct forward_list *list);


void htable_init(struct htable *table, const int num)
{
	assert(num > 0);
	
	table->table = malloc(sizeof(struct forward_list *) * num);
	assert(table->table);

	for (int i = 0; i < num; i++) {
		table->table[i] = calloc(1, sizeof(struct forward_list));
		assert(table->table[i]);
	}

	table->size = num;
	table->curr_elem = 0;
}


void htable_free(struct htable *table)
{
	// free the forward list nodes
	for (int i = 0; i < table->size; i++) 
		forward_list_free(table->table[i]);
	

	// free the pointer to forward list in the arr
	for (int i = 0; i < table->size; i++)
		free(table->table[i]);

	// free the arr
	free(table->table);
}


void htable_add(struct htable *table, const struct key_val *data)
{
	uint64_t index;

	index = djb2_hash(data->key);
	index = index % table->size;

	forward_list_push_front(table->table[index], data);

	table->curr_elem++;
}


void htable_add_arr(struct htable *table, const struct key_val *arr, const int num)
{
	assert(num > 0);

	for (int i = 0; i < num; i++) 
		htable_add(table, &(arr[i]));
}


int htable_at(struct htable *table, const char *key)
{
	uint64_t index;

	assert(key);

	index = djb2_hash(key);
	index = index % table->size;

	return forward_list_find(table->table[index], key);
}


static uint64_t djb2_hash(const char *str)
{
	uint64_t hash = 5381;
	int c;

	while (c = *str++)
	    hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;
}


static void node_init(struct node **node, const struct key_val *data)
{
	*node = malloc(sizeof(struct node));
	assert(*node);

	(*node)->data = data;
	(*node)->next = NULL;
}


static void forward_list_push_front(struct forward_list *list, const struct key_val *data)
{
	struct node *node;
	node_init(&node, data);
	node->next = list->head;
	list->head = node;
}


static void forward_list_pop_front(struct forward_list *list)
{
	if (!list->head)
		return;

	struct node *tmp = list->head->next;
	free(list->head);
	list->head = tmp;
}


static int forward_list_find(const struct forward_list *list, const char *key)
{
	if (!list->head)
		return NOT_FOUND;

	struct node *tmp = list->head;
	while (tmp) {
		if (!strcmp(tmp->data->key, key))
			return tmp->data->val;
		tmp = tmp->next;
	}

	return NOT_FOUND;
}


static void forward_list_free(struct forward_list *list)
{
	while (list->head) 
		forward_list_pop_front(list);
}











