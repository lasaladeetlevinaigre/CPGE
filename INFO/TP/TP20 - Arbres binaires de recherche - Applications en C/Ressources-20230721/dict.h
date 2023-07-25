bst *bstdict_create(bst *l, key_type k, elt_type v, bst *r);
void bstdict_free(bst **addr_t);
int  bstdict_number_of_entries(bst *t);
void bstdict_print_inorder(bst *t);
bst *bstdict_insert(key_type k, elt_type v, bst *t);
bool bstdict_find(key_type k, bst *t, node **addr_node);
void bstdict_remove(key_type k, bst *t);
