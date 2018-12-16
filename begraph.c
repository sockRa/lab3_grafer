
/****************************************************************************/
/* DSA node program example  D.F. ROSS                                      */
/****************************************************************************/

/****************************************************************************/
/* include files and  global data objects                                   */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>

/****************************************************************************/
/* define types & constants                                                 */
/****************************************************************************/
#define MAXNOD 20
#define INFIN  9999
#define NULLREF NULL

/****************************************************************************/
/* node element definition (this is hidden!)                                */
/****************************************************************************/
typedef struct nodeelem * noderef;

typedef struct nodeelem {
        char       nname;
        int        ninfo;
        noderef    edges;
        noderef    nodes;
        } nodeelem;

/****************************************************************************/
/* define graph instance                                                    */
/****************************************************************************/
static noderef G     = (noderef) NULLREF;       /* adjacency list           */
static int     adjmat[MAXNOD][MAXNOD];          /* adjacency matrix         */

/****************************************************************************/
/* define display structures: Dijkstra-SPT, Floyd, Warshall, Prim           */
/****************************************************************************/
static int  D[MAXNOD];                          /* Dijkstra-SPT distance    */
static char E[MAXNOD];                          /* Dijkstra-SPT edge        */
static int  L[MAXNOD];                          /* Dijkstra-SPT weight      */

static int  Floyd[MAXNOD][MAXNOD];              /* Floyd matrix             */
static int  Warshall[MAXNOD][MAXNOD];           /* Warshall matrix          */

static int  lowcost[MAXNOD];                    /* Prim lowcost             */
static int  closest[MAXNOD];                    /* Prim closest             */

/****************************************************************************/
/* private operations on the node - basic operationa                        */
/****************************************************************************/
static int      is_empty(noderef N)             { return N == NULLREF; }

static char     get_nname(noderef N)            { return N->nname; }
static int      get_ninfo(noderef N)            { return N->ninfo; }
static noderef  get_edges(noderef N)            { return N->edges; }
static noderef  get_nodes(noderef N)            { return N->nodes; }

static noderef  set_nname(noderef N, char c)    { N->nname = c; return N; }
static noderef  set_ninfo(noderef N, int v)     { N->ninfo = v; return N; }
static noderef  set_edges(noderef N, noderef E) { N->edges = E; return N; }
static noderef  set_nodes(noderef N, noderef M) { N->nodes = M; return N; }

/****************************************************************************/
/* create and initialise an element in the node                             */
/****************************************************************************/
static noderef create_n(char c, int w)
{
   return set_nodes(
             set_edges(
                set_ninfo(
                   set_nname(malloc(sizeof(nodeelem)), c),
                w),
             NULLREF),
          NULLREF);
}

/****************************************************************************/
/* private operations on the graph - basic operations                       */
/****************************************************************************/
/****************************************************************************/
/* head and tail - a RECURSIVE view of the sequence                         */
/****************************************************************************/
static noderef nhead(noderef N)     { return N; }
static noderef ntail(noderef N)     { return get_nodes(N); }

static noderef ehead(noderef E)     { return E; }
static noderef etail(noderef E)     { return get_edges(E); }

/****************************************************************************/
/* CONStruct a new node with the element at the head of the node            */
/****************************************************************************/
static noderef ncons(noderef e, noderef N) { return set_nodes(e, N); }
static noderef econs(noderef e, noderef E) { return set_edges(e, E); }

/****************************************************************************/
/* display the edges                                                        */
/****************************************************************************/
static void b_edisp(noderef E) {
  if (!is_empty(E)) { 
     printf(" %c(%2d)", get_nname(ehead(E)), get_ninfo(ehead(E)));
     b_edisp(etail(E));
     }
}

/****************************************************************************/
/* display the nodes                                                        */
/****************************************************************************/
static void b_ndisp(noderef G) {
  if (is_empty(G)) printf("\n");
  else { printf(" %c : ", get_nname(nhead(G)));
         b_edisp(get_edges(G)); printf("\n");
         b_ndisp(ntail(G));
         }
}

