
/****************************************************************************/
/* DSA graph program example D.F. ROSS                                      */
/****************************************************************************/
#ifndef FEGRAPH_H
#define FEGRAPH_H

/****************************************************************************/
/* function prototypes - operations on the Graph (a virtual machine)        */
/****************************************************************************/
/* Graph = (V, E) where V is a set of vertices (nodes) and E a set of edges */
/* There are a limited number of operations (9) which can be applied to G   */
/****************************************************************************/
void fe_display_adjlist();               /* display G as an adjacency list  */
void fe_display_adjmatrix();             /* display G as an adjacency matrix*/

void fe_addnode();                       /* add a vertex (node) to G        */
void fe_remnode();                       /* remove a vertex (node) from G   */

void fe_addedge();                       /* add an edge (with weight) to G  */
void fe_remedge();                       /* remove an edge from G           */

int  fe_is_nmember();                    /* is a  node a member of G?       */
int  fe_is_emember();                    /* is an edge a member of G?       */
/****************************************************************************/
/* algorithms which may be applied to G                                     */
/****************************************************************************/
void fe_Dijkstra();
void fe_Floyd();
void fe_Warshall();
void fe_Prim();

#endif
/****************************************************************************/
/* end of definitions                                                       */
/****************************************************************************/

