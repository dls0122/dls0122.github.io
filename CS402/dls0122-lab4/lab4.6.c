#include<stdio.h>
int ack(int x, int y)
{
	if (x == 0)
	{
		return y + 1;
	}
	else if (y == 0)
	{
		return ack(x - 1, 1);
	}
	else //if (x > 0 && y > 0)
	{
		return ack(x - 1, ack(x, y - 1));
	}
 
}
 
int main()
{
	int x,y ;
	scanf_s("%d,%d",&x,&y); 
	printf("%d", ack(x, y));
 
	return 0;
}