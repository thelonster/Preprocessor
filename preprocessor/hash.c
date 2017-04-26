//hash.c
#include "hash.h"
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#define HASHSIZE 101
#define MAXWORD 100
#define BUFFERSIZE 100
#define MAXLINE 500

typedef struct nlist nlist;

char buffer[BUFFERSIZE];
int bufferp = 0;
nlist *hashtab[HASHSIZE];

unsigned hash(char *s) {
    unsigned hashval;
    for (hashval = 0; *s != '\0'; s++)
        hashval = *s + 31 * hashval;
    return hashval % HASHSIZE;
}

nlist *lookup(char *s) {
    nlist *np;
    for (np = hashtab[hash(s)]; np != NULL; np = np->next)
        if (!strcmp(s, np->name))
            return np;
    return NULL;
}

nlist *install(char *name, char *defn) {
    nlist *np;
    unsigned hashval;
    if ((np = lookup(name)) == NULL) {
        np = (nlist *) malloc(sizeof(*np));
        if (np == NULL || (np->name = mystrdup(name)) == NULL)
            return NULL;
        hashval = hash(name);
        np->next = hashtab[hashval];
        hashtab[hashval] = np;
    }
    else
        free((void *)np->defn);
    if ((np->defn = mystrdup(defn)) == NULL)
        return NULL;
    else
        return np;
}

void mystrcpy(char* p1, char* p2) {
    while ((*p1++ = *p2++))
        ;
}

char *mystrdup(char *s) {
    char *p;
    if ((p = (char *)malloc(strlen(s) + 1)) != NULL)
        mystrcpy(p, s);
    return p;
}

void undef(char *name) {
    nlist *pp, *np;
    
    for (pp = NULL, np = hashtab[hash(name)]; np != NULL; pp = np, np = np->next)
        if (!strcmp(name, np->name)) {
            if (pp == NULL)
                hashtab[hash(name)] = np->next;
            else
                pp->next = np->next;
            free(np->name);
            free(np->defn);
            free(np);
            return;
        }
}

void defineroutine() {
    
    FILE *file, *newfile;
    file = fopen("testfile.cpp","r");
    newfile = fopen("newfile.cpp","w");
    if (file == NULL) {
        perror("Error opening file\n");
        return;
    }
    int c;
    char word[MAXWORD];
    char name[MAXWORD];
    char def[MAXWORD];
    char line[MAXLINE] = { 0 };
    printf("===== ===== =====>>> preprocessing beginning <<<===== ===== =====\n");
    while ((c = fgetc(file)) != EOF) { 
        if (c == '/') {
            printf("%c", c);
            fputc(c, newfile);
            c = fgetc(file);
            if (c == '/') {
                printf("%c", c);
                fputc(c, newfile);
                printf(" ");
                fputc(' ', newfile);
                filegetline(line, MAXLINE, file);
                printf("%s", line);
                fputs(line, newfile);
                clearline(line);
                printf("\n");
                fputc('\n', newfile);
            }
            else if (c == '*') {
                printf("%c", c);
                fputc(c, newfile);
                while ((c = fgetc(file)) != 47 && c != EOF) {
                    printf("%c", c);
                    fputc(c, newfile);
                }
                printf("%c", c);
                fputc(c, newfile);
                printf("\n");
                fputc('\n', newfile);
            }
            else
                ungetc(c, file);
        }
        else if (c == '#') {
            ungetc(c, file);
            filegetword(word, MAXWORD, file);
            if (strcmp(word, "#define") == 0) {
                filegetword(name, MAXWORD, file);
                filegetline(def, MAXWORD, file);
                install(name, def);
                mystrcpy(word, "");
                mystrcpy(name, "");
                mystrcpy(def, "");
            }
            else if (strcmp(word, "#undef") == 0) {
                filegetword(name, MAXWORD, file);
                nlist* temp = lookup(name);
                if (temp != NULL) {
                    undef(name);
                }
                else
                    printf("Definition not found\n");
                mystrcpy(word, "");
                mystrcpy(name, "");
            }
        }
        else if (isalpha(c)) {
            ungetc(c, file);
            filegetword(word, MAXWORD, file);
            nlist * check = lookup(word);
            if (check != NULL) {
                printf("%s", check->defn);
                fputs(check->defn, newfile);
            }
            else {
                printf("%s", word);
                fputs(word, newfile);
            }                
        }
        else if (c == 10)
            ;// Do nothing, I control where the newlines are.
        else if (c == ';') {
            printf("%c\n", c);
            fputc(c, newfile);
            fputc('\n', newfile);
        }      
        else {
            printf("%c", c);
            fputc(c, newfile);
        }                        
    }
    printf("===== ===== =====>>> preprocessing ending <<<===== ===== =====\n");
    fclose(file);
    printf("End of file reached successfully\n");
    printhashtable();
}

int filegetword(char* word, int lim, FILE *tfile) {
    int c;
    char* w = word;
    while (isspace(c = fgetc(tfile))) 
        ;
    if (c != EOF)
        *w++ = c;
    if (!isalpha(c) && c != 35) {
        *w = '\0';
        return c;
    }
    for (; --lim > 0; w++)
        if (!isalnum(*w = fgetc(tfile))) {
            fileungetch(*w);
            ungetc(*w, tfile);
            break;
        }
    *w = '\0';
    return word[0];
}

int filegetline(char* word, int lim, FILE *tfile) {
    char* w = word;
    int c;
    while (isspace(c = fgetc(tfile)))
        ;
    ungetc(c, tfile);
    while ((c = fgetc(tfile)) != '\n' && c != EOF) {
        *w++ = c;
    }
    *w = '\0';
    return word[0];
}

int filegetch(FILE *tfile) {
    return (buffer > 0) ? buffer[--bufferp] : fgetc(tfile);
}

void fileungetch(int c) {
    if (bufferp >= BUFFERSIZE)
        printf("ungetch: too many characters\n");
    else
        buffer[bufferp++] = c;
}

char* mystrcat(char *dest, char *src) {
    size_t i, j;
    for (i = 0; dest[i] != '\0'; i++)
        ;
    for (j = 0; src[j] != '\0'; j++)
        dest[i + j] = src[j];
    dest[i + j] = '\0';
    return dest;
}

void printhashtable() {
    nlist * node;
    for (int a = 0; a < HASHSIZE; a++)
        if ((node = hashtab[a]) != NULL)
            printf("%s : \"%s\"\n",node->name,node->defn);
}


void clearline(char str[]) {
    for (int a = 0; a < MAXLINE; a++)
        str[a] = '\0';
}