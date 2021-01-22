#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define N 4
#define NxN (N*N)
#define TRUE 1
#define FALSE 0

struct node {
	int tiles[N][N];
	int f, g, h;
	short zero_row, zero_column;	/* location (row and colum) of blank tile 0 */
	struct node *next;
	struct node *parent;			/* used to trace back the solution */
};

int goal_rows[NxN];
int goal_columns[NxN];
struct node *start,*goal;
struct node *open = NULL, *closed = NULL;
struct node *succ_nodes[4];

void print_a_node(struct node *pnode) {
	int i,j;
	for (i=0;i<N;i++) {
		for (j=0;j<N;j++) 
			printf("%2d ", pnode->tiles[i][j]);
		printf("\n");
	}
	printf("\n");
}

struct node *initialize(char **argv){
	int i,j,k,index, tile;
	struct node *pnode;
	pnode=(struct node *) malloc(sizeof(struct node));
	index = 1;
	for (j=0;j<N;j++)
		for (k=0;k<N;k++) {
			tile=atoi(argv[index++]);
			pnode->tiles[j][k]=tile;
			if(tile==0) {
				pnode->zero_row=j;
				pnode->zero_column=k;
			}
		}
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	pnode->parent=NULL;
	start=pnode;
	printf("initial state\n");
	print_a_node(start);

	pnode=(struct node *) malloc(sizeof(struct node));
	goal_rows[0]=3;
	goal_columns[0]=3;

	for(index=1; index<NxN; index++){
		j=(index-1)/N;
		k=(index-1)%N;
		goal_rows[index]=j;
		goal_columns[index]=k;
		pnode->tiles[j][k]=index;
	}
	pnode->tiles[N-1][N-1]=0;	      /* empty tile=0 */
	pnode->f=0;
	pnode->g=0;
	pnode->h=0;
	pnode->next=NULL;
	goal=pnode; 
	printf("goal state\n");
	print_a_node(goal);
	return start;
}

/* merge unrepeated nodes into open list after filtering */
void merge_to_open() {
	for(int i=0;i<N;i++)
	{
		if(succ_nodes[i] == NULL)
		{
			continue;
		}
		struct node *temp = (struct node *)malloc(sizeof(struct node));
		memcpy(temp->tiles, succ_nodes[i]->tiles, NxN*sizeof(int));
		temp->f = succ_nodes[i]->f;
		temp->g = succ_nodes[i]->g;
		temp->h = succ_nodes[i]->h;
		temp->zero_row = succ_nodes[i]->zero_row;
		temp->zero_column = succ_nodes[i]->zero_column;
		temp->parent = succ_nodes[i]->parent;

		if(open == NULL)
		{
			open = temp;
			continue;
		}

		struct node *temp1 = open;
	
		int alreadyIn = FALSE;

		while(temp1 != NULL && temp1->next != NULL)
		{
			if(temp->f < temp1->next->f)
			{
				temp->next = temp1->next;
				temp1->next = temp;
				alreadyIn = TRUE;
				break;
			}
		temp1 = temp1->next;
		}
		
		if(alreadyIn == FALSE)
		{
			temp1->next = temp;
		}
		
	}

}



/*swap two tiles in a node*/
void swap(int row1,int column1,int row2,int column2, struct node * pnode){
	int tile = pnode->tiles[row1][column1];
	pnode->tiles[row1][column1]=pnode->tiles[row2][column2];
	pnode->tiles[row2][column2]=tile;
}

/*update the f,g,h function values for a node */
void update_fgh(struct node *pnode){
	pnode->h=0;
	pnode->g +=1;
	//ideal spot is tile[num%4][num-(4*(num%4)-1)
	for (int i=0;i<N;i++)
	{
		for (int j=0;j<N;j++)
		{
			if (pnode->tiles[i][j] != (goal->tiles[i][j]))
			pnode->h+=1;
		}
	}
	pnode->f = pnode->h + pnode->g;
}

/* 0 goes down by a row */
void move_down(struct node * pnode)
{
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row + 1, pnode->zero_column, pnode);
	pnode->zero_row++;
}


/* 0 goes right by a column */
void move_right(struct node * pnode)
{
	swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column+1, pnode);
        pnode->zero_column++;
}

/* 0 goes up by a row */
void move_up(struct node * pnode)
{
		swap(pnode->zero_row, pnode->zero_column, pnode->zero_row-1, pnode->zero_column, pnode);
        	pnode->zero_row--;
}

/* 0 goes left by a column */
void move_left(struct node * pnode)
{
		swap(pnode->zero_row, pnode->zero_column, pnode->zero_row, pnode->zero_column-1, pnode);
        	pnode->zero_column--;
}

/* expand a node, get its children nodes, and organize the children nodes using
 * array succ_nodes.
 */
void expand(struct node *selected) {
	if (selected->zero_column<N-1)
	{	
		succ_nodes[0] = (struct node *)malloc(sizeof(struct node));
		memcpy(succ_nodes[0]->tiles, selected->tiles, NxN*sizeof(int));
		succ_nodes[0]->zero_row = selected->zero_row;
		succ_nodes[0]->zero_column = selected->zero_column;
		succ_nodes[0]->parent = selected;
		move_right(succ_nodes[0]);
		update_fgh(succ_nodes[0]);
	}
	if (selected->zero_row>0)
	{	
		succ_nodes[1] = (struct node *)malloc(sizeof(struct node));
		memcpy(succ_nodes[1]->tiles, selected->tiles, NxN*sizeof(int));
		succ_nodes[1]->zero_row = selected->zero_row;
		succ_nodes[1]->zero_column = selected->zero_column;
		succ_nodes[1]->parent = selected;
		move_up(succ_nodes[1]);
		update_fgh(succ_nodes[1]);
	}
	if (selected->zero_column>0)
	{	
		succ_nodes[2] = (struct node *)malloc(sizeof(struct node));
		memcpy(succ_nodes[2]->tiles, selected->tiles, NxN*sizeof(int));
		succ_nodes[2]->zero_row = selected->zero_row;
		succ_nodes[2]->zero_column = selected->zero_column;
		succ_nodes[2]->parent = selected;
		move_left(succ_nodes[2]);
		update_fgh(succ_nodes[2]);
	}
	if (selected->zero_row<N-1)
	{	
		succ_nodes[3] = (struct node *)malloc(sizeof(struct node));
		memcpy(succ_nodes[3]->tiles, selected->tiles, NxN*sizeof(int));
		succ_nodes[3]->zero_row = selected->zero_row;
		succ_nodes[3]->zero_column = selected->zero_column;
		succ_nodes[3]->parent = selected;
		move_down(succ_nodes[3]);
		update_fgh(succ_nodes[3]);
	}
}


int nodes_same(struct node *a,struct node *b) {
	int flg=FALSE;
	if (memcmp(a->tiles, b->tiles, sizeof(int)*NxN) == 0)
		flg=TRUE;
	return flg;}


/* Filtering. Some nodes in succ_nodes may already be included in either open 
 * or closed list. Remove them. It is important to reduce execution time.
 * This function checks the (i)th node in succ_nodes array. You must call this
 & function in a loop to check all the nodes in succ_nodes.
 */ 
void filter(int i, struct node *pnode_list){
	struct node* head = pnode_list;
	while (head && succ_nodes[i])
	{
		if (nodes_same(head, succ_nodes[i]))
		{
			succ_nodes[i] = NULL;			
		}
	head = head->next;
	} 
}


int main(int argc,char **argv) {
	int iter,cnt;
	struct node *copen, *cp, *solution_path;
	int ret, i, pathlen=0, index[N-1];
	
	solution_path=NULL;
	start=initialize(argv);	/* init initial and goal states */
	open=start;
	iter=0;
	while (open!=NULL) {	/* Termination cond with a solution is tested in expand. */
		copen=open;
		open=open->next;  /* get the first node from open to expand */
		if(nodes_same(copen,goal)){ /* goal is found */
			do{ /* trace back and add the nodes on the path to a list */
				copen->next=solution_path;
				solution_path=copen;
				copen=copen->parent;
				pathlen++;
			} while(copen!=NULL);
			printf("Path (length=%d):\n", pathlen); 
			copen=solution_path;
			 /* print out the nodes on the list */
			while(solution_path != NULL)
			{
				print_a_node(solution_path);
				solution_path = solution_path->next;
			}
			break;
		}
		expand(copen);       /* Find new successors */
		for(i=0;i<4;i++){
			filter(i,open);
			filter(i,closed);
		}
		merge_to_open(); /* New open list */
		copen->next=closed;
		closed=copen;		/* New closed */
		/* print out something so that you know your 
		 * program is still making progress 
		 */
		iter++;
		if(iter %20000 == 0)
		{
			printf("No solution.\n");
			return 0;
		}	
	}
	return 0;
} /* end of main */

