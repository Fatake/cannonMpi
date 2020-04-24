#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#define MAX_RAND 76

int generaRandom();
float *generaMatriz(int row, int column);

//
// Main
//
int main(int argc,char *argv[]){
   int rank,size, row, column, i=0,j=0,k=0;
   float *A,*B,*C,a=0,b=0,c=0;
   
   srand(time(NULL));
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD,&size);
   MPI_Comm_rank(MPI_COMM_WORLD,&rank);
   row = column = 3;

   if(rank == 0){//Si es el core 0
      A = generaMatriz(row, column);
      B = generaMatriz(row, column);

      k=0;
      printf("Matriz A:\n");
      for(i=0; i < row; i++) {
         for(j=0; j < column; j++){
            printf("%f\t",A[k]);
            k++; 
         } 
         printf("\n"); 
      }
      
      k=0;
      printf("\nMatriz B:\n");
      for(i=0;i<row;i++) {
         for(j=0;j<column;j++){
            printf("%f\t",B[k]);
            k++; 
         }
         printf("\n");  
      }
   }

   MPI_Bcast(&row,1,MPI_INT,0,MPI_COMM_WORLD);
   int periods[]={1,1}; //movimiento vertica y horizontal en las matrices 
   int dims[] = {row,row};
   int coords[2]; /* 2 dimensiones, (x,y)*/
   int right=0, left=0, down=0, up=0;    // Vecino ranks
   MPI_Comm cart_comm;
   MPI_Cart_create(MPI_COMM_WORLD,2,dims,periods,1,&cart_comm );
   MPI_Scatter(A,1,MPI_FLOAT,&a,1,MPI_FLOAT,0,cart_comm);
   MPI_Scatter(B,1,MPI_FLOAT,&b,1,MPI_FLOAT,0,cart_comm);
   MPI_Comm_rank(cart_comm,&rank);
   MPI_Cart_coords(cart_comm,rank,2,coords);
   MPI_Cart_shift(cart_comm, 1, coords[0], &left,&right);
   MPI_Cart_shift(cart_comm, 0, coords[1], &up,&down);
   MPI_Sendrecv_replace(&a,1,MPI_FLOAT,left,11,right,11,cart_comm,MPI_STATUS_IGNORE);
   MPI_Sendrecv_replace(&b,1,MPI_FLOAT,up,11,down,11,cart_comm,MPI_STATUS_IGNORE);
   
   c = c + a*b;
   for(i=1; i<row ;i++){
      MPI_Cart_shift(cart_comm, 1, 1, &left,&right);
      MPI_Cart_shift(cart_comm, 0, 1, &up,&down);
      MPI_Sendrecv_replace(&a,1,MPI_FLOAT,left,11,right,11,cart_comm,MPI_STATUS_IGNORE);
      MPI_Sendrecv_replace(&b,1,MPI_FLOAT,up,11,down,11,cart_comm,MPI_STATUS_IGNORE);
      c = c + a*b;
   }
   C=(float*)calloc(sizeof(float),row*row);
   MPI_Gather(&c,1,MPI_FLOAT,C,1,MPI_FLOAT,0,cart_comm);
   if(rank==0){
      k=0; 
      printf("\nA * B:\n");
      for(i=0;i<row;i++){
         for(j=0;j<row;j++){
            printf("%f\t",C[k]);
            k++;
         }    
         printf("\n");
      }
   }
   MPI_Finalize();
   return 0;
}

int generaRandom(){
   return rand() % MAX_RAND;
}

float *generaMatriz(int row, int column){
   int i,j,k = 0;
   float *matAux = (float*)calloc(sizeof(float),row*column);
   for(i=0; i<row; i++) {
         for(j=0; j<column; j++){
            matAux[k] = (float)generaRandom();
            k++; 
         } 
   }
   return matAux;
}