#include <stdio.h>

int n, m, max, a[10][10], d[7][7], p[10], b[10], c[10];

void x(void)
{
	int i, j, cnt = 0, z = 1;

	for (i = 1; i <= n; i++)
		for (j = 1; j <= n; j++)
			d[i][j] = 0;

	for (i = 1; i <= n; i++)
		for (j = 1; j <= n; j++)
			if (a[i][j] && c[i] && c[j])
				d[b[i]][b[j]]++;

	for (i = 1; i <= 6; i++)
		for (j = 1; j <= 6; j++)
			if (d[i][j] > 1)
				z = 0;
			else if (i != j)
				cnt += d[i][j];
			else
				cnt += d[i][j] * 2;
	
	cnt /= 2;

	if (0)
	{
		for (i = 1; i <= n; i++)
		{
			for (j = 1; j <= n; j++)
				printf("%d ", d[i][j]);
			printf("\n");
		}
	}
	printf("%d %d\n", z, cnt);

	if (z && max < cnt)
		max = cnt;
}
void h(int lev)
{
	if (lev > n)
		x();
	else
		for (int i = 0; i <= 1; i++)
		{
			c[lev] = i;
			h(lev + 1);
		}
}
void g(int k)
{
	int i, cnt = 0;

	for (i = 1; i <= n; i++)
		b[i] = i;
	for (i = 1; i <= k; i += 2)
		b[p[i + 1]] = b[p[i]];

	h(1);
}
void f(int lev, int k)
{
	if (lev > k)
		g(k);
	else
		for (int i = p[lev - 1] + 1; i <= n; i++)
		{
			p[lev] = i;
			f(lev + 1, k);
		}
}
int main(void)
{
	int i, j, x, q;

	scanf("%d %d", &n, &m);
	for (i = 1; i <= m; i++)
	{
		scanf("%d %d", &x, &q);
		a[x][q] = 1;
		a[q][x] = 1;
	}

	for (i = (n != 7 ? 0 : 1); i <= n / 2; i++)
	{
		for (j = 1; j <= 2 * i; j++)
			p[j] = j;
		f(1, i * 2);
	}

	printf("%d", max);

	return 0;
}