#include <math.h>
#include <stdlib.h>
#include <stdio.h>

int main(int argc, char** argv)
{
	int x = atoi(argv[1]);
	printf("x = %d\n", x);
        double sqrtX = sqrt((double)x);
        printf("sqrt(x) = %f\n", sqrtX);

	if (sqrtX < 5)
	{
		printf("sqrtX < 5\n");
	}

        return 0;
}
