#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "param.h"
#include "particle.h"

void  save_position(FILE *arquivo, particle *boid, int time_step, int n)
{
   int i;
   fprintf(arquivo,"%d %d \n",time_step,n);
   for(i=0;i<n;i++)fprintf(arquivo,"%f %f %f %f %d\n",boid[i].x,boid[i].y,boid[i].vx,boid[i].vy,boid[i].box);
   fflush(stdout);
}


float PolarOrderParameter( particle *boid,  int time, int n)
{
   float sum,sum_x,sum_y;
   FILE *file;
   file=fopen("data//phi.dat","a");
   if( !file )
     {	
	printf("unable to create the file in SaveParticlesBin\n");
//	return 1;
	sum = 0.0;
     }
   
   if( V1 == 0.0)
     sum = 0.0;
//     return 0.0;

   int i;
   sum_x = 0.0;
   sum_y = 0.0;
   for(i=0; i<n; i++)
     {
	sum_x += boid[i].vx;
	sum_y += boid[i].vy;
	//      printf("%f %f",senangle[GP[i].angle],cosangle[GP[i].angle]);
     }
   sum=sqrt(sum_x*sum_x+sum_y*sum_y);
   sum/=(V1*(float)n);
   fprintf(file,"%d  %f  %f  %2.2f %d \n",time,sum,atan2(sum_y,sum_x),(float)(n/(Lx*Ly)),n);
   fclose(file);
   return sum;
}
	// 


void SaveImageDensity(particle *boid, const int box_for_line, const int box_for_row, 
		     int *imagem, float *vx_mean, float *vy_mean, float *phi_local, float *angle_local, int n)
{
   int i,caixas_x,caixas_y;
//   int *imagem;
//   int retorno = 0;
//   int total_box = box_for_line * box_for_row;
//   float *vx_mean, *vy_mean;
//   float *phi_local, *angle_local;
   FILE *file,*file2;
//   printf(" aqui 3.7.6 \n");
   file=fopen("data//image.dat","a");
   file2=fopen("data//localphi.dat","a");
   if( !file )
     {
	
	printf("unable to create the file in SaveParticlesBin\n");
	exit(0);
     }
  // printf(" aqui 3.7.7 \n");
  /* 
   retorno += posix_memalign((void**)&imagem, 16, total_box*sizeof(int));
   retorno += posix_memalign((void**)&vx_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&vy_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&phi_local, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&angle_local, 64, total_box*sizeof(float));
      
    if(retorno > 0)printf("Problema com a posix dentro da save positions \n");
   */
   for(i=0;i<box_for_line*box_for_row;i++)
     {
	imagem[i]=0;
	vx_mean[i]=0;
	vy_mean[i]=0;
	phi_local[i]=0.0;
	angle_local[i]=10.0;
     }
   
   //printf(" aqui 3.7.8 \n");
   for(i=0;i<n;i++)
     {
	caixas_x=(int)(boid[i].x/R_MAX);
	caixas_y=(int)((Ly-boid[i].y)/R_MAX);
	imagem[caixas_y*box_for_line+caixas_x]++;
	vx_mean[caixas_y*box_for_line+caixas_x]+=boid[i].vx;
	vy_mean[caixas_y*box_for_line+caixas_x]+=boid[i].vy;
     }
   //printf(" aqui 3.7.9 \n");
   for(i=0;i<box_for_line*box_for_row;i++)
     {	
	if(imagem[i]>0)phi_local[i]=sqrt((vx_mean[i]*vx_mean[i])+(vy_mean[i]*vy_mean[i]))/imagem[i];
	if(imagem[i]>0)angle_local[i]=atan2(vy_mean[i],vx_mean[i]);
     }
   for(i=0;i<box_for_line*box_for_row;i++)	fprintf(file,"%d ",imagem[i]);
   for(i=0;i<box_for_line*box_for_row;i++)	fprintf(file2," %f ",phi_local[i]);
   //printf(" aqui 3.7.10 \n");
   for(i=0;i<box_for_line*box_for_row;i++)fprintf(file2," %f ",angle_local[i]);
   fprintf(file," \n ");
   fprintf(file2," \n ");
   fflush(stdout);
   fclose(file);
   fclose(file2);
 /*  free(imagem);
   free(vx_mean);
   free(vy_mean);
   free(phi_local);
   free(angle_local);
  */ 
}
