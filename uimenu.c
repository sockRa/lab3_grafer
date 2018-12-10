
/*****************************************************************************/
/* DSA Example program - Donald F. Ross                                      */
/*****************************************************************************/

/*****************************************************************************/
/* include files; define types and  global data objects                      */
/*****************************************************************************/
#include <stdio.h>
#include <string.h>
#include "fegraph.h"

/*****************************************************************************/
/* global data objects                                                       */
/*****************************************************************************/
#define    cbuffsize 81                              /* input buffer sizze   */
int        echomode=0;                               /* echo mode on/off     */
int        udirmode=1;                               /* graph: dir / undir   */
char       cbuff[cbuffsize];                         /* input buffer         */

/****************************************************************************/
/* User dialogue                                                            */
/****************************************************************************/
char ui_getNode() {
   printf(" Enter node: ");
   scanf("%s", cbuff); if (echomode) printf(" %c", cbuff[0]);
   return cbuff[0];
   }
int  ui_getWeight() {
   int w;
   printf(" Enter weight: ");
   scanf("%d", &w); if (echomode) printf(" %d", w);
   return w;
   }

void ui_putGraphEmpty()    { printf("\n The graph is empty "); }

void ui_putGraphNoEdges()  { printf("\n The graph contains no edges "); }

void ui_putTitleList(int nnodes, int nedges)
{ printf("\n\n Adjacency list (%d nodes %d edges): \n\n", nnodes, nedges);}

void ui_putNodeError(char c)   {
  printf("\n *** ERROR Node %c already exists! ", c);
  }

void ui_putNoNodeError(char c)   {
  printf("\n *** ERROR Node %c NOT in graph! ", c);
  }

void ui_putEdgeError(char cs, char cd) {
  printf("\n *** ERROR Edge (%c, %c) already exists! ", cs, cd);
}

void ui_putNoEdgeError(char cs, char cd) {
  printf("\n *** ERROR Edge (%c, %c) NOT in graph! ", cs, cd);
}

void ui_putNodeFound(char c)    { printf("\n Node %c found in graph ", c); }
void ui_putNodeNotFound(char c) { printf("\n Node %c NOT found in graph ", c); }

void ui_putEdgeFound(char cs, char cd) {
   printf("\n Edge (%c, %c) found in graph ", cs, cd);
}
void ui_putEdgeNotFound(char cs, char cd) {
   printf("\n Edge (%c, %c) NOT found in graph ", cs, cd);
}

int ui_isModeU() { return udirmode; }

/****************************************************************************/
/* UI functions                                                             */
/****************************************************************************/
static void showmode() {

   switch (udirmode) {
      case 0: printf(" directed graph mode ");    break;
      case 1: printf(" undirected graph mode ");  break;
      }
}

static char ui_get_input() {
   printf("\n Enter choice ==> ");
   scanf("%s", cbuff); if (echomode) printf(" %c", cbuff[0]);
   return cbuff[0];
   }

/*****************************************************************************/
/*  dispmenu: display the user menu                                          */
/*****************************************************************************/
static void dispmenu() {

   printf("\n");
   printf("***************************************************************\n");
   printf("***                                                         ***\n");
   printf("***   h:   display this menu                                ***\n");
   printf("***   u:   set the mode to undirected graph                 ***\n");
   printf("***   i:   set the mode to directed graph                   ***\n");
   printf("***   k:   display the mode (directed / undirected)         ***\n");
   printf("***                                                         ***\n");
   printf("***   d:   display the graph                                ***\n");
   printf("***   m:   display the matrix                               ***\n");
   printf("***                                                         ***\n");
   printf("***   a:   add a  node to the graph                         ***\n");
   printf("***   b:   add an edge to the graph                         ***\n");
   printf("***   r:   remove a  node from the graph                    ***\n");
   printf("***   s:   remove an edge from the graph                    ***\n");
   printf("***                                                         ***\n");
   printf("***   n:   find a  node in the graph                        ***\n");
   printf("***   e:   find an edge in the graph                        ***\n");
   printf("***                                                         ***\n");
   printf("***   t:   Dijkstra + SPT                                   ***\n");
   printf("***   v:   Floyd                                            ***\n");
   printf("***   w:   Warshall                                         ***\n");
   printf("***   x:   Prim                                             ***\n");
   printf("***                                                         ***\n");
   printf("***   q:   quit the program                                 ***\n");
   printf("***                                                         ***\n");
   printf("***************************************************************\n");
   printf("\n");
   }

/****************************************************************************/
/*  runmenu: execute the user menu                                          */
/****************************************************************************/
void runmenu() {
  
   char   choice;                     /* user menu selection  */

   dispmenu();
   do {
      choice = ui_get_input();
      
      switch(choice) {

         case 'h': case 'H': dispmenu();                        break;

         case 'z': case 'Z': echomode=1;                        break;
         case 'u': case 'U': udirmode=1;                        break;
         case 'i': case 'I': udirmode=0;                        break;
         case 'k': case 'K': showmode();                        break;

         case 'd': case 'D': fe_display_adjlist();              break;
         case 'm': case 'M': fe_display_adjmatrix();            break;

         case 'a': case 'A': fe_addnode();                      break;
         case 'b': case 'B': fe_addedge();                      break;

         case 'r': case 'R': fe_remnode();                      break;
         case 's': case 'S': fe_remedge();                      break;

         case 'n': case 'N': fe_is_nmember();                   break;
         case 'e': case 'E': fe_is_emember();                   break;
         
         case 't': case 'T': fe_Dijkstra();                     break;
         case 'v': case 'V': fe_Floyd();                        break;
         case 'w': case 'W': fe_Warshall();                     break;
         case 'x': case 'X': fe_Prim();                         break;

         case 'q': case 'Q': printf("\n *** BYE BYE! ***");     break;

         default:  printf("\n INVALID MENU CHOICE ***\n");
                   dispmenu();                                  break;
         }
      } while (choice != 'Q' && choice != 'q');
      printf("\n");
   }
/****************************************************************************/
/* end of menu functions                                                    */
/****************************************************************************/


