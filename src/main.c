/* Modelo de Vicsek 
 * Inicio: 28/07/2016
 * 
 * 
 */ 

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#define TWO_PI 6.283185307

#include "param.h"
#include "particle.h"



long int  initialize(particle *boid, int n);
void  save_position(FILE *arquivo, particle *boid, int time_step, int n);  
void  calculate_distance(particle *boid, float *sum_vx, float *sum_vy,
			 float *sum_fx, float *sum_fy,
			 const int box_for_line,
			 const int box_for_row,  neighbor_box *neighbor_box_list, 
			 int *box_list, int n);
int update_position(particle *boid, float *sum_vx, float *sum_vy,
		     float *sum_fx, float *sum_fy, int n);
void calc_box(particle *boid, const int box_for_line, int n);
void SaveImageDensity(particle *boid, const int box_for_line, const int box_for_row,
		     int *imagem, float *vx_mean, float *vy_mean, float *phi_local, float *angle_local, int n);
float PolarOrderParameter(particle *boid, int time, int n);
void neighbor_box_calc(neighbor_box *neighbor_box_list, const int box_for_line, const int box_for_row);

int main (void)
{
   int n, delta_n;
   int i;
   float *sum_vx,*sum_vy; 
   float *sum_fx,*sum_fy; 
   float angle;
   particle *boid;
   int *box_list;
   int retorno = 0, retorno2 = 0;
   float tempo_init = 0 , tempo_calc_box = 0 , tempo_distance = 0 , tempo_file = 0, tempo_outro = 0, tempo_total = 0;
   clock_t clock_init, clock_end;
   clock_init = clock();
   long int time_step=0, tnp=0, time_out=0;
   const int box_for_line =  (int)(Lx/R_MAX);
   const int box_for_row  =  (int)(Ly/R_MAX);
   const int total_box = box_for_line * box_for_row;
   int *imagem; 
   float *vx_mean, *vy_mean;
   float *phi_local, *angle_local;
   float phi = 0.0;
   srand48(time(0));
   neighbor_box *neighbor_box_list;
   FILE *arquivo_saida;
   float max_x;
   int ja_foi = 0;

   // Allocating stuff   
   retorno +=  posix_memalign((void**)&box_list, 16, total_box*sizeof(int));
   if(retorno > 0)printf("Problema com a posix dentro da distancia \n");
   retorno += posix_memalign((void**)&boid, 64, N*sizeof(particle));
   if (retorno > 0) printf("Erro com a alocacao da posix boid \n");
   retorno += posix_memalign((void**)&sum_vx, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_vx \n");
   retorno += posix_memalign((void**)&sum_vy, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_vy \n");
   retorno += posix_memalign((void**)&sum_fx, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_fx \n");
   retorno += posix_memalign((void**)&sum_fy, 64, N*sizeof(float));
   if (retorno > 0) printf("Erro com a alocacao da posix sum_fy \n");
   retorno += posix_memalign((void**)&neighbor_box_list, 64,  (total_box*sizeof(neighbor_box)));
   if (retorno > 0) printf("Erro com a alocacao da posix \n");
   
   if(LOAD==0)
     {
	retorno2 += system("rm data//phi.dat");
	retorno2 += system("rm data//posicoes.dat");
	retorno2 += system("rm data//image.dat");
	retorno2 += system("rm data//localphi.dat");
     }
   if (retorno2 > 0) printf("Erro com a remoção dos arquivos \n");
 
   neighbor_box_calc(neighbor_box_list, box_for_line, box_for_row);
   retorno += posix_memalign((void**)&imagem, 16, total_box*sizeof(int));
   retorno += posix_memalign((void**)&vx_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&vy_mean, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&phi_local, 64, total_box*sizeof(float));
   retorno += posix_memalign((void**)&angle_local, 64, total_box*sizeof(float));
   
   if(SORTEIO==CANTO)
     {
       n = L_CANTO * Ly * RESEVOIR_DENSITY;
     }
   else
     {
       n = N;
     }
   printf("Numero inicial de boids........... %d ................................... OK\n",n);
   
   time_step = initialize(boid, n);
   tnp=time_step+pow(time_step,0.8);
   fflush(stdout);
   arquivo_saida=fopen("data//posicoes.dat","a");   
   fflush(stdout);

   clock_end = clock();
   tempo_init = clock_end - clock_init;
   clock_init = clock();
   calc_box(boid, box_for_line, n);
   clock_end = clock();
   tempo_calc_box = clock_end - clock_init;
   clock_init = clock();
   
   printf("Inicio da execução......................................................... OK\n");
   printf("Numero de particulas:.................. %d .............................. OK \n",N);
   printf("Tipo de sorteio: %d ........................................................ OK \n",SORTEIO);
   printf("Alpha:..%f ................................................................. \n",   ALPHA);
   printf("Beta:..%f ................................................................. \n",   BETA);
//   printf("Alpha:..%f ....%f.............................................. \n",   ALPHA11,ALPHA12);
//   printf("Alpha:..%f ....%f.............................................. OK \n",ALPHA12,ALPHA22);
//   printf("Beta:...%f ....%f.............................................. \n",   BETA11,BETA12);
//   printf("Beta:...%f ....%f.............................................. OK \n",BETA12,BETA22);
   printf("Velocidade: .... %f........................................... OK \n",V1);
   printf("Lx: ........ %d ........ Ly: ......... %d ........................... OK \n",Lx,Ly);
   if(LOAD==1)
     {
	printf("Tempo: ........ %li ............. ............................................ OK \n",time_step);
     }
   
   if(LOAD==0)save_position(arquivo_saida, boid, time_step, n);
//   if(LOAD==0)SaveImageDensity(boid, box_for_line, box_for_row, imagem, vx_mean, vy_mean, phi_local, angle_local,n);
   clock_end = clock();
   tempo_file = clock_end - clock_init;
   clock_init = clock();  
   time_step = 1;
   while (time_step < TIME_FINAL)
     {
	if(ja_foi == 0)
	  {
	     time_out ++; 
	     max_x =  boid[0].x;
	     for(i=1; i<n; i++)
	       {
		  if(boid[i].x > max_x)
		    max_x = boid[i].x;
	       }
	     if(time_out%SNAPSHOT==0)printf("time step trhow away: %li max x: %f barreira: %f \n", time_out, max_x, L_CENTRO_X+(R_FRONT*R_ESFERA));
	     if(max_x >= L_CENTRO_X+(R_FRONT*R_ESFERA))
	       ja_foi = 1;
	  }
	else
	  {
	     time_step++;
	  }	
	calc_box(boid, box_for_line, n);
	clock_end = clock();
	tempo_calc_box += clock_end - clock_init;
//	for(i=0;i<n;i++)printf("%f %f %f %f %d\n",boid[i].x,boid[i].y,boid[i].vx,boid[i].vy,boid[i].box);
	clock_init = clock();
	calculate_distance(boid, sum_vx, sum_vy, sum_fx, sum_fy, box_for_line, box_for_row, neighbor_box_list, box_list, n);
	clock_end = clock();
	tempo_distance += clock_end - clock_init;
	clock_init = clock();
	delta_n = update_position(boid, sum_vx, sum_vy, sum_fx, sum_fy, n);
//	printf("Numero de boids: .... %d ......delta n: ..... %d .................... OK\n",n,delta_n);
	if(delta_n>0 && n+delta_n <= N)
	  { 
//	     printf("Numero de boids: .... %d ......delta n: ..... %d .................... OK\n",n,delta_n);
	     //inicializando as novas particulas
	      for(i=n;i<n+delta_n;i++)
	       {  
		  angle = drand48()*TWO_PI;
		  boid[i].vx = cos(angle);
		  boid[i].vx = sin(angle);
		  boid[i].x  = L_CANTO * drand48();
		  boid[i].y  = Ly * drand48();
		  boid[i].box=-1;
		  boid[i].next=-1;
		  boid[i].v0 = V1;
		  boid[i].type=0;
	       }
	     n += delta_n;
	  }
	clock_end = clock();
	tempo_outro += clock_end - clock_init;
	clock_init = clock();
	if(SNAPSHOT==-1)
	  {
	     if(time_step>=tnp)
	       {
		  tnp=tnp+pow(time_step,0.8);
		  save_position(arquivo_saida, boid, time_step, n);
//		  SaveImageDensity(boid, box_for_line, box_for_row,  imagem, vx_mean, vy_mean, phi_local, angle_local, n);
		  phi = PolarOrderParameter(boid, time_step, n);
		  printf("Salvando posicoes %li %f %d \n",time_step,phi,n);
		  clock_end = clock();
		  tempo_file += clock_end - clock_init;
		  clock_init = clock();
		  
	       }
	  }
	else
	  {
	     if(time_step%SNAPSHOT==0)
	       {
		  save_position(arquivo_saida, boid, time_step, n);
//		  SaveImageDensity(boid, box_for_line, box_for_row,  imagem, vx_mean, vy_mean, phi_local, angle_local, n);
		  phi = PolarOrderParameter(boid, time_step, n);
		  printf("Salvando posicoes %li %f %d \n",time_step,phi,n);
		  clock_end = clock();
		  tempo_file += clock_end - clock_init;
		  clock_init = clock();
	       }
	  }
     }
   // LIBERATING MEMORY
   //   sum_vx = free (N, sum_vx);
   //   sum_vy = free, sum_vy);
   clock_end = clock();
   tempo_outro += clock_end - clock_init;
   tempo_total = tempo_init+tempo_calc_box+tempo_distance+tempo_file+tempo_outro;
   printf("tempo total %f s \n", tempo_total/((float)CLOCKS_PER_SEC));
   printf("tempo init  %f s \n", tempo_init/((float)CLOCKS_PER_SEC));
   printf("tempo box   %f s \n", tempo_calc_box/((float)CLOCKS_PER_SEC));
   printf("tempo file  %f s \n", tempo_file/((float)CLOCKS_PER_SEC));
   printf("tempo dist  %f s \n", tempo_distance/((float)CLOCKS_PER_SEC));
   printf("tempo other %f s \n", tempo_outro/((float)CLOCKS_PER_SEC));
   //if(arquivo_saida!=NULL)fclose(arquivo_saida);
   //if(box_list!=NULL)free(box_list);
   //if(sinangle!=NULL)free(sinangle);
   //if(cosangle!=NULL)free(cosangle);
   printf("Final da execução :D \n");
   return 0;
}

