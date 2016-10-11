#ifndef ILIST.H
#define ILIST.H

#define FALSE 0
#define TRUE 1
#define LEFT_VINCULUM 2
#define RIGHT_VINCULUM 3
#define LEFT_BRACKET 4
#define RIGHT_BRACKET 5
#define SEMICOLON 6
#define MUL 7
#define BREAK 8
#define CLASS 9
#define CONTINUE 10
#define BOOLEAN 11
#define DO 12
#define DOUBLE 13
#define ELSE 14
#define FOR 15
#define IF 16
#define INT 17
#define RETURN 18
#define STRING 19
#define STATIC 20 
#define VOID 21
#define WHILE 22
#define ID 23
#define STRING_LITERAL 24
#define INT_LITERAL 25
#define DOUBLE_LITERAL 26
#define INC 27
#define PLUS 28
#define DEC 29
#define MINUS 30
#define LESS_EQ 31
#define LESS 32
#define GREAT_EQ 33
#define GREAT 34
#define N_EQUAL 35
#define DIV 36

// summ = 666

typedef struct
{
  int instType;  // typ instrukce
  void *addr1; // adresa 1
  void *addr2; // adresa 2
  void *addr3; // adresa 3
} tInstr;

typedef struct listItem
{
  tInstr instruction;
  struct listItem *nextItem;
} tListItem;
    

typedef struct
{
  struct listItem *first;  // ukazatel na prvni prvek
  struct listItem *last;   // ukazatel na posledni prvek
  struct listItem *active; // ukazatel na aktivni prvek
} tListOfInstr;


void listInit(tListOfInstr *L); // funkce inicializuje seznam instrukci
void listFree(tListOfInstr *L); // funkce dealokuje seznam instrukci
void listInsertLast(tListOfInstr *L, tInstr I); // vlozi novou instruci na konec seznamu
void listFirst(tListOfInstr *L); // zaktivuje prvni instrukci
void listNext(tListOfInstr *L); // aktivni instrukci se stane nasledujici instrukce
tInstr *listGetData(tListOfInstr *L); // vrati aktivni instrukci
void listGoto(tListOfInstr *L, tListItem *gotoInstr); // nastavime aktivni instrukci podle zadaneho ukazatele
void *listGetPointerLast(tListOfInstr *L); // vrati ukazatel na posledni instrukci


#endif