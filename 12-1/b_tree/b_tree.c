#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "b_tree.h"

int init_btree(B_tree** tree, int order){
    (*tree)=(B_tree*) calloc(1, sizeof(B_tree));

    if(!(*tree))
        return 0;

    if(!(*tree)->root){

        (*tree)->height=1;
        (*tree)->order=order;
        (*tree)->root=NULL;

        return 1;
    }

    return 0;
}

int init_node(T_node** node, int order){
    (*node)=(T_node*) calloc(1, sizeof(T_node));

    if(!(*node))
        return 0;

    (*node)->count=0;
    (*node)->values=(int*) calloc(order, sizeof(int));
    (*node)->links=(T_node**) calloc(order+1, sizeof(T_node*));
}

void display_btree(B_tree* tree, T_node* cur_node){
    int i;

    if(!tree)
        return;
    
    if(!cur_node){
        printf("B-Tree (order %d): ", tree->order);
        fflush(stdout);
        display_btree(tree, tree->root);
        return;
    }
    if(!cur_node->links[0]){
        printf("(");
        if(cur_node->count)
            printf("%d", cur_node->values[0]);

        for(i=1; i<cur_node->count; i++)
            printf(" %d", cur_node->values[i]);

        printf(")");
        fflush(stdout);
        return;
    }

    printf("(");
    fflush(stdout);

    display_btree(tree, cur_node->links[0]);
    fflush(stdout);

    for(i=0; i<cur_node->count; i++){
        printf(" %d ", cur_node->values[i]);
        fflush(stdout);

        display_btree(tree, cur_node->links[i+1]);
        fflush(stdout);
    }

    printf(")");
    fflush(stdout);

}

void display_btree_by_level(B_tree* tree, T_node* cur_node, int level, char** s){
    char aux[15];

    if(!tree)
        return;

    if(!cur_node){
        char** s = (char**) malloc(sizeof(char*)*tree->height+1);
        s[0] = (char*) malloc(sizeof(char)*100);
        sprintf(s[0], "Arvore-B (order %d | height %d):", tree->order, tree->height);

        s[1]= (char*) malloc(sizeof(char)*tree->root->count*10);

        display_btree_by_level(tree, tree->root, 0, s);

        printf(s[0]);
        printf(s[1]);
        return;
    }

    for(int i=0; i<cur_node->count; i++){
        sprintf(aux, "%d ", cur_node->values[i]);
        strcat(s[level+1], aux);
    }
        
}

int insert_value(B_tree* tree, T_node* cur_node, int value){
    int i=0, j, status;

    if(!tree || !tree->root)
        return 0;

    if(!cur_node){
        status = insert_value(tree, tree->root, value);

        if(tree->root->count>=tree->order){
            cur_node = tree->root;
            init_node(&tree->root, tree->order);
            tree->height++;

            tree->root->links[0]=cur_node;

            cur_node = tree->root;
        }
        else{
            return status;
        }
    }
    else{
        for(i=0; i<cur_node->count && cur_node->values[i]<value; i++);

        if(cur_node->links[0]){
            status = insert_value(tree, cur_node->links[i], value);
        }
        else{
            for(j=cur_node->count-1; j>=i; j--)
                cur_node->values[j+1] = cur_node->values[j];

            cur_node->values[i]=value;
            cur_node->count++;
            return 1;
        }
    }

    //repair tree
    if(cur_node->links[i]->count>=tree->order){
        for(j=cur_node->count-1; j>=i; j--){
            cur_node->values[j+1] = cur_node->values[j];
            cur_node->links[j+2] = cur_node->links[j+1];
        }
            
        cur_node->values[i]=cur_node->links[i]->values[(cur_node->links[i]->count-1)/2];
        cur_node->count++;
        
        init_node(&cur_node->links[i+1], tree->order); //memory leak

        for(j=0; j<((cur_node->links[i]->count)-(cur_node->links[i]->count+1)/2); j++)
            cur_node->links[i+1]->values[j]=cur_node->links[i]->values[j+(cur_node->links[i]->count+1)/2];

        if(cur_node->links[i]->links[0]){
            for(j=0; j<=((cur_node->links[i]->count)-(cur_node->links[i]->count+1)/2); j++)
                cur_node->links[i+1]->links[j]=cur_node->links[i]->links[j+(cur_node->links[i]->count+1)/2];

            cur_node->links[i+1]->links[j]=cur_node->links[i]->links[cur_node->links[i]->count];
        }

        cur_node->links[i+1]->count = (cur_node->links[i]->count)/2;
        cur_node->links[i]->count = (cur_node->links[i]->count-1)/2;
        
    }

    return status;  
}

T_node* find_value(B_tree* tree, T_node* cur_node, int value){
    int i;

    if(!tree)
        return NULL;
    
    if(!cur_node)
        return find_value(tree, tree->root, value);
    
    for(i=0; i<cur_node->count && cur_node->values[i]<value ; i++);

    if (i<cur_node->count && cur_node->values[i] == value)
        return cur_node;
    
    if(cur_node->links[0])
        return find_value(tree, cur_node->links[i], value);

    return NULL;
}

int remove_value(B_tree* tree, T_node* cur_node, int value){
    int i, j;
    T_node* node;

    if(!tree)
        return 0;

    if(!cur_node)
        return remove_value(tree, tree->root, value);

    for(i=0; i<cur_node->count && cur_node->values[i]<value; i++);

    if(cur_node->values[i] != value){
        if(!cur_node->links[0])
            return 0;
        
        remove_value(tree, cur_node->links[i], value);
        //To-Do: prevent underflow
    }
    else{
        //To-Do:node found. Remove if it is leaf or non-leaf node
        if(!cur_node->links[0]){ //leaf node
            cur_node->count--;

            for(j=i; j<cur_node->count; j++)
                cur_node->values[i]=cur_node->values[i+i];
        }
        else{ //non-leaf node
            //find smallest greater node
                for(node=cur_node->links[i]; node->links[0]; node=node->links[0]);
            //substitute removed by sgn
                cur_node->values[i]=node->values[0];
            //remove sgn
                remove_value(tree, cur_node, node->values[0]);
            //possibly patch underflow in children
        }
    }
}
