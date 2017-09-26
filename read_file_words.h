#ifndef __READF_H__
#define __READF_H__

#define MAX_WORD_LENGTH 100

//Nodo de la lista.
typedef struct LNode {
  char* word;
  int line;
  int pos;
  struct LNode* next;
} LNode;

//Lista de palabras y posiciones.
typedef LNode* WList;

//Funciones sobre la lista.
WList wlist_add_node_at_end(WList list, char* word, int line, int pos);
WList read_file_words(char *file);
void wlist_destroy(WList wl);

int word_separator(char c);


#endif /* __READF_H__ */