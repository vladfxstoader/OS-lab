#include <stdio.h>
#include <stdlib.h>

using namespace std;
int main()
{
    int n;
    scanf("%d", &n);
    int *mat=(int*)malloc(n*n*sizeof(int));
    int i=0;
    int j;
    int offset;
    while (i<n)
    {
        j=0;
        while (j<n)
        {
            int x;
            scanf("%d", &x);
            offset=i*n+j;
            *(mat+offset)=x;
            ++j;
        }
        ++i;
    }
    printf("\nAfisarea matricei:\n");
    i=0;
    while (i<n)
    {
        j=0;
        while (j<n)
        {
            offset=i*n+j;
            printf("%d ",*(mat+offset));
            ++j;
        }
        ++i;
        printf("\n");
    }
    printf("\nAfisarea elementului din mijloc:\n");
    if (n%2)
    {
        offset=(n/2)*n+(n/2);
        printf("%d\n",*(mat+offset));
    }
    else
        printf("Matricea nu are elementul din mijloc! (n e par)\n");
    i=0;
    printf("\nAfisarea elementelor de pe diagonala principala:\n");
    while (i<n)
    {
        offset=i*n+i;
        printf("%d ",*(mat+offset));
        ++i;
    }
    i=0;
    printf("\n\nAfisarea elementelor de pe diagonala secundara:\n");
    while (i<n)
    {
        offset=i*n+(n-1-i);
        printf("%d ",*(mat+offset));
        ++i;
    }
    printf("\n");
    free(mat);
    return 0;
}
