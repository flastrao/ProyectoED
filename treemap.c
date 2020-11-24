#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "treemap.h"

typedef struct TreeNode TreeNode;

struct TreeNode {
    void* key;
    void * value;
    TreeNode * left;
    TreeNode * right;
    TreeNode * parent;
};

struct TreeMap {
    TreeNode * root;
    TreeNode * current;
    int (*lower_than) (void* key1, void* key2);
};

int is_equal(TreeMap* tree, void* key1, void* key2){
    if(tree->lower_than(key1,key2)==0 &&  
        tree->lower_than(key2,key1)==0) return 1;
    else return 0;
}


TreeNode * createTreeNode(void* key, void * value) {
    TreeNode * new = (TreeNode *)malloc(sizeof(TreeNode));
    if (new == NULL) return NULL;
    new->key = key;
    new->value = value;
    new->parent = new->left = new->right = NULL;
    return new;
}

TreeMap * createTreeMap(int (*lower_than) (void* key1, void* key2)){
    TreeMap * new = (TreeMap *)malloc(sizeof(TreeMap)); //Se crea arbol y se le asigna memoria
    new->lower_than = lower_than;
    return new; //Se retorna el arbol
}

void insertTreeMap(TreeMap * tree, void* key, void * value) {
    if(tree == NULL) return;

    TreeNode * aux = tree->root;
    TreeNode * parent = NULL;

    while(aux != NULL){
      parent = aux;
      if(tree->lower_than(key, aux->key)){ 
        aux = aux->left;
      }
      else if(tree->lower_than(aux->key, key)){
        aux = aux->right;
      }
      else{
        return;
      }
    }

    TreeNode * nuevo = createTreeNode(key, value);
    nuevo->parent = parent;

    if(parent == NULL){
      tree->root = nuevo;
    }
    else if(tree->lower_than(nuevo->key, parent->key)){
      parent->left = nuevo;
    }
    else{
      parent->right = nuevo;
    }
    tree->current = nuevo;
}

TreeNode * minimum(TreeNode * x){
    if(x == NULL) return NULL; //Si el arbol no existe retorna NULL

    TreeNode * aux = x; //Se inicia aux

    while(aux != NULL){
        if(aux->left == NULL){
         return aux; //Si no tiene un nodo hijo al lado izquierdo retorna el mismo nodo
        }
        else{
          aux = aux->left; //Si tiene un nodo hijo entonces sigue bajando por la izquierda
        }
    }
    return NULL;
}

void removeNode(TreeMap * tree, TreeNode* node) {
    if(tree == NULL) return ; //Si el arbol es NULL retorna NULL

    if(node->left == NULL && node->right == NULL){ //Si el nodo no tiene hijos se le asigna NULL a los punteros del padre que apuntaban al nodo
      node->parent->right = NULL;
      node->parent->left = NULL;
    }

    if(node->left != NULL && node->right == NULL){
      if(node->parent->right == node){
        node->parent->right = node->left; //El puntero del padre ahora apunta al hijo del nodo
        node->left->parent = node->parent; //Se actualiza el parent del hijo del nodo para que apunte al parent del nodo
      }
    }

    if(node->left != NULL && node->right != NULL){
      TreeNode * minimo = minimum(node->right); //Se crea variable minimo que contiene el minimo del subarbol
      node->value = minimo->value; //Se le asigna a el nodo los datos del minimo
      node->key = minimo->key;
      removeNode(tree, minimo); //Se elimina el minmimo
    }
}

void eraseTreeMap(TreeMap * tree, void* key){
    if (tree == NULL || tree->root == NULL) return;

    if (searchTreeMap(tree, key) == NULL) return;
    TreeNode* node = tree->current;
    removeNode(tree, node);

}

void * searchTreeMap(TreeMap * tree, void* key) {
    if(tree == NULL) return NULL; //Si el arbol no existe se retorna NULL

    TreeNode * aux = tree->root; //Se inicia aux

    while(aux != NULL){

      if(tree->lower_than(key, aux->key)){ 
        aux = aux->left; //Si la key del root es mayor entonces avanza para la izquierda

      }
      else if(tree->lower_than(aux->key, key)){
        aux = aux->right; //Si la key que nos ingresan es mayor que la key del root entonces avanza hacia la derecha
      }
      else{ //Para cualquier otro caso significa que las keys son iguales
        tree->current = aux; //Se actualiza current
        return aux->value;
      }
    }
    return NULL;
}


void * upperBound(TreeMap * tree, void* key) {
    if(tree == NULL) return NULL; //Si el arbol no existe retorna NULL
    
    TreeNode * ub_node = tree->root; //Va guardando el menor nodo mayor a key

    TreeNode * aux = tree->root; //Se inicia aux

    while(aux != NULL){

      if(tree->lower_than(key, aux->key)){
        if(aux->key >= ub_node->key){
          ub_node = aux; //Si la key del nodo donde estamos parado es mayor o igual a la key de ub_node entonces se le asigna a ub_node el nodo
        }
        aux = aux->left; //Si la key del root es mayor entonces avanza para la izquierda
      }
      else if(tree->lower_than(aux->key, key)){
        if(aux->key >= ub_node->key){
          ub_node = aux;//Si la key del nodo donde estamos parado es mayor o igual a la key de ub_node entonces se le asigna a ub_node el nodo
        }
        aux = aux->right; //Si la key que nos ingresan es mayor que la key del root entonces avanza hacia la derecha
      }
      else{
        return aux->value; //Si encuentra la key retorna el valor del nodo asociado
      }
    }
    return ub_node->value; //Si no encuentra la key se sale del while y retorna el ub_node
}

void * firstTreeMap(TreeMap * tree) {
    TreeNode * aux = tree->root;

    if(tree == NULL) return NULL; //Si no existe un arbol retorna NULL

    while(aux != NULL){
        if(aux->left == NULL){
          return aux->value; //Si no tiene un nodo hijo al lado izquierdo retorna el valor del nodo
        }
        else{
          aux = aux->left; //Si tiene un nodo hijo entonces sigue bajando por la izquierda
        }
    }
    return NULL;
}

void * nextTreeMap(TreeMap * tree) {
    if(tree == NULL) return NULL; //Si el arbol es nulo retorna NULL
    if(tree->current == NULL) return NULL; //Si current en NULL retorna NULL
    return tree->current->right->value; //Se retorna el nodo siguiente de forma trucha
}
