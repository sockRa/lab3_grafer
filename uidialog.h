/****************************************************************************/
/* DSA graph program example D.F. ROSS                                      */
/****************************************************************************/
#ifndef UIDIALOG_H
#define UIDIALOG_H

/****************************************************************************/
/* function prototypes - User Dialog                                        */
/****************************************************************************/
char ui_getNode();                         /* input a node                  */
int  ui_getWeight();                       /* input a weight                */

void ui_putTitleList();                    /* print title heading           */
void ui_putTitleMatrix();                  /* print title heading           */

void ui_putNodeError(char c);              /* print node exists error       */
void ui_putEdgeError(char cs, char cd);    /* print edge exists error       */
void ui_putNoNodeError(char c);            /* print node not in graph error */
void ui_putNoEdgeError(char cs, char cd);  /* print edge not in graph error */

void ui_putGraphEmpty();                   /* Empty graph messsage          */
void ui_putGraphNoEdges();                 /* graph no edges messsage       */
void ui_putNodeFound(char c);              /* node found message            */
void ui_putNodeNotFound(char c);           /* node NOT found message        */
void ui_putEdgeFound(char cs, char cd);    /* edge found message            */
void ui_putEdgeNotFound(char cs, char cd); /* edge NOT found message        */

int  ui_isModeU();                         /* is mode undirected            */

#endif
/****************************************************************************/
/* end of definitions                                                       */
/****************************************************************************/