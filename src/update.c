#include <time.h>
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#include "param.h"
#include "particle.h"

#define TWO_PI 6.283185307

int   update_position(particle *boid, float *sum_vx, float *sum_vy,
		      float *sum_fx, float *sum_fy, int n)
{
   float angle;//movement_angle;
   int i;
   float modulo_deslocamento;
   float componente_x,componente_y;
   int ent_esq=0, sai_dir=0, ent_dir=0;
   float densidade_reservatorio;
   int delta_n;
   float new_x, new_y;
   ent_esq = 0;
   sai_dir = 0;
   ent_dir = 0;
   densidade_reservatorio = 0.0;
   delta_n = 0;
   for(i=0;i<n;i++)
     {

	angle =  TWO_PI * drand48();
/*	if((sum_vx[i]+sum_fx[i])!=0)
	  {
	     movement_angle = atan2(sum_vy[i]+sum_fy[i]+ETA*sin(angle),
				    sum_vx[i]+sum_fx[i]+ETA*cos(angle));
	  }
	else
	  {
	     movement_angle = angle;
	  }
 
	while(movement_angle>TWO_PI)movement_angle-=TWO_PI;
	while(movement_angle<0)movement_angle+=TWO_PI;
*/
	componente_x =  sum_vx[i]+sum_fx[i]+ETA*cos(angle);
	componente_y =  sum_vy[i]+sum_fy[i]+ETA*sin(angle);

	modulo_deslocamento = sqrt((componente_x*componente_x)+(componente_y*componente_y));
	
	boid[i].vx =  boid[i].v0 * componente_x/modulo_deslocamento;
	boid[i].vy =  boid[i].v0 * componente_y/modulo_deslocamento;
	
	new_x = boid[i].x + boid[i].vx;
	new_y = boid[i].y + boid[i].vy;
	
	
	
	
	//contagem das particulas que passaram ou não
	if(SORTEIO == CANTO)
	  {
	     if(new_x>=Lx)ent_esq++;
	     if(boid[i].x<L_CANTO && new_x>L_CANTO) sai_dir++;
	     if(boid[i].x>L_CANTO && new_x<L_CANTO) ent_dir++;
	  }
	
	boid[i].x = new_x; 
	boid[i].y = new_y;
	// condicoes de contorno
	if(SORTEIO == CANTO)
	  {
	    while(boid[i].x<0.0)
	      {
//     	        boid[i].x+=Lx;
		boid[i].x=-boid[i].x;
		boid[i].vx=-boid[i].vx;
	      }
	    while(boid[i].y<0.0)
	      {
//	          boid[i].y+=Ly;
		boid[i].y=-boid[i].y;
		boid[i].vy=-boid[i].vy;
	      }
	    while(boid[i].x>Lx)
	      {
		boid[i].x-=Lx;
	      }
	    while(boid[i].y>=Ly)
	      {
		//	     boid[i].y-=Ly;
		boid[i].y=2.0*Ly-boid[i].y;
		boid[i].vy=-boid[i].vy;
		//	     boid[i].angle=-boid[i].angle;
	      }		
	  }
	else
	  {
	    while(boid[i].x<0.0)boid[i].x+=Lx;
	    while(boid[i].y<0.0)boid[i].y+=Ly;
//	    while(boid[i].x>Lx)boid[i].x-=Lx;
	    while(boid[i].y>Ly)boid[i].y-=Ly;
	  }

//	boid[i].angle = movement_angle;
//	if(SORTEIO==CANTO)
//	  {
	if( boid[i].x<L_CANTO)
	  { 
	    densidade_reservatorio += 1;
	  }
	//	  }
     }
   if(SORTEIO==CANTO)
     {
       delta_n = (int) ( RESEVOIR_DENSITY*(L_CANTO*Ly) - densidade_reservatorio);   
       densidade_reservatorio /= (1.0 * L_CANTO * Ly);
       //       printf("sai_dir: %d ent_dir: %d ent_esq: %d  total: %d  dens. reser.: %f dn: %d n: %d \n",sai_dir, ent_dir, ent_esq, (sai_dir - ent_dir - ent_esq), densidade_reservatorio,delta_n,n);
     }
   else
     {
       delta_n = 0;
     }
	
   
   return(delta_n);
}

