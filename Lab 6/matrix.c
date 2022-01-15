#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>

int A[100][100], B[100][100], C[100][100];
int n,m,p; 
int rows_c = 0;

void* multi(void* arg)
{
	int i = rows_c++; 

	for (int j = 0; j < p; j++)
	for (int k = 0; k < m; k++)
		C[i][j] += A[i][k] * B[k][j];
}

int main()
{

	printf("Matricea A\n");

    printf("Dimensiuni (randuri, coloane):");
    scanf("%d",&n);
    scanf("%d",&m);
    
    printf("Valorile matricei A:\n");
    for(int x=0;x<n;++x)
        for(int y=0;y<m;y++)
            scanf("%d",&A[x][y]);
    
    printf("\nMatricea B\n");

    printf("Dimensiuni (%d randuri, coloane):", m);
    scanf("%d",&p);
    
    printf("Valorile matricei B:\n");
    for(int x=0;x<m;x++)
        for(int y=0;y<p;y++)
            scanf("%d",&B[x][y]);
    
    printf("Matricea A\n");
    for(int x=0;x<n;x++){
        for(int y=0;y<m;y++)
            printf("%d ",A[x][y]);
        printf("\n");
    }
    
    printf("Matricea B\n");
    for(int x=0;x<m;x++){
        for(int y=0;y<p;y++)
            printf("%d ",B[x][y]);
        printf("\n");
    }

	 printf("Matricea A\n");
    for(int x=0;x<n;x++){
        for(int y=0;y<m;y++)
            printf("%d ",A[x][y]);
        printf("\n");
    }

	printf("Matricea B\n");
    for(int x=0;x<m;x++){
        for(int y=0;y<p;y++)
            printf("%d ",B[x][y]);
        printf("\n");
    }

	pthread_t threads[n];

	for (int i = 0; i < n; i++)
    {
        int x = pthread_create(&threads[i], NULL, multi, NULL);
        if (x != 0)
        {
            perror("Error at pthread_create");
            return errno;
        }
    }

	for (int i = 0; i < n; i++)
    {
        int x = pthread_join(threads[i], NULL);
        if (x != 0)
        {
            perror("Error at pthread_join");
            return errno;
        }
    }
	printf("Matricea C (produsul dintre A si B)\n");
    for(int x=0;x<n;x++){
        for(int y=0;y<p;y++)
            printf("%d ",C[x][y]);
        printf("\n");
    }
	return 0;
}
