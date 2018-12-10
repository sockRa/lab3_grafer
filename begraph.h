
/****************************************************************************/
/* DSA list program example D.F. ROSS                                       */
/****************************************************************************/
#ifndef BEGRAPH_H
#define BEGRAPH_H

/****************************************************************************/
/* function prototypes - operations on the Graph (a virtual machine)        */
/****************************************************************************/
/* Graph = (V, E) where V is a set of vertices (nodes) and E a set of edges */
/* There are a limited number of operations (9) which can be applied to G   */
/****************************************************************************/
void be_display_adjlist();               /* display G as an adjacency list  */
void be_display_adjmatrix();             /* display G as an adjacency matrix*/

void be_addnode(char c);                 /* add a vertex (node) to G        */
void be_remnode(char c);                 /* remove a vertex (node) from G   */

void be_addedge(char cs, char cd, int v);/* add an edge (with weight) to G  */
void be_remedge(char cs, char cd);       /* remove an edge from G           */

int  be_is_nmember(char c);              /* is a  node a member of G?       */
int  be_is_emember(char cs, char cd);    /* is an edge a member of G?       */

int  be_nsize();                         /* the number of nodes in G        */
int  be_esize();                         /* the number of edges in G        */
/****************************************************************************/
/* algorithms which may be applied to G                                     */
/****************************************************************************/
void be_Dijkstra(char node);
void be_Floyd();
void be_Warshall();
void be_Prim(char node);

#endif
/****************************************************************************/
/* end of definitions                                                       */
/****************************************************************************/

