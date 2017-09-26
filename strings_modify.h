#ifndef __STRINGS_MODIFY_H__
#define __STRINGS_MODIFY_H__

#include "create_hashtable.h"

//Nodo de la lista.
typedef struct LSNode {
  char* word;
  struct LSNode* next;
} LSNode;

//Lista.
typedef LSNode* SList;

//Funciones de la lista de sugerencias.
SList slist_create();
SList slist_add(SList sl, char* s);
SList suggestion_list_create(HashTable *ht, char* s);
void slist_destroy(SList sl);
void slist_fprint(FILE *file, SList sl);

//Funciones sobre strings.
char* swap_chars(char*, int index, int length);
char* insert_char(char*, int index, int character, int length);
char* delete_char(char*, int index, int length);
char* replace_char(char*, int index, int character, int length);
char** separate_word(char*, int index, int length);

#endif /* __STRINGS_MODIFY_H__ */