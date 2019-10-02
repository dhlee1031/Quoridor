#include <stdio.h>
#include <unordered_map>
#include <queue>
#define N 3
#define B 0

using namespace std;
typedef long long ll;
unordered_map<ll, int> M;
queue<int> Q;

ll coorblk, cnt;
int p1bn, p2bn;
int px1, py1, px2, py2;
int pbk[N + 1][N + 1];
int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
int px[4][2] = { {-1,-1}, {-1,0}, {0,0}, {0,-1} }, py[4][2] = { {-1,0}, {0,0}, {0,-1}, {-1,-1} };

void make_coorblk(int x1, int y1, int x2, int y2)
{
	coorblk = 0;	coorblk |= p1bn;
	coorblk <<= 3;	coorblk |= p2bn;
	coorblk <<= 3;	coorblk |= x1;
	coorblk <<= 3;	coorblk |= y1;
	coorblk <<= 3;	coorblk |= x2;
	coorblk <<= 3;	coorblk |= y2;
	coorblk <<= 32;

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			if (pbk[i][j] == 1)
				coorblk |= ((ll)1 << ((i - 1)*(N - 1) + j - 1));
			else if (pbk[i][j] == 2)
				coorblk |= ((ll)1 << ((i - 1)*(N - 1) + j - 1 + (N - 1)*(N - 1)));
}
void get_coorblk(void)
{
	ll temp = coorblk;

	int m = (N - 1)*(N - 1);
	for (int i = 0; i < m; i++)
		if (((ll)1 << i) & coorblk)
			pbk[i % (N - 1) + 1][i / (N - 1)] = 1;	//세로
	for (int i = m; i < m * 2; i++)
		if (((ll)1 << i) & coorblk)
			pbk[i % (N - 1) + 1][i / (N - 1)] = 2;	//가로

	coorblk >>= 32;
	py2 = coorblk & 7;	coorblk >>= 3;
	px2 = coorblk & 7; coorblk >>= 3;
	py1 = coorblk & 7; coorblk >>= 3;
	px1 = coorblk & 7; coorblk >>= 3;
	p2bn = coorblk & 7; coorblk >>= 3;
	p1bn = coorblk & 7;

	coorblk = temp;
}
bool small_range(int x)
{
	if (1 <= x && x < N)
		return true;
	return false;
}
bool large_range(int x)
{
	if (1 <= x && x <= N)
		return true;
	return false;
}
bool can_go(int x, int y, int k)
{
	int x1 = x + px[k][0], y1 = y + py[k][0], x2 = x + px[k][1], y2 = y + py[k][1];

	if (!(large_range(x + dx[k]) && large_range(y + dy[k])))
		return false;
	if (small_range(x1) && small_range(y1) && pbk[x1][y1] == k % 2 + 1)
		return false;
	if (small_range(x2) && small_range(y2) && pbk[x2][y2] == k % 2 + 1)
		return false;
	return true;
}
bool pbk_exist(int x, int y, int k)
{
	if (small_range(x) && small_range(y) && pbk[x][y] == k)
		return false;
	return true;
}
bool blk_ok(int z, int x, int y, int d)
{
	get_coorblk();

	if (z == 1 && p1bn == 0 || z == 2 && p2bn == 0)
		return false;
	if (!(d == 1 && pbk[x][y] == 0 && pbk_exist(x, y - 1, 1) && pbk_exist(x, y + 1, 1)
	   || d == 2 && pbk[x][y] == 0 && pbk_exist(x - 1, y, 2) && pbk_exist(x + 1, y, 2)))
		return false;

	int i, end = 0, xx, yy, vst[N + 1][N + 1] = { 0 };
	while (!Q.empty())
		Q.pop();
	if (z == 1) { Q.push(px1); Q.push(py1); vst[px1][py1] = 1; }
	else { Q.push(px2); Q.push(py2); vst[px2][py2] = 1; }

	while (!Q.empty() && !end)
	{
		xx = Q.front(); Q.pop();
		yy = Q.front(); Q.pop();
		if (z == 1 && yy == N || z == 2 && yy == 1)
		{
			end = 1;
			continue;
		}
		for (i = 0; i < 4; i++)
			if (can_go(xx, yy, i) && vst[xx][yy] == 0) {
				Q.push(xx + dx[i]); Q.push(yy + dy[i]); vst[xx + dx[i]][yy + dy[i]] = 1;
			}
	}

	if (end)
		return true;
	return false;
}
void f(int);
int insert(int i, int y, int n, int k)
{
	if (M.find(coorblk) == M.end())
		if (y == n)
		{
			M.insert(make_pair(coorblk, k));
			return 10;
		}
		else
		{
			M.insert(make_pair(coorblk, 0));
			f(3 - k);
		}
	return i;
}
void f(int k)
{
	get_coorblk();
	int i, j, tx1 = px1, ty1 = py1, tx2 = px2, ty2 = py2, t1bn = p1bn, t2bn = p2bn, tbk[N + 1][N + 1];
	for (i = 1; i < N; i++)
		for (j = 1; j < N; j++)
			tbk[i][j] = pbk[i][j];

	if (k == 1)
	{
		for (i = 1; i < 5; i++)
			if (can_go(tx1, ty1, i % 4))
			{
				if (tx1 + dx[i % 4] == tx2 && ty1 + dy[i % 4] == ty2)
				{
					if (can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], i % 4))
					{
						make_coorblk(tx1 + 2 * dx[i % 4], ty1 + 2 * dy[i % 4], tx2, ty2);
						i = insert(i, ty1 + 2 * dy[i % 4], N, k);
					}
					else
					{
						if (can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 1) % 4))
						{
							make_coorblk(tx1 + dx[i % 4] + dx[(i + 1) % 4], ty1 + dy[i % 4] + dy[(i + 1) % 4], tx2, ty2);
							i = insert(i, ty1 + dy[i % 4] + dy[(i + 1) % 4], N, k);
						}
						if (can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 3) % 4))
						{
							make_coorblk(tx1 + dx[i % 4] + dx[(i + 3) % 4], ty1 + dy[i % 4] + dy[(i + 3) % 4], tx2, ty2);
							i = insert(i, ty1 + dy[i % 4] + dy[(i + 3) % 4], N, k);
						}
					}
				}
				else
				{
					make_coorblk(tx1 + dx[i % 4], ty1 + dy[i % 4], tx2, ty2);
					i = insert(i, ty1 + dy[i % 4], N, k);
				}
			}
	}
	else
	{
		for (i = 3; i < 7; i++)
			if (can_go(tx2, ty2, i % 4))
			{
				if (tx2 + dx[i % 4] == tx1 && ty2 + dy[i % 4] == ty1)
				{
					if (can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], i % 4))
					{
						make_coorblk(tx1, ty1, tx2 + 2 * dx[i % 4], ty2 + 2 * dy[i % 4]);
						i = insert(i, ty2 + 2 * dy[i % 4], 1, k);
					}
					else
					{
						if (can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 1) % 4))
						{
							make_coorblk(tx1, ty1, tx2 + dx[i % 4] + dx[(i + 1) % 4], ty2 + dy[i % 4] + dy[(i + 1) % 4]);
							i = insert(i, ty2 + dy[i % 4] + dy[(i + 1) % 4], 1, k);
						}
						if (can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 3) % 4))
						{
							make_coorblk(tx1, ty1, tx2 + dx[i % 4] + dx[(i + 3) % 4], ty2 + dy[i % 4] + dy[(i + 3) % 4]);
							i = insert(i, ty2 + dy[i % 4] + dy[(i + 3) % 4], 1, k);
						}
					}
				}
				else
				{
					make_coorblk(tx1, ty1, tx2 + dx[i % 4], ty2 + dy[i % 4]);
					i = insert(i, ty2 + dy[i % 4], 1, k);
				}
			}
	}
	if (i != 10)
	{
		int t = 0;
		for (int l = 1; l <= 2; l++)
			for (int i = 1; i < N; i++)
				for (int j = 1; j < N; j++)
					if (blk_ok(k, i, j, l))
					{
						
						tbk[i][j] = l;
						if (k == 1) t1bn--;
						else t2bn--;
						make_coorblk(tx1, ty1, tx2, ty2);
						t = insert(0, 0, 1, k);

						tbk[i][j] = 0;
						if (k == 1) t1bn++;
						else t2bn++;
						make_coorblk(tx1, ty1, tx2, ty2);
					}
	}
}
int main(void)
{
	p1bn = p2bn = B;
	make_coorblk(N / 2 + 1, 1, N / 2 + 1, N);
	M.insert(make_pair(coorblk, 0));
	f(1);

	FILE *out = fopen("out.txt", "w");

	auto iter = M.begin();
	for (; iter != M.end(); ++iter)
	{
		cnt++;
		coorblk = iter->first;
		get_coorblk();
		int a[N + 1][N + 1] = { 0 };
		a[N - py1 + 1][px1] = 1;
		a[N - py2 + 1][px2] = 2;
		if (pbk[1][1] + pbk[1][2] + pbk[2][1] + pbk[2][2] == 0)
		{
			for (int i = 1; i <= N; i++)
			{
				for (int j = 1; j <= N; j++)
					fprintf(out, "%d ", a[i][j]);
				fprintf(out, "\n");
			}
			for (int i = 1; i < N; i++)
			{
				for (int j = 1; j < N; j++)
					fprintf(out, "%d ", pbk[i][j]);
				fprintf(out, "\n");
			}
			fprintf(out, "\n");
		}
	}
	fprintf(out, "%lld\n", cnt);
	

	return 0;
}