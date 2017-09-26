#include <stdio.h>
#include <string.h>
#include <malloc.h>
#include "create_hashtable.h"
#include "read_file_words.h"
#include "strings_modify.h"

//Función hash.
unsigned universal_hash_function(char* s, int tSize, int prime) {
  unsigned index = 0;
  int i, length = strlen(s);
  for (i = 0; i < length; i++) 
    index = prime * index + s[i];

  return (index % tSize);
}

int main(int argc, char **argv) {
  if (argc != 3) return 0;
  //Leemos el diccionario y lo guardamos en un array de cadenas.
  char **dict = create_dictionary("lemario-general-del-espanol.txt");

  //Creamos la tabla hash usando perfect hashing.
  HashTable* newTable = ht1_create(universal_hash_function, dict);
  newTable = ht_insert_all(newTable, dict);

  //Los datos estan en la tabla, asi que liberamos el diccionario.
  free_dictionary(dict);

  //Leemos el archivo de entrada y guardamos cada palabra del archivo en una
  //lista enlazada.
  WList wordsToCheck = read_file_words(argv[1]);

  //Creamos el archivo de salida.
  FILE *outputFile = fopen(argv[2], "w");

  SList suggestionList;//Acá se va a llevar una lista con las sugerencias.
  WList node = wordsToCheck;//Nodo iterativo
  while (node) {
    //Creamos una lista de sugerencias sobre la palabra actual.
    suggestionList = suggestion_list_create(newTable, node->word);
    //Si no hay sugerencias, la lista es nula, asi que comprobamos eso...
    if (suggestionList) {
      //Si no es nula escribimos las sugerencias en el archivo de salida.
      fprintf(outputFile, 
        "'%s' no encontrada. Linea %d, posicion %d.\nSugerencias:\n", 
        node->word, node->line, node->pos);
      slist_fprint(outputFile, suggestionList);
      fprintf(outputFile, "/------------------------------------------/\n");
    }

    //Liberamos la lista de sugerencias.
    slist_destroy(suggestionList);
    //Seguimos iterando.
    node = node->next;
  }

  //Liberamos la lista de palabras del archivo de entrada.
  wlist_destroy(wordsToCheck);
  //Liberamos la tabla hash.
  free_ht(newTable);
  //Cerramos el archivo.
  fclose(outputFile);
}