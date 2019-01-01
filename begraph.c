
/****************************************************************************/
/* DSA node program example  D.F. ROSS                                      */
/****************************************************************************/

/****************************************************************************/
/* include files and  global data objects                                   */
/****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>

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
static char closest[MAXNOD];                    /* Prim closest             */

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

   return   is_empty(G)                ?     G
   :        c == get_nname(nhead(G))   ?  ntail(G)
   :        ncons(nhead(G),b_remn(c,ntail(G)));
}

/****************************************************************************/
/* REMove an edge from the graph                                            */
/****************************************************************************/
static noderef b_reme(char c, noderef E) {

   return   is_empty(E)                ?     E
   :        c == get_nname(ehead(E))   ?   etail(E)
   :        econs(ehead(E),b_reme(c,etail(E)));
}

/****************************************************************************/
/* REMove all edges for a given node from the graph                         */
/****************************************************************************/
static void b_remalle(char c, noderef G) {

   if(!is_empty(G)){
      b_reme(c,ehead(G));
      b_remalle(c,ntail(G));
   }
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
static int b_nsize(noderef G){return is_empty(G) ? 0 : 1 + b_nsize(ntail(G));}

/****************************************************************************/
/* FIND the number of edges in the graph (cardinality edges)                */
/****************************************************************************/
static int b_nedges(noderef E){

   noderef node = E;
   noderef edge = NULLREF;
   int count = 0;
   while(!is_empty(node)){
      edge = etail(node);
         while(!is_empty(edge)){
            count++;
            edge = etail(edge);
         }
         node = ntail(node);
   }
   return count;
}

static int b_esize(noderef G){
  
   return is_empty(G) ? 0 : b_nedges(G);
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

   int count = 0;
   noderef temp = G;

   while(get_nname(fnode) != get_nname(temp)){
      temp = ntail(temp);
      count++;
   }
   return count;

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
int minDistance(int D[],bool sptSet[],int size){
   
   int min = INFIN,index;

   for(int v = 0; v < size; v++){
      if(sptSet[v] == false && D[v] <= min){
         min = D[v];
         index = v;
      }
   }

   return index;   
}

static void b_dispSPT(){

   int size = b_nsize(G),i;
   
   noderef temp = G;

   printf("\n");
   printf("Dijkstra + SPT\n");
   printf("N:");

   for(i = 0; i < size; i++){
      printf("%7c",get_nname(temp));
      temp = ntail(temp);
   }
   

   printf("\n\nD:");

   for(i = 0; i < size; i++){
      if(D[i] == INFIN || D[i] == 0)
         printf("%7s","*");
      else
         printf("%7d",D[i]);
   }

   printf("\nE:");
   for(i = 0; i < size; i++)
      printf("%7c",E[i]);
   
   printf("\nL:");
   for(i = 0; i < size; i++){
      if(L[i] == INFIN || L[i] == 0)
         printf("%7s","*");
      else
         printf("%7d",L[i]);
   }

}

static void b_Dijkstra(char a){

   cre_adjmat();

   int size = b_nsize(G),i,u,j;
   
   bool sptSet[size];

   //noderef u = NULLREF;
   noderef source = b_findn(a,G);
   noderef temp = G;

   for(int i = 0; i < size; i++){
      D[i]        = INFIN;
      if(i == get_pos(source)){
         E[i] = '*';
      }else
      E[i]        = a;
      L[i]        = INFIN;
      sptSet[i]   = false;
   }

   D[get_pos(source)] = 0;
   L[get_pos(source)] = 0;

   for(i = 0; i < size - 1; i++){

      //Min-vertex index
      u = minDistance(D,sptSet,size);
      temp = G;

      //Find corresponding node from index
      for(j = 0; j < size; j++){
         if(j == u)  
            break;
         else
            temp = ntail(temp);
      }
      
      //Mark picked vertex as processed
      sptSet[u] = true;

      for(int v = 0; v < size; v++){
         if(sptSet[v] == false && adjmat[u][v] && D[u] != INFIN && D[u] + adjmat[u][v] < D[v]){
            D[v] = D[u] + adjmat[u][v];
            if(v != get_pos(source))
            E[v] = get_nname(temp);
            L[v] = adjmat[u][v];
         } 
      }
   }

   b_dispSPT(); 
}

/****************************************************************************/
/* Floyd                                                                    */
/****************************************************************************/
static void b_Floyd()  {

   int v = b_nsize(G);

   cre_adjmat();

   for(int y = 0; y < v; y++){
      for(int x = 0; x < v ; x++){
         if(x == y)  Floyd[y][x] = 0;
         else        Floyd[y][x] =  adjmat[y][x];
      }
   }
 
   for(int k = 0; k < v; k++){
      for(int i = 0; i < v; i++){
         for(int j = 0; j < v; j++){
            if(Floyd[i][j] > Floyd[i][k] + Floyd[k][j])
               Floyd[i][j] = Floyd[i][k] + Floyd[k][j];
         }
      }
   }

b_mdisp(Floyd,"Floyd");   

}

/****************************************************************************/
/* Warshall                                                                 */
/****************************************************************************/
static void b_Warshall() {
  
   int size = b_nsize(G),i,j,k;

   cre_adjmat();

   for(i = 0; i < size; i++){
      for(j = 0; j < size; j++){
         Warshall[i][j] = adjmat[i][j];
      }
   }

   for(k = 0; k < size; k++){
      for(i = 0; i < size; i++){
         for(j = 0; j < size; j++){
            if(Warshall[i][k] + Warshall[k][j] < Warshall[i][j])
               Warshall[i][j] = Warshall[i][k] + Warshall[k][j];
         }
      }
   }

   for(i = 0; i < size; i++){
      for(j = 0; j < size; j++){
         if(Warshall[i][j] != INFIN) 
            Warshall[i][j] = 1;
         else
            Warshall[i][j] = 0;
      }
   }

b_mdisp(Warshall,"Warshall");

}

/****************************************************************************/
/* Prim                                                                     */
/****************************************************************************/

static void b_dispMST(){ 
   
   int size = b_nsize(G),i;
   
   noderef temp = G;

   printf("\n");
   printf("Prim + MST\n");
   printf("N: ");

   for(i = 0; i < size; i++){
      printf("%7c",get_nname(temp));
      temp = ntail(temp);
   }

   printf("\n\nlc:");
   for(i = 0; i < size; i++){
      if(lowcost[i] == INFIN || lowcost[i] == 0)
         printf("%7s","*");
      else
         printf("%7d",lowcost[i]);
   }

   printf("\ncl:");
   for(i = 0; i < size; i++){
      printf("%7c",closest[i]);
   }
}   
static void b_sumMST(){

   int count = 0;
   int size = b_nsize(G);

   for(int i = 0; i < size; i++){
      if(lowcost[i] != INFIN)
         count += lowcost[i];
   }

   printf("\n\nMST sum is: %d\n",count);

   
}


static void b_Prim(char a){

   cre_adjmat();

   int size = b_nsize(G),i,nodeIndex,u,j;
   bool visited[size];

   //Find source node
   noderef source = b_findn(a,G);
   noderef temp = NULLREF;
   //noderef u = NULLREF;

   //Init all values
   for(i = 0; i < size; i++){
      lowcost[i]  =  INFIN;
      closest[i]  =  a;
      visited[i]  =  false;
   }

   //Mark source node as visited and make it's distance zero.
   nodeIndex = get_pos(source);
   lowcost[nodeIndex]   =  0;
   closest[nodeIndex]   =  '*';
   //visited[nodeIndex]   =  true;

  for(i = 0; i < size - 1; i++){

     //Pick minimum key vertex
      u = minDistance(lowcost, visited,size);
      temp = G;

      //Find correct node from index u
      for(j = 0; j < size; j++){
         if(j == u)
            break;
         else
            temp = ntail(temp);
      }

      //Mark picked vertex as visited
      visited[u] = true;

      for(int v = 0; v < size; v++){
         if(adjmat[u][v] && visited[v] == false && adjmat[u][v] < lowcost[v]){
            lowcost[v] = adjmat[u][v];
            closest[v] = get_nname(temp);
         }

      }
  }

   b_dispMST();
   b_sumMST();
   
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



