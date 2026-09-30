#include <stdio.h>

int func4(int param_1)
{
  int iVar1;
  int iVar2;
  
  if (param_1 < 2) {
    iVar2 = 1;
  }
  else {
    iVar1 = func4(param_1 + -1);
    iVar2 = func4(param_1 + -2);
    iVar2 = iVar2 + iVar1;
  }
  return iVar2;
}

int main()
{
  int iVar1 = 1;
  int local_8 = 1;
  
  while (iVar1 != 0x37) {
    iVar1 = func4(local_8);
    printf("%d\n", local_8);
    local_8 += 1;
  }
  return 0;
}
