#include<stdio.h>
#include<stdlib.h>

int main()
{
	int layers=1;
	int flag=0;
	int i,j=0;
	printf("Num of layers: \n");
	scanf("%d",&layers);
	int arr[layers][layers];
	for(i=0;i<layers;i++)
	{
		for(j=0;j<layers;j++)
		{
			scanf("%d",&arr[i][j]);
		}
	}
	
	for(i=0;i<layers;i++)
	{
		for(j=0;j<layers;j++)
		{
			if(arr[i][j]==arr[j][i])
				continue;
			else
			{
				flag=1;
				break;
			}		
		}
	}
	
	if(flag==0)
	{
		printf("Symmetric.");
	}
	else
	{
		printf("NOT symmetric.");
	}
	return 0;
}
