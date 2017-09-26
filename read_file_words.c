#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "read_file_words.h"

/* 
 * Agrega un nodo al final de la lista y devuelve la lista.
 */
WList wlist_add_node_at_end(WList list, char* word, int line, int pos) {
  LNode* newNode = malloc(sizeof(LNode));
  newNode->word = malloc(sizeof(char) * MAX_WORD_LENGTH);
  newNode->word = strcpy(newNode->word, word);
  newNode->line = line;
  newNode->pos = pos;
  newNode->next = NULL;

  if (!list)
    return newNode;

  WList node = list;

  for (;node->next != NULL; node = node->next);
  //Ahora 'node' apunta al ultimo elemento en la lista.

  node->next = newNode;

  return list;
}

/* 
 * Determina si el caractér dado es un separador.
 * Definimos separador como cualquier caractér que no forme parte de una
 * palabra del diccionario español.
 */
int word_separator(char c) {
  int es_letra = ((65 <= c) && (c <= 90) || ((97 <= c) && (c <= 122)) || 
           ((48 <= c) && (c <= 57)));

  //Si c es letra, entonces no es un separador.
  if (es_letra) return 0;

  //Lista de excepciones. EJ: á, Á, é, í, ó, ú, ñ.
  int excepciones[] = {-61, -95, -87, -83, -77, -70, -68, -127, 
             -119, -115, -109, -102, -100, -79, -111};
  
  //Comparamos c con las excepciones.
  //Si c es una excepción, entonces no es un separador.
  int i;
  for (i = 0; i < 16; i++) {
    if (c == excepciones[i]) {
      return 0;
    }
  }

  return 1;
}

/*
 * Crea una lista con todas las palabras del archivo dado.
 * Además la lista incluye la línea donde se 
 * encuentra, y la posición en dicha línea.
 */
WList read_file_words(char *fileName) {
  char c;
  char prevChar;
  FILE* file;
  file = fopen(fileName, "r");
  if (!file) return NULL;

  char* word;
  WList wordList = NULL;
  int i, line = 1, pos = 1, posStart = 1;

  //Inicializamos el caracter anterior con un espacio.
  prevChar = ' ';

  while ((c = getc(file)) != EOF) {
    if (c == '\n') {
      line++;
      pos = 1;
      continue;
    }
    //Si el caracter actual es una letra, y el anterior fue un separador...
    if ((!word_separator(c)) && word_separator(prevChar)) {
      //Recordamos la posición inicial, para agregarla luego.
      posStart = pos;
      word = malloc(sizeof(char) * MAX_WORD_LENGTH);
      //Agregamos los caracteres de la palabra a la variable 'word'.
      for (i = 0; ((!word_separator(c)) && (i < MAX_WORD_LENGTH - 1)); i++) {
        
        //Si c es -61 es un carácter doble, 
        //así que no aumentamos la posición.
        if (c != -61) pos++;

        word[i] = c;
        c = getc(file);
      }
      word[i] = '\0';

      //Agregamos la palabra a la lista, 
      //junto con la posición y la línea.
      wordList = wlist_add_node_at_end(wordList, word, line, posStart);
      free(word);//Liberamos la palabra.
    }
    if (c == '\n') {
      line++;
      pos = 1;
      continue;
    }
    //Si c es -62 es un caractér doble, así que
    //tampoco aumentamos la posición en este caso.
    if (c != -62) pos++;
    prevChar = c;
  }

  fclose(file);

  return wordList;
}

/* 
 * Destruye una lista.
 */
void wlist_destroy(WList wl) {
  WList temp;
  while (wl) {
    temp = wl->next;
    free(wl->word);
    free(wl);
    wl = temp;
  }
}