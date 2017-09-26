#ifndef __HASH_H__
#define __HASH_H__

//Cantidad de palabras en el diccionario.
#define MAX_WORDS 87899
//Tamaño de la primera tabla.
#define HT1_SIZE 130
//Máxima cantidad de letras por palabra en el diccionario.
#define MAX_LENGTH 30
//Número primo grande, por default.
#define BIG_PRIME 1123

//Función hash.
typedef unsigned (*UniversalHashFunction) (char*, int tSize, int prime);

//Casilla de la tabla hash de segundo nivel.
typedef struct SecondarySlot {
  int occupied;
  char* value;
} SecondarySlot;

//Tabla hash de segundo nivel.
typedef struct SecondaryHashTable {
  SecondarySlot* table2;
  int tSize;
  int prime;
} SecondaryHashTable;

//Tabla hash de primer nivel.
typedef struct HashTable {
  SecondaryHashTable** table;
  int tSize;
  UniversalHashFunction hash;
} HashTable;

//Nodo de la tabla hash auxiliar.
typedef struct AuxHTNode {
  char *string;
  struct AuxHTNode *next;
} AuxHTNode;

//Tabla hash auxiliar.
typedef struct AuxHashTable {
  AuxHTNode **table;
  UniversalHashFunction hf;
  unsigned tSize;
} AuxHashTable;

//Funciones de la tabla hash auxiliar.
AuxHashTable *aht_create(UniversalHashFunction hf, unsigned tSize);

//Funciones de la tabla hash.
HashTable *ht1_create(UniversalHashFunction uHash, char **dict);
SecondaryHashTable *ht2_create(SecondaryHashTable *ht2, 
                               int tSize, int prime);
HashTable *ht_insert_all(HashTable *ht, char **dict);
SecondaryHashTable *ht2_insert(SecondaryHashTable ht2, char *str);
int ht_find(HashTable *ht, char *str);
void free_secondary_table(SecondaryHashTable *ht2, int tSize);
void free_ht(HashTable *ht);

//Funciones diccionario.
char **create_dictionary(char *fileName);
void free_dictionary(char **dict);

#endif /* __HASH_H__ */