/****************************************************************************/
/* ADD to the node in ascending order                                       */
/****************************************************************************/
static noderef b_addn(char c, noderef G)
{
   return is_empty(G)   ?  create_n(c,0)
   :  c < get_nname(G)  ?  ncons(create_n(c,0),G)
   :  ncons(G,b_addn(c,ntail(G)));
}

/****************************************************************************/
/* ADD to the edge in ascending order                                       */
/****************************************************************************/
static noderef b_adde(char c, int w, noderef E)
{
   //No auto-sort
   return is_empty(E)   ?  create_n(c,w)
   : c < get_nname(E)   ?  econs(E,econs(create_n(c,w), etail(E)))
   : econs(E,b_adde(c,w,etail(E)));  
   
}

/****************************************************************************/
/* REMove a  node from the graph                                            */
/****************************************************************************/
static noderef b_remn(char c, noderef G) {

   noderef temp = G;
   noderef parent = NULLREF;

   if(c == get_nname(temp)){
      G = ntail(temp);
      free(temp);
      return G;
   }
   
   while(c != get_nname(temp)){
      parent = temp;
      temp = ntail(temp);
   }

   ncons(parent,ntail(temp));
   free(temp);
   return G;

}

/****************************************************************************/
/* REMove an edge from the graph                                            */
/****************************************************************************/
static noderef b_reme(char c, noderef E) {

   noderef temp = E;
   noderef parent = NULLREF;

   if(c == get_nname(temp)){
      E = etail(temp);
      free(temp);
      return E;
   }

   while(c != get_nname(temp)){
      parent = temp;
      temp = etail(temp);
   }
   econs(parent,etail(temp));
   free(temp);
   return E;
}

/****************************************************************************/
/* REMove all edges for a given node from the graph                         */
/****************************************************************************/
static void b_remalle(char c, noderef G) {

   noderef temp = G;

   while(!is_empty(get_edges(temp))){
      if(c == get_nname(temp))   b_reme(c,temp);
      else  temp = ntail(temp);
   }

//   if(c == get_nname(temp)) b_reme(c,temp);
}

/****************************************************************************/
/* FIND a  node in the graph                                                */
/****************************************************************************/
static noderef b_findn(char c, noderef G) {
   return is_empty(G)   ?  NULLREF
   :  c > get_nname(G)  ?  b_findn(c,ntail(G))
   :  G;   
}

/****************************************************************************/
/* FIND an edge in the graph                                                */
/****************************************************************************/
static noderef b_finde(char c, noderef E) {
   return is_empty(E)   ?  NULLREF
   :  c != get_nname(E) ?  b_finde(c,etail(E))
   :  E;
}

/****************************************************************************/
/* FIND the number of nodes in the graph (cardinality nodes)                */
/****************************************************************************/
static int b_nsize(noderef G) {return is_empty(G) ? 0 : 1 + b_nsize(ntail(G));}

/****************************************************************************/
/* FIND the number of edges in the graph (cardinality edges)                */
/****************************************************************************/
static int b_nedges(noderef E) {
  printf("\n TO BE DONE "); return 0;
  }

static int b_esize(noderef G) {
  printf("\n TO BE DONE "); return 0;
  }

/****************************************************************************/
/* CREATE the adjacency matrix (AM)                                         */
/****************************************************************************/
/****************************************************************************/
/* Find the position (index) of a node in the adjacency list (node list)    */
/* NB: this is used to determine the index for the edge                     */
/*     this index is in turn used to fill in the weight in the AM           */
/* e.g. for an adjacency list:                                              */
/*               (position 0)   a => b(3) -> c(2) eol                       */
/*               (position 1)   b => a(3) -> c(7) eol                       */
/*               (position 2)   c => a(2) -> b(7) eol                       */
/*                              eol                                         */
/* get_pos("b") will give 1 (and hence AM[0][1] is set to 3 i.e. a-3-b)     */
/****************************************************************************/
static int get_pos(noderef fnode)  {

   printf("\n TO BE DONE "); return 0;
}

