/*
Escreva um programa em C para incluir e pesquisar itens utilizando estrutura de Árvore-B:
    • O Programa terá um menu com opções para inserir e localizar o item selecionado pelo usuário.
    • Não permitir chave duplicada.
    • Exibir mensagem de chave não encontrada caso o item selecionado pelo usuário não for encontrado.

*/

#include <stdio.h>
#include <stdlib.h>
#include "B_Tree/b_tree.h"

#define ORDER 3

#define CLEAR(stream) while(fgetc(stream) != '\n');

int RUNNING = 1;

int main(){
    B_tree* tree;
    T_node* node;
    char op;
    int value;

    init_btree(&tree, ORDER);
    init_node(&tree->root, ORDER);

    do{
        system("clear");
        printf("[1] Inserir elemento\n[2] Remover elemento\n[3] Buscar elemento\n[4] Mostrar Arvore-B\n[0] Sair\n\n");
        op = fgetc(stdin);
        CLEAR(stdin);

        switch(op){
            case '1':
                system("clear");
                printf("\n\n");

                printf("Digite o elemento a ser inserido: ");
                fflush(stdin);
                scanf("%d", &value);
                CLEAR(stdin);

                if(!find_value(tree, NULL, value)){
                    insert_value(tree, NULL, value);
                    printf("\nElemento inserido com sucesso!");
                }     
                else{
                    printf("\nElemento não pode ser inserido, pois já existe na árvore.");
                }

                printf("\n\nEnter para continuar...");
                fflush(stdout);
                CLEAR(stdin);
                break;
           
            case '2':
                system("clear");
                printf("\n\n");

                printf("Digite o elemento a ser removido: ");
                fflush(stdin);
                scanf("%d", &value);
                CLEAR(stdin);

                if(find_value(tree, NULL, value)){
                    remove_value(tree, NULL, value);
                    printf("\nElemento removido com sucesso!");
                }     
                else{
                    printf("\nElemento não pode ser removido, pois não pertence a arvore.");
                }

                printf("\n\nEnter para continuar...");
                fflush(stdout);
                CLEAR(stdin);
                break;
            

            case '3':
                system("clear");
                printf("\n\n");

                printf("Digite o elemento a ser buscado: ");
                fflush(stdin);
                scanf("%d", &value);
                CLEAR(stdin);

                node = find_value(tree, NULL, value);
                if(!node){
                    printf("\nO elemento não foi encontrado na árvore!");
                }     
                else{
                    printf("\nO elemento foi encontrado! No seguinte bloco de dados:\n");
                    printf("(%p): ", (void*) node);

                    for(int i=0; i<node->count; i++)
                        printf(" %d", node->values[i]);
                }

                printf("\n\nEnter para continuar...");
                fflush(stdout);
                CLEAR(stdin);
                break;

            case '4':
                system("clear");
                printf("\n\n");

                display_btree_by_level(tree, NULL, 0, NULL);

                printf("\n\nEnter para continuar...");
                CLEAR(stdin);
                break;

            case '0':
                RUNNING=0;
                break;

            default:
                system("clear");
                printf("\n\nOpção inválida! Enter para continuar...");
                CLEAR(stdin);
                break;
        }
    }while(RUNNING);

    return 0;
}