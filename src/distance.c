#include <stdio.h>
#include <math.h>
#include <stdlib.h>

#include "param.h"
#include "particle.h"

void create_box_list(int *box_list, particle *boid, int total_box, int n);


void calculate_distance(particle *boid , float *sum_vx, float *sum_vy,
			float *sum_fx, float *sum_fy,
			const int box_for_line,
			const int box_for_row, 
			neighbor_box *neighbor_box_list, int *box_list
		       , int n)
{
   int i,j,k,box;
   float dx,dy;
   float vx,vy;
   float fx,fy;
   int test_loop;
   float distance;
   float distance_center;
   const float rmax2 = R_MAX*R_MAX; 
   const float Lxh = Lx/2., Lyh = Ly/2.;
   int total_box;
   int neighbor[8];
//   const float alpha[2][2] = {{ALPHA11, ALPHA12},{ALPHA12, ALPHA22}};
//   const float beta[2][2] = {{BETA11, BETA12},{BETA12, BETA22}};
   particle  particle_i;
   particle  particle_j;
   total_box = box_for_line * box_for_row;
   create_box_list(box_list, boid, total_box, n);
   for(i=0;i<n;i++)
     {
	particle_i = boid[i];
	vx=0.0;
	vy=0.0;
	fx=0.0;
	fy=0.0;
	box = particle_i.box;
	neighbor[0]=neighbor_box_list[box].v0;
	neighbor[1]=neighbor_box_list[box].v1;
	neighbor[2]=neighbor_box_list[box].v2;
	neighbor[3]=neighbor_box_list[box].v3;
	neighbor[4]=neighbor_box_list[box].v4;
	neighbor[5]=neighbor_box_list[box].v5;
	neighbor[6]=neighbor_box_list[box].v6;
	neighbor[7]=neighbor_box_list[box].v7;
	j = box_list[box];
	while (j>=0&&j<n)
	  {
	     particle_j = boid[j];
	     distance=0.0;
	     dx = particle_i.x - particle_j.x;
	     dy = particle_i.y - particle_j.y;
	     distance=(dx*dx)+(dy*dy);
	     if (distance < rmax2 && distance > 0.0)
	       {
		  if(distance < R_CORE*R_CORE)
		    {
		       distance = sqrt(distance);
		       fx += F_0 * dx / distance;
		       fy += F_0 * dy / distance;
		    }
		  else
		    {
		       distance = sqrt(distance);  //b22*dx*(1/r-f0)
		       //fx += beta[particle_i.type][particle_j.type] * (1.0/distance-1.0/R_EQ)*dx;
		       fx += BETA * (1.0/distance-1.0/R_EQ)*dx;
		       fy += BETA * (1.0/distance-1.0/R_EQ)*dy;
		    }
		  vx += ALPHA * particle_j.vx;///V1;
		  vy += ALPHA * particle_j.vy;///V1;
	       }
	     j = particle_j.next;	  
	     if(j>0&&j<N)
	       {
		  if(boid[j].next==j)printf("next é o mesmo boid ***** LOOP i %d j %d next %d\n", i, j, boid[j].next);
		  if(boid[j].next>0&&boid[j].next<N)
		    {
		       test_loop = boid[boid[j].next].next;
		       if(test_loop == j)printf("next é o mesmo boid ***** LOOP 2  i %d j %d  next next %d\n",i ,j, test_loop);
		    }
		  else
		    {
		       if(boid[j].next!=-1)printf("next é fora do escopo ***** LOOP 2  i %d j %d  next %d\n",i ,j, boid[j].next);
		    }
	       }
	     else
	       {
		 if(j!=-1) printf("next é fora do escopo ***** LOOP 2  i %d j %d  next  \n",i ,j);
	       }
	     
	     
	  }
	// box with their neighbors
	for(k=0;k<8;k++)
	  {   
	     j = box_list[neighbor[k]];
	     if(neighbor[k]>total_box || neighbor[k]<0)
	       {
		  printf(" \n");
		  printf("neighbor k %d k = %d box = %d \n",neighbor[k],k, box);
		  printf("particle %d\n",i);
		  printf("Lx: %d Ly: %d \n",Lx,Ly);
		  printf("X: %f Y: %f box: %d \n",boid[i].x,boid[i].y,boid[i].box);
		  printf("X: %f Y: %f box: %d \n",particle_i.x,particle_i.y,particle_i.box);
	       }
 
	     while (j>=0&&j<n)
	       {
		  //printf(" aqui 5.2  particle_i: %d particle_j: %d box: %d %d next j: %d \n",i,j,particle_i.box,particle_j.box,particle_j.next);
		  particle_j = boid[j];
		  if(box_list[neighbor[k]]> n || box_list[neighbor[k]]<0)printf("%d \n",box_list[neighbor[k]]);
		  distance=0.0;
		  dx = particle_i.x - particle_j.x;
		  if(dx<-Lxh)dx+=Lx;else if(dx>Lxh)dx-=Lx;
		  dy = particle_i.y - particle_j.y;
		  if(dy<-Lyh)dy+=Ly;else if(dy>Lyh)dy-=Ly;
		  distance=(dx*dx)+(dy*dy);
		  if (distance < rmax2 && distance > 0.0)
		    {
		       if(distance< R_CORE*R_CORE)
			 {
			    distance = sqrt(distance);
			    fx += F_0 * dx / distance;
			    fy += F_0 * dy / distance;
			 }
		       else
			 {
			    distance = sqrt(distance);
			    fx += BETA * (1.0/distance-1.0/R_EQ)*dx;//distance;
			    fy += BETA * (1.0/distance-1.0/R_EQ)*dy;//distance;
			 }
		       vx +=  ALPHA * particle_j.vx;///V1;
		       vy +=  ALPHA * particle_j.vy;///V1;
		    }
		  j = boid[j].next;
//		  if(j==i)printf("next é o mesmo boid ***** LOOP\n");
//		  if(boid[j].next==i)printf("next do next é o mesmo boid ***** LOOP 2\n");
	       }
	  }
	if(SORTEIO == CANTO)
	  {
	    if(OBSTACULO==1)
	      {
		// testando se está perto da esfera central
		dx = particle_i.x-L_CENTRO_X;
		dy = particle_i.y-L_CENTRO_Y;
		distance_center = sqrt((dx*dx)+(dy*dy));	
		if(distance_center<R_ESFERA+R_CORE)
		  {
		    // adicionar forca pra fora da esfera
		    fx += 1.0 * F_0 * dx / distance_center;
		    fy += 1.0 * F_0 * dy / distance_center;
		  }
	      }
	    // testando se esta perto das paredes 
	    if(PAREDE==1)
	      {
		if(particle_i.y>Ly-R_CORE)
		  {
		    fy -= F_0;
		  }
		else if(particle_i.y<R_CORE)
		  {
		    fy += F_0;
		  }
		if(particle_i.x<R_CORE)
		  {
		    fx += F_0;
		  }
	      }
	  }
	if(particle_i.x<Lx)
	  {
	     sum_vx[i] = vx;
	     sum_vy[i] = vy;
	     sum_fx[i] = fx;
	     sum_fy[i] = fy;
	  }
     }
}
