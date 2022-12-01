#ifndef B_TREE
#define B_TREE

typedef struct Node{
    int* values, count;
    struct Node** links;
}T_node;

typedef struct B_tree
{
    T_node* root;
    int order;
    int height;
}B_tree;

int init_btree(B_tree** tree, int order);
int init_node(T_node** node, int order);
void display_btree(B_tree* tree, T_node* parent_node);
void display_btree_by_level(B_tree* tree, T_node* parent_node, int level, char** s);

int insert_value(B_tree* tree, T_node* cur_node, int value);
int remove_value(B_tree* tree, T_node* cur_node, int value);
T_node* find_value(B_tree* tree, T_node* cur_node, int value);

#endif