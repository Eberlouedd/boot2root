#include <stdio.h>

void explode_bomb()
{
	printf("boom");
}

int main()
{
	int iVar1;
  	int aiStack_20 [7];
	aiStack_20[0] = 0;
	aiStack_20[1] = 1;
	if (aiStack_20[1] != 1) {
		explode_bomb();
	}
	iVar1 = 1;
	do {
		aiStack_20[iVar1 + 1] = (iVar1 + 1) * aiStack_20[iVar1];
		iVar1 = iVar1 + 1;
	} while (iVar1 < 6);
	printf("%d ", aiStack_20[1]);
	printf("%d ", aiStack_20[2]);
	printf("%d ", aiStack_20[3]);
	printf("%d ", aiStack_20[4]);
	printf("%d ", aiStack_20[5]);
	printf("%d", aiStack_20[6]);
	return 0;
}