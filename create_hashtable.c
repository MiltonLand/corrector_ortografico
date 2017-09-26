#include <stdio.h>
#include <malloc.h>
#include <string.h>
#include <assert.h>
#include "create_hashtable.h"

/* 
 * Crea un arreglo de cadenas de caracteres a la cual llamaremos diccionario.
 */
char **create_dictionary(char *fileName) {
  char c;
  FILE* file = fopen(fileName, "r");

  //Pedimos memoria para el diccionario.
  char **wordList = malloc(sizeof(char*) * MAX_WORDS);
  int i;
  for (i = 0; i < MAX_WORDS; i++) {
    wordList[i] = malloc(sizeof(char) * MAX_LENGTH);
  }
  
  i = 0;
  int j = 0;

  //Lee el archivo y crea el arreglo con las palabras.
  if (file) {
    while ((c = getc(file)) != EOF) {
      if (c == '\n') {
        wordList[i][j] = '\0';
        i++;
        j = 0;
      }
      else {
        wordList[i][j] = c;
        j++;
      }
    }
    fclose(file);
  }
  else {
    printf("Archivo no encontrado.\n");
    return NULL;
  }

  return wordList;
}

/* 
 * Libera el diccionario.
 */
void free_dictionary(char **dict) {
  int i;
  for (i = 0; i < MAX_WORDS; i++) free(dict[i]);
  free(dict);
}

/* 
 * Determina si el número dado es primo.
 */
int prime_number(int n) {
  int i, divCount = 0;

  for (i = 2; i <= n; i++) {
    if (n % i == 0) divCount++;
  }
  return (divCount == 1);
}

/* 
 * Devuelve el 't' siguiente número primero a 'n'.
 * Si 'n' es primo y 't' es 0, devuelve 'n'.
 * Si 'n' no es primo y 't' es 0, devuelve el 
 * siguiente número primo mayor a n. 
 * Si 'n' es primo y 't' es mayor a 0, devuelve el 
 * 't'-ésimo siguiente número primo.
 * EJEMPLOS: find_prime(5, 0) == 5; find_prime(10, 0) == 11;
 * find_prime(10, 1) == 13;
 */
int find_prime(int n, int t) {
  assert(t >= 0);
  int i;
  int count = -1;

  for (i = n; ; i++) {
    if (prime_number(i) == 1) count++;
    if (count == t) return i;
  }

  return 0;
}

/* 
 * Crea una tabla hash auxiliar.
 */
AuxHashTable *aht_create(UniversalHashFunction hf, unsigned tSize) {
  AuxHashTable *newTable = malloc(sizeof(AuxHashTable));
  newTable->table = malloc(sizeof(AuxHTNode*) * tSize);
  newTable->hf = hf;
  newTable->tSize = tSize;

  for (int i = 0; i < tSize; i++) newTable->table[i] = NULL;

  return newTable;
}

/* 
 * Inserta un elemento a una tabla hash auxiliar.
 * Esta función sólo va a ser usada para insertar elementos de un
 * diccionario, por lo tanto, no checkea si el elemento a insertar
 * ya se encuentra en la tabla hash. Es decir, si está repetido, 
 * lo ingresa dos veces. 
 */
AuxHashTable *aht_insert(AuxHashTable *aht, char *s) {
  unsigned index = aht->hf(s, aht->tSize, BIG_PRIME);

  AuxHTNode *newNode = malloc(sizeof(AuxHTNode));

  if (!aht->table[index]) {
    newNode->string = malloc(sizeof(char) * MAX_LENGTH);
    newNode->string = strcpy(newNode->string, s);
    newNode->next = NULL;
  }
  else {
    newNode->string = malloc(sizeof(char) * MAX_LENGTH);
    newNode->string = strcpy(newNode->string, s);
    newNode->next = aht->table[index];
  }

  aht->table[index] = newNode;

  return aht;
}

/* 
 * Crea la tabla hash de segundo nivel.
 */
SecondaryHashTable* ht2_create(SecondaryHashTable* ht2, int tSize, int prime) {
  ht2 = malloc(sizeof(SecondaryHashTable));
  ht2->table2 = malloc(sizeof(SecondarySlot) * tSize);
  ht2->tSize = tSize;
  ht2->prime = prime;

  int i;
  for (i = 0; i < tSize; i++) {
    ht2->table2[i].occupied = 0;
    ht2->table2[i].value = NULL;

  }

  return ht2;
}

/* 
 * Dada una tabla hash y un diccionario, devuelve un array con la cantidad de 
 * elementos que va a tener cada índice de la tabla hash. 
 * Ej: Si tableSizes[0] == 758, significa que debemos crear la tabla hash
 * con 758(al cuadrado, después se explica por qué) elementos en el índice 0.
 */
int *create_table_sizes(HashTable *ht, char **dict) {
  int *tableSizes = malloc(sizeof(int) * ht->tSize);
  int i;
  for (i = 0; i < ht->tSize; i++) tableSizes[i] = 0;
  unsigned index;
  for (i = 0; i < MAX_WORDS; i++) {
    index = ht->hash(dict[i], ht->tSize, BIG_PRIME);
    tableSizes[index]++;
  }

  return tableSizes;
}

/* 
 * Crea una tabla hash primaria. Es decir, es el primer nivel de la tabla hash.
 */
HashTable *ht1_create(UniversalHashFunction uHash, char **dict) {
  HashTable* primaryHashTable = malloc(sizeof(HashTable));
  primaryHashTable->tSize = HT1_SIZE;
  primaryHashTable->hash = uHash;
  primaryHashTable->table = malloc(sizeof(SecondaryHashTable*) * 
                      primaryHashTable->tSize);

  int i, tableSize;
  int *tableSizes = create_table_sizes(primaryHashTable, dict);
  printf("TABLE_SIZES = {");
  for (i = 0; i < HT1_SIZE; i++) printf("%d, ", tableSizes[i]);
  printf("}\n");

  for (i = 0; i < primaryHashTable->tSize; i++) {
    //Elegimos el tamaño al cuadrado para tener un 50% de probabilidades 
    //de colisionar con otro elemento de la tabla.
    tableSize = tableSizes[i] * tableSizes[i];
    //Creamos una nueva tabla hash para cada indice la primera tabla.
    primaryHashTable->table[i] = ht2_create(primaryHashTable->table[i], 
                   tableSize, find_prime(BIG_PRIME, 0));
  }

  free(tableSizes);

  return primaryHashTable;
}

/* 
 * Libera una tabla hash de segundo nivel.
 */
void free_secondary_table(SecondaryHashTable* ht2, int tSize) {
  free(ht2->table2);
  free(ht2);
}

/* 
 * Libera la tabla hash auxiliar.
 */
void free_auxiliary_HT(AuxHashTable *aht) {
  int i;
  AuxHTNode *node;
  AuxHTNode *temp;
  for (i = 0; i < HT1_SIZE; i++) {
    node = aht->table[i];
    while (node) {
      temp = node->next;
      free(node->string);
      free(node);
      node = temp;
    }
  }
  free(aht->table);
  free(aht);
}

/*
 * Dada una tabla hash de dos niveles y un diccionario, ingresa todas 
 * las palabras del diccionario a la tabla hash.
 */
HashTable *ht_insert_all(HashTable* ht, char **dictionary) {
  int i, index1, index2, tableSize;
  int trial = 0;
  int *tableSizes = create_table_sizes(ht, dictionary);
  AuxHashTable *aht = aht_create(ht->hash, HT1_SIZE);

  //Insertamos una a una todas las palabras a la tabla hash auxiliar.
  for (i = 0; i < MAX_WORDS; i++) {
    aht = aht_insert(aht, dictionary[i]);
  }

  //Y de la tabla auxiliar la pasamos a la tabla de dos niveles.
  AuxHTNode *currentNode;
  for (index1 = 0; index1 < HT1_SIZE; index1++) {
    currentNode = aht->table[index1];
    while (currentNode) {
      //Calculamos la función hash universal.
      index2 = ht->hash(currentNode->string, 
                        ht->table[index1]->tSize,
                        ht->table[index1]->prime);
      if (ht->table[index1]->table2[index2].occupied) {
          printf("COLLISION CON %s\n", 
            ht->table[index1]->table2[index2].value);
          printf("CREANDO NUEVA TABLA.\n");
          //Borramos todo lo que hicimos y empezamos de nuevo.
          free_secondary_table(ht->table[index1], ht->table[index1]->tSize);
          //Volvemos al principio de la lista.
          currentNode = aht->table[index1];
          //Pedimos memoria para la nueva tabla.
          ht->table[index1] = malloc(sizeof(SecondaryHashTable*));
          //Elegimos el tamaño de la tabla al cuadrado para reducir 
          //las collisiones frecuentes.
          tableSize = tableSizes[index1] * tableSizes[index1];
          //Creamos la nueva tabla.
          ht->table[index1] = ht2_create(ht->table[index1], 
                           tableSize, 
                           find_prime(BIG_PRIME, trial++));
          printf("NUEVA TABLA CREADA.\n\n\n\n");
      }
      else {
        //Agregamos los datos a la tabla.
        ht->table[index1]->table2[index2].occupied = 1;
        ht->table[index1]->table2[index2].value = 
          malloc(sizeof(char) * MAX_LENGTH);
        strcpy(ht->table[index1]->table2[index2].value, currentNode->string);
        //Seguimos iterando sobre la lista.
        currentNode = currentNode->next;
        printf("%s INGRESADA.\n", ht->table[index1]->table2[index2].value);
      }
    }
  }

  printf("TABLA CREADA.\n");
  free_auxiliary_HT(aht);
  free(tableSizes);

  return ht;
}

/* 
 * Busca una cadena en la tabla hash de dos niveles.
 */
int ht_find(HashTable* ht, char* str) {
  int index1, index2;
  //Calculamos la primera función hash.
  index1 = ht->hash(str, ht->tSize, BIG_PRIME);
  //Calculamos la función hash universal.
  index2 = ht->hash(str, 
           ht->table[index1]->tSize, 
           ht->table[index1]->prime);
  if ((ht->table[index1]->table2[index2].value) && 
      (strcmp(ht->table[index1]->table2[index2].value, str) == 0))
    return 1;
  return 0;
}

/* 
 * Libera la tabla hash de dos niveles.
 */
void free_ht(HashTable *ht) {
  int i;
  for (i = 0; i < ht->tSize; i++) {
    free_secondary_table(ht->table[i], ht->table[i]->tSize);
  }
  free(ht->table);
  free(ht);
}