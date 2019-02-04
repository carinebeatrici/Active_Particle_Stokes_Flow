#include "particle.h"
#include "param.h"

void create_box_list(int *box_list, particle *boid, int total_box, int n)
{
   int i,box;
   for(i=0;i<n;i++)boid[i].next=-1;
   for(box=0;box<total_box;box++)box_list[box]=-1;
   for(i=n-1;i>=0;--i)
     {
	box = boid[i].box;
	boid[i].next=box_list[box];
	box_list[box]=i;
     }
	
}
