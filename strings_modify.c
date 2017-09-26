#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include <assert.h>
#include "strings_modify.h"
#include "create_hashtable.h"

/* 
 * Devuelve una nueva cadena, que es el resultado de 'intercambiar'
 * el caractér en el índice pasado como argumento, por su adyacente.
 */
char* swap_chars(char* s, int index, int length) {
  int i;
  assert((index < length - 1) && (index >= 0));
  char* newString = malloc(sizeof(char) * length);
  newString = strcpy(newString, s);

  newString[index] = s[index + 1];
  newString[index + 1] = s[index];

  return newString;
}

/* 
 * Devuelve una nueva cadena, que es el resultado de agregar
 * un entero n(que representa una letra de la 'a' a la 'z'),
 * en el índice pasado como argumento.
 */
char* insert_char(char* s, int index, int n, int length) {
  //Nos aseguramos que n sea una letra, y que el index 
  //este en el rango de la cadena.
  assert(((n >= 97) && (n <= 122)) || ((n >= 65) && (n <= 90)));
  assert((index >= 0) && (index <= length));

  char c = n;
  char* newString = malloc(sizeof(char) * length + 1);
  int i;
  for (i = 0; i < index; i++)
    newString[i] = s[i];

  newString[index] = c;

  for (i = index; i < length; i++)
    newString[i + 1] = s[i];

  return newString;
}

/* 
 * Devuelve una cadena nueva que es el resultado de copiar 
 * la cadena pasada como argumento en la nueva, pero sin el caractér en 
 * el índice dado.
 * En otras palabras, 'borra' el caractér en la posición dada.
 */
char* delete_char(char* s, int index, int length) {
  assert((index < length) && (index >= 0));
  char* newString = malloc(sizeof(char) * length - 1);

  int i;
  for (i = 0; i < index; i++)
    newString[i] = s[i];

  for (i = index; i < length - 1; i++)
    newString[i] = s[i + 1];

  return newString;
}

/* 
 * Reemplaza el caractér en el índice 'index' de la cadena pasada como
 * argumento por un número n que representa un caractér de la 'a' a la 'z'.
 */
char* replace_char(char* s, int index, int n, int length) {
  //Nos aseguramos que n sea una letra, y que el index 
  //este en el rango de la cadena.
  assert(((n >= 97) && (n <= 122)) || ((n >= 65) && (n <= 90)));
  assert((index >= 0) && (index < length));

  char* newString = malloc(sizeof(char) * length);
  newString = strcpy(newString, s);

  char c = n;

  newString[index] = c;

  return newString;
}

/* 
 * Devuelve un arreglo de 2 cadenas de caracteres, que son el resultado
 * de separar la cadena dada en el índice pasado como argumento.
 */
char** separate_word(char* s, int index, int length) {
  assert((index > 0) && (index < length));

  char** tuple = malloc(sizeof(char*) * 2);
  char* string1 = malloc(sizeof(char) * index);
  char* string2 = malloc(sizeof(char) * (length - index));

  int i;
  for (i = 0; i < index; i++)
    string1[i] = s[i];
  string1[i] = '\0';

  for (i = 0; i < (length - index); i++)
    string2[i] = s[index + i];
  string2[i] = '\0';

  tuple[0] = string1;
  tuple[1] = string2;

  return tuple;
}

/*
 * Dadas 2 cadenas, las concatena con un espacio.
 */
char *append_with_space(char *s1, char *s2) {
  int len1 = strlen(s1);
  int len2 = strlen(s2);
  int newLength = len1 + len2 + 1;
  char *newString = malloc(sizeof(char) * newLength);

  int i, j;
  for (i = 0; i < len1; i++) {
    newString[i] = s1[i];
  }
  newString[i] = ' ';
  i++;
  for (j = 0; j < len2; i++, j++) newString[i] = s2[j];
  newString[i] = '\0';

  return newString;
}

/* 
 * Descapitaliza una palabra.
 */
void uncapitalize(char *s) {
  assert(s);
  if ((s[0] <= 90) && (s[0] >= 65)) s[0] += 32;
}

/* 
 * Capitaliza una palabra.
 */
void capitalize(char *s) {
  assert(s);
  if ((s[0] >= 97) && (s[0] <= 122)) s[0] -= 32;
}

/* 
 * Crea una lista nula.
 */
SList slist_create() {
  return NULL;
}

