#include <stdio.h>

#include "param.h"
#include "particle.h"

void calc_box(particle *boid, const int box_for_line, int n)
{
   int x,y;
   int i;
   for(i=0;i<n;i++)
     {
	x = (int)(boid[i].x/R_MAX);
	y = (int)(boid[i].y/R_MAX);
	boid[i].box= (y*box_for_line)+x;
//	if( boid[i].box>box_for_line*box_for_line)printf("box: %d x: %f x:b %d  y: %f y: %d \n",boid[i].box,boid[i].x,x,boid[i].y,y);
     }
}

void neighbor_box_calc(neighbor_box *neighbor_box_list, const int box_for_line, const int box_for_row)
{
   int box;
   int total_box = box_for_line*box_for_row;
   for(box=0;box<total_box;box++)
     {
	//  6   5   4
	//  7   x   3
	//  0   1   2
	neighbor_box_list[box].v1=box-box_for_line;
	neighbor_box_list[box].v0=neighbor_box_list[box].v1-1;
	neighbor_box_list[box].v2=neighbor_box_list[box].v1+1;
	neighbor_box_list[box].v3=box+1;
	neighbor_box_list[box].v7=box-1;
	neighbor_box_list[box].v5=box+box_for_line;
	neighbor_box_list[box].v6=neighbor_box_list[box].v5-1;
	neighbor_box_list[box].v4=neighbor_box_list[box].v5+1;
	//first row
	if(box%box_for_line==0)
	  {
	     neighbor_box_list[box].v0+=box_for_line;
	     neighbor_box_list[box].v6+=box_for_line;
	     neighbor_box_list[box].v7+=box_for_line;
	  }
	//last row
	if((box+1)%box_for_line==0)
	  {
	     neighbor_box_list[box].v2-=box_for_line;
	     neighbor_box_list[box].v3-=box_for_line;
	     neighbor_box_list[box].v4-=box_for_line;
	  }
	//first line
	if(box<box_for_line)
	  {
	     neighbor_box_list[box].v0+=total_box;
	     neighbor_box_list[box].v1+=total_box;
	     neighbor_box_list[box].v2+=total_box;
	  }
	//last line
	if(box>=(total_box-box_for_line))
	  {
	     neighbor_box_list[box].v4-=total_box;
	     neighbor_box_list[box].v5-=total_box;
	     neighbor_box_list[box].v6-=total_box;
	  }
     }
}

