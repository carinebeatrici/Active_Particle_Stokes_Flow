typedef struct
{
   float x;
   float y;
   float v0;
   float vx;
   float vy;
   int box;
   int next;
   int type;
}particle;

typedef struct
{
   int begin;
   int end;
   int number;
}int_3d;

typedef struct
{
   int v0;
   int v1;
   int v2;
   int v3;
   int v4;
   int v5;
   int v6;
   int v7;
}neighbor_box;