/* 
 * Agrega un nuevo nodo a la lista con la cadena dada.
 */
SList slist_add(SList sl, char* s) {
  SList newList = malloc(sizeof(LSNode));
  newList->word = malloc(sizeof(char) * 100);
  strcpy(newList->word, s);
  newList->next = sl;

  return newList;
}

/* 
 * Dada una palabra y una tabla hash que contiene un diccionario,
 * si ésta no se encuentra en la tabla hash, crea una lista de palabras de 
 * sugerencia si, modificándola en cierta manera(ver función) la/s palabra/s 
 * resultante se encuentra/n en la tabla hash.
 */
SList suggestion_list_create(HashTable *ht, char* s) {
  //Si se encuentra la palabra, se presume que no hay errores ortográficos.
  if (ht_find(ht, s)) return NULL;//Por lo tanto retornamos una lista nula.
  capitalize(s);
  if (ht_find(ht, s)) return NULL;
  uncapitalize(s);
  if (ht_find(ht, s)) return NULL;

  int length = strlen(s);
  int i, j;

  SList sugList = slist_create();
  char* result;//Acá vamos a guardar la cadena modificada.
  char** resultTuple;//Acá vamos a guardar las cadenas separadas.

  for (i = 0; i < length - 1; i++) {
    //Intercambiamos los caracteres del índice i con el i + 1.
    result = swap_chars(s, i, length);
    //Si se encuentra en la tabla hash, lo agregamos a la lista de sugerencias.
    if (ht_find(ht, result)) sugList = slist_add(sugList, result);
    free(result);
  }

  for (i = 0; i < length + 1; i++) {
    for (j = 97; j < 123; j++) {
      //Inserta un caractér (j) entre medio de cada 2 letras de la palabra o
      //al principio o al final.
      result = insert_char(s, i, j, length);
      //Si se encuentra en la tabla hash, 
      //lo agregamos a la lista de sugerencias.
      if (ht_find(ht, result)) sugList = slist_add(sugList, result);
      free(result);
      //Lo mismo de antes pero ahora las j van a ser mayúsculas.
      result = insert_char(s, i, j - 32, length);
      if (ht_find(ht, result)) sugList = slist_add(sugList, result);
      free(result);
    }
  }

  for (i = 0; i < length; i++) {
    //Borra un caracter en el índice i, y modifica la cadena 
    //para que no haya un espacio vacío.
    result = delete_char(s, i, length);
    //Si se encuentra en la tabla hash, lo agregamos a la lista de sugerencias.
    if (ht_find(ht, result)) sugList = slist_add(sugList, result);
    free(result);
  }

  for (i = 0; i < length; i++) {
    for (j = 97; j < 123; j++) {
      //Reemplaza una letra en el índice i, por un caractér j.
      result = replace_char(s, i, j, length);
      //Si se encuentra en la tabla hash, 
      //lo agregamos a la lista de sugerencias.
      if (ht_find(ht, result)) sugList = slist_add(sugList, result);
      free(result);
      //Ahora las j representan las mayúsculas.
      result = replace_char(s, i, j - 32, length);
      //Si se encuentra en la tabla hash,
      //lo agregamos a la lista de sugerencias.
      if (ht_find(ht, result)) sugList = slist_add(sugList, result);
      free(result);
    }
  }

  for (i = 1; i < length; i++) {
    //Divide una palabra en el índice i, y devuelve un arreglo de 2 
    //cadenas de caracteres.
    resultTuple = separate_word(s, i, length);
    //Si ambas se encuentran en la tabla hash, agrega las 2 palabras
    //separadas por un espacio.
    if ((ht_find(ht, resultTuple[0])) && (ht_find(ht, resultTuple[1]))) {
      char *word = append_with_space(resultTuple[0], resultTuple[1]);
      sugList = slist_add(sugList, word);
      free(word);
    }
    free(resultTuple[0]);
    free(resultTuple[1]);
    free(resultTuple);
  }

  return sugList;
}

/* 
 * Destruye una lista.
 */
void slist_destroy(SList sl) {
  SList temp;
  while (sl) {
    temp = sl->next;
    free(sl->word);
    free(sl);
    sl = temp;
  }
}

/* 
 * Escribe las palabras de una lista en un archivo dado.
 */
void slist_fprint(FILE *file, SList sl) {
  while (sl != NULL) {
    fprintf(file, "%s\n", sl->word);
    sl = sl->next;
  }
}