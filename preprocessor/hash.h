// Lonnie Hansen
// hash.h
#ifndef hash_h
#define hash_h
#define _CRT_SECURE_NO_DEPRECATE
#include <stdio.h>

struct nlist {
    struct nlist *next;
    char *name;
    char *defn;
};

unsigned hash(char *s);
struct nlist *lookup(char *s);
char *mystrdup(char *s);
struct nlist *install(char *name, char *defn);
void mystrcpy(char* p1, char* p2);
void undef(char *name);
void defineroutine();
int filegetword(char* word, int lim, FILE *tfile);
int filegetline(char* word, int lim, FILE *tfile);
int filegetch(FILE *tfile);
void fileungetch(int c);
char* mystrcat(char * dest, char* src);
void printhashtable();
void clearline(char str[]);


#endif /* hash_h */