/****************************************************************************/
/* Fill in the values in the adjancy matrix from the adjacency list         */
/* this will give an adjacency matrix:     a       b       c                */
/*                                    -------------------------             */
/*                                    a |  0       3       2                */
/*                                    b |  3       0       7                */
/*                                    c |  2       7       0                */
/****************************************************************************/
static void cre_adjmat() {

   int i,j, nnodes;
   noderef pnode, pedge;

   nnodes = b_nsize(G);
   if (nnodes > MAXNOD) printf("\n *** System failure - too many nodes! ");
   else {
      for (i=0; i<nnodes; i++) for (j=0; j<nnodes; j++) adjmat[i][j] = INFIN;
      pnode = G;
      for (i=0; i<nnodes; i++) {
         pedge = get_edges(pnode);
         while (!is_empty(pedge)) {
            j = get_pos(pedge);
            adjmat[i][j] = get_ninfo(pedge);
            pedge = etail(pedge);
            }
         pnode = ntail(pnode);
         }
     }
}

/****************************************************************************/
/* DISPLAY the adjacency matrix                                             */
/****************************************************************************/

static void b_pline() {
   int i;
   printf("\n ----"); for (i=0; i<b_nsize(G); i++) printf("----");
   }

static void b_mhead(char * header) {

   noderef pnode;

   b_pline(); printf("\n %s", header); b_pline();
   printf("\n     ");
   pnode = G;
   while (!is_empty(pnode)) {
      printf("  %c ", get_nname(pnode));
      pnode = ntail(pnode);
      }
   b_pline();
   }

static void b_mdisp(int m[MAXNOD][MAXNOD], char * header) {

   int i,j, nnodes;
   noderef pnode;

   b_mhead(header);
   nnodes = b_nsize(G); pnode = G;
   for (i=0; i<nnodes; i++) {
      printf("\n "); printf(" %c: ", get_nname(pnode));
      for (j=0; j<nnodes; j++) 
         if (m[i][j] == INFIN) printf("  * ");
         else printf(" %2d ", m[i][j]);
      pnode = ntail(pnode);
      }
   b_pline();
   printf("\n ");
  }

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
/****************************************************************************/
/* Dijkstra-SPT                                                             */
/****************************************************************************/

static void b_dispSPT()  {

   printf("\n TO BE DONE "); 
  }

static void b_Dijkstra(char a) {
  
   printf("\n TO BE DONE ");
   }

/****************************************************************************/
/* Floyd                                                                    */
/****************************************************************************/
static void b_Floyd()  {

   printf("\n TO BE DONE ");
   }

/****************************************************************************/
/* Warshall                                                                 */
/****************************************************************************/
static void b_Warshall() {
  
   printf("\n TO BE DONE ");
   }

/****************************************************************************/
/* Prim                                                                     */
/****************************************************************************/

static void b_dispMST()  { 
   
   printf("\n TO BE DONE ");
   }
   
static void b_sumMST() {

   printf("\n TO BE DONE ");
   }

static void b_Prim(char a)     {

   printf("\n TO BE DONE ");
   }                        


/****************************************************************************/
/* public operations on the node                                            */
/****************************************************************************/
void be_display_adjlist()      { b_ndisp(G); }
void be_display_adjmatrix()
   { cre_adjmat(); b_mdisp(adjmat, "Adjacency Matrix"); }

void be_addnode(char c)  { G = b_addn(c, G); }
void be_remnode(char c)  { G = b_remn(c, G); b_remalle(c, G); }

void be_addedge(char cs, char cd, int v) {
   set_edges(b_findn(cs, G), b_adde(cd, v, get_edges(b_findn(cs, G))));
   }

void be_remedge(char cs, char cd) {
   set_edges(b_findn(cs, G), b_reme(cd, get_edges(b_findn(cs, G))));
   }

int be_is_nmember(char c) { return !is_empty(b_findn(c, G)); }

int be_is_emember(char cs, char cd) {
   return be_is_nmember(cs) &&
          !is_empty(b_finde(cd, get_edges(b_findn(cs, G))));
   }

int be_nsize() { return b_nsize(G); }
int be_esize() { return b_esize(G); }

/****************************************************************************/
/* GRAPH ALGORITHMS                                                         */
/****************************************************************************/
void be_Dijkstra(char node)  { b_Dijkstra(node);  }
void be_Floyd()              { b_Floyd();         }
void be_Warshall()           { b_Warshall();      }
void be_Prim(char node)      { b_Prim(node);      }
/****************************************************************************/
/* end of basic functions                                                   */
/****************************************************************************/



