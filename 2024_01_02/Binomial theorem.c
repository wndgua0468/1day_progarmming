#include <stdio.h>


int binomialCoefficient(int n, int k)
{
    if (k == 0 || k == n)
    {
        return 1;
    }
    else
    {
        return binomialCoefficient(n - 1, k - 1) + binomialCoefficient(n - 1, k);
    }
}


void expandExpression(int n)
{
    for (int k = 0; k <= n; k++)
    {
        int coefficient = binomialCoefficient(n, k);

        if (coefficient != 1)
        {
            printf("%d", coefficient);
        }

        if (n - k > 0)
        {
            printf("x^%d", n - k);
        }

        if (k > 0)
        {
            printf("a^%d", k);
        }

        if (k < n)
        {
            printf(" + ");
        }
    }
    printf("\n");
}

int main()
{
    int n;

    // 사용자로부터 정수 n 입력
    printf("정수 n을 입력하세요: ");
    scanf_s("%d", &n);

    // (x+a)^n을 전개하여 출력
    expandExpression(n);

    return 0;
}
