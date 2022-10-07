#ifndef HTABLE_H_
#define HTABLE_H_


struct key_val {
	char *key;
	int val;
};

struct htable {
	int size;
	int curr_elem;
	struct forward_list **table;
};


/*
 * Function Declarations
 */
void htable_init(struct htable *table, const int num);
void htable_free(struct htable *table);

void htable_add(struct htable *table, const struct key_val *data);
void htable_add_arr(struct htable *table, const struct key_val *arr, const int num);

int htable_at(struct htable *table, const char *key);


#endif /* HTABLE_H_ */
