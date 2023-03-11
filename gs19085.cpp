#include <stdio.h>
#include <unordered_map>
#include <queue>
#include <vector>

using namespace std;
typedef long long ll;
unordered_map<ll, int> M, P;
unordered_map<int, ll> K;
queue<int> Q;
vector<vector<int> > V;

ll coorblk, cnt, prev;
int N, B;
int p1bn, p2bn, ccnt;
int px1, py1, px2, py2;
int pbk[6][6];
int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
int px[4][2] = { {-1,-1}, {-1,0}, {0,0}, {0,-1} }, py[4][2] = { {-1,0}, {0,0}, {0,-1}, {-1,-1} };
int turn, vst[10000000];
char filename[128];
FILE *in, *out;

void make_coorblk(int k, int x1, int y1, int x2, int y2)
{
	coorblk = k - 1;
	coorblk <<= 3;	coorblk |= p1bn;
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

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			pbk[i][j] = 0;
	int m = (N - 1)*(N - 1);
	for (int i = 0; i < m; i++)
		if (((ll)1 << i) & coorblk)
			pbk[i / (N - 1) + 1][i % (N - 1) + 1] = 1;	//세로
	for (int i = m; i < m * 2; i++)
		if (((ll)1 << i) & coorblk)
			pbk[(i - m) / (N - 1) + 1][(i - m) % (N - 1) + 1] = 2;	//가로

	coorblk >>= 32;
	py2 = coorblk & 7;	coorblk >>= 3;
	px2 = coorblk & 7;	coorblk >>= 3;
	py1 = coorblk & 7;	coorblk >>= 3;
	px1 = coorblk & 7;	coorblk >>= 3;
	p2bn = coorblk & 7;	coorblk >>= 3;
	p1bn = coorblk & 7; coorblk >>= 3;
	turn = (coorblk & 1) + 1;

	coorblk = temp;
}
void show_coorblk(void)
{
	get_coorblk();
	int a[6][6] = { 0 };
	a[N - py1 + 1][px1] = 1;
	a[N - py2 + 1][px2] = 2;

	fprintf(out, "whos turn? %d\n", turn);
	fprintf(out, "player location:\n");
	for (int i = 1; i <= N; i++)
	{
		for (int j = 1; j <= N; j++)
			fprintf(out, "%d ", a[i][j]);
		fprintf(out, "\n");
	}
	fprintf(out, "block location:\n");
	for (int i = 1; i < N; i++)
	{
		for (int j = 1; j < N; j++)
			fprintf(out, "%d ", pbk[j][N - i]);
		fprintf(out, "\n");
	}
	fprintf(out, "p1bn, p2bn:\n");
	fprintf(out, "%d %d\n", p1bn, p2bn);
	fprintf(out, "\n");
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
bool pbk_empty(int x, int y, int k)
{
	if (small_range(x) && small_range(y) && pbk[x][y] == k)
		return false;
	return true;
}
bool can_go(int x, int y, int k)
{
	int x1 = x + px[k][0], y1 = y + py[k][0], x2 = x + px[k][1], y2 = y + py[k][1];

	if (large_range(x + dx[k]) && large_range(y + dy[k]) && pbk_empty(x1, y1, k % 2 + 1) && pbk_empty(x2, y2, k % 2 + 1))
			return true;
	return false;
}
bool blk_ok(int z, int x, int y, int d)
{
	get_coorblk();

	if (z == 1 && p1bn == 0 || z == 2 && p2bn == 0)
		return false;
	if (!(d == 1 && pbk[x][y] == 0 && pbk_empty(x, y - 1, 1) && pbk_empty(x, y + 1, 1)
	   || d == 2 && pbk[x][y] == 0 && pbk_empty(x - 1, y, 2) && pbk_empty(x + 1, y, 2)))
		return false;

	int i, end1 = 0, end2 = 0, xx, yy, vst[6][6] = { 0 };
	pbk[x][y] = d;
	while (!Q.empty())
		Q.pop();
	Q.push(px1); Q.push(py1);
	vst[px1][py1] = 1;
	while (!Q.empty() && !end1)
	{
		xx = Q.front(); Q.pop();
		yy = Q.front(); Q.pop();
		if (yy == N)
		{
			end1 = 1;
			continue;
		}
		for (i = 0; i < 4; i++)
			if (can_go(xx, yy, i) && vst[xx + dx[i]][yy + dy[i]] == 0) {
				Q.push(xx + dx[i]); Q.push(yy + dy[i]); vst[xx + dx[i]][yy + dy[i]] = 1;
			}
	}

	for (int i = 0; i < 6; i++)
		for (int j = 0; j < 6; j++)
			vst[i][j] = 0;
	while (!Q.empty())
		Q.pop();
	Q.push(px2); Q.push(py2);
	vst[px2][py2] = 1;
	while (!Q.empty() && !end2)
	{
		xx = Q.front(); Q.pop();
		yy = Q.front(); Q.pop();
		if (yy == 1)
		{
			end2 = 1;
			continue;
		}
		for (i = 0; i < 4; i++)
			if (can_go(xx, yy, i) && vst[xx + dx[i]][yy + dy[i]] == 0) {
				Q.push(xx + dx[i]); Q.push(yy + dy[i]); vst[xx + dx[i]][yy + dy[i]] = 1;
			}
	}
	pbk[x][y] = 0;

	if (end1 && end2)
		return true;
	return false;
}
void f(int);
int insert(ll prev, int i, int y, int n, int k)
{
	if (M.find(coorblk) == M.end())
	{
		P.insert(make_pair(coorblk, ++cnt));
		K.insert(make_pair(cnt, coorblk));
		V.push_back(vector<int>());
		V[P[prev]].push_back(P[coorblk]);
		if (y == n)
		{
			M.insert(make_pair(coorblk, k));
			return -10;
		}
		else
		{
			M.insert(make_pair(coorblk, 0));
			f(3 - k);
		}
	}
	else
		V[P[prev]].push_back(P[coorblk]);
	return i;
}
void f(int k)
{
	get_coorblk();

	ll prev = coorblk;
	int i, j, tx1 = px1, ty1 = py1, tx2 = px2, ty2 = py2, t1bn = p1bn, t2bn = p2bn, tbk[6][6];
	for (i = 1; i < N; i++)
		for (j = 1; j < N; j++)
			tbk[i][j] = pbk[i][j];

	if (k == 1)
	{
		for (i = 1; i < 5 && i > 0; i++)
			if (i > 0 && can_go(tx1, ty1, i % 4))
			{
				if (i > 0 && tx1 + dx[i % 4] == tx2 && ty1 + dy[i % 4] == ty2)
				{
					if (i > 0 && can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], i % 4))
					{
						make_coorblk(3 - k, tx1 + 2 * dx[i % 4], ty1 + 2 * dy[i % 4], tx2, ty2);
						i = insert(prev, i, ty1 + 2 * dy[i % 4], N, k);
						make_coorblk(k, tx1, ty1, tx2, ty2);
					}
					else if (i > 0)
					{
						if (i > 0 && can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 1) % 4))
						{
							make_coorblk(3 - k, tx1 + dx[i % 4] + dx[(i + 1) % 4], ty1 + dy[i % 4] + dy[(i + 1) % 4], tx2, ty2);
							i = insert(prev, i, ty1 + dy[i % 4] + dy[(i + 1) % 4], N, k);
							make_coorblk(k, tx1, ty1, tx2, ty2);
						}
						if (i > 0 && can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 3) % 4))
						{
							make_coorblk(3 - k, tx1 + dx[i % 4] + dx[(i + 3) % 4], ty1 + dy[i % 4] + dy[(i + 3) % 4], tx2, ty2);
							i = insert(prev, i, ty1 + dy[i % 4] + dy[(i + 3) % 4], N, k);
							make_coorblk(k, tx1, ty1, tx2, ty2);
						}
					}
				}
				else
				{
					if (i > 0)
					{
						make_coorblk(3 - k, tx1 + dx[i % 4], ty1 + dy[i % 4], tx2, ty2);
						i = insert(prev, i, ty1 + dy[i % 4], N, k);
						make_coorblk(k, tx1, ty1, tx2, ty2);
					}
				}
			}
	}
	else
	{
		for (i = 3; i < 7 && i > 0; i++)
			if (i > 0 && can_go(tx2, ty2, i % 4))
			{
				if (i > 0 && tx2 + dx[i % 4] == tx1 && ty2 + dy[i % 4] == ty1)
				{
					if (i > 0 && can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], i % 4))
					{
						make_coorblk(3 - k, tx1, ty1, tx2 + 2 * dx[i % 4], ty2 + 2 * dy[i % 4]);
						i = insert(prev, i, ty2 + 2 * dy[i % 4], 1, k);
						make_coorblk(k, tx1, ty1, tx2, ty2);
					}
					else if (i > 0)
					{
						if (i > 0 && can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 1) % 4))
						{
							make_coorblk(3 - k, tx1, ty1, tx2 + dx[i % 4] + dx[(i + 1) % 4], ty2 + dy[i % 4] + dy[(i + 1) % 4]);
							i = insert(prev, i, ty2 + dy[i % 4] + dy[(i + 1) % 4], 1, k);
							make_coorblk(k, tx1, ty1, tx2, ty2);
						}
						if (i > 0 && can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 3) % 4))
						{
							make_coorblk(3 - k, tx1, ty1, tx2 + dx[i % 4] + dx[(i + 3) % 4], ty2 + dy[i % 4] + dy[(i + 3) % 4]);
							i = insert(prev, i, ty2 + dy[i % 4] + dy[(i + 3) % 4], 1, k);
							make_coorblk(k, tx1, ty1, tx2, ty2);
						}
					}
				}
				else
				{
					if (i > 0)
					{
						make_coorblk(3 - k, tx1, ty1, tx2 + dx[i % 4], ty2 + dy[i % 4]);
						i = insert(prev, i, ty2 + dy[i % 4], 1, k);
						make_coorblk(k, tx1, ty1, tx2, ty2);
					}
				}
			}
	}
	if (i > 0)
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
						p1bn = t1bn; p2bn = t2bn;
						for (int a = 1; a < N; a++)
							for (int b = 1; b < N; b++)
								pbk[a][b] = tbk[a][b];
						make_coorblk(3 - k, tx1, ty1, tx2, ty2);

						t = insert(prev, 0, 0, 1, k);

						tbk[i][j] = 0;
						if (k == 1) t1bn++;
						else t2bn++;
						p1bn = t1bn; p2bn = t2bn;
						for (int a = 1; a < N; a++)
							for (int b = 1; b < N; b++)
								pbk[a][b] = tbk[a][b];
						make_coorblk(k, tx1, ty1, tx2, ty2);
					}
	}
}
void dfs(int idx)
{
	vst[idx] = 1;
	coorblk = K[idx];
	fprintf(out, "%d\n", idx);
	show_coorblk();

	int siz = V[idx].size();
	for (int i = 0; i < siz; i++)
		if (vst[V[idx][i]] == 0)
			dfs(V[idx][i]);
}
int main(void)
{
	in = fopen("input.txt", "r");
	fscanf(in, "%d %d", &N, &B);

	p1bn = p2bn = B;
	make_coorblk(1, N / 2 + 1, 1, N / 2 + 1, N);
	M.insert(make_pair(coorblk, 0));
	P.insert(make_pair(coorblk, ++cnt));
	K.insert(make_pair(cnt, coorblk));
	V.push_back(vector<int>());
	V.push_back(vector<int>());
	f(1);

	printf("\n");
	sprintf(filename, "%dx%d_%d.txt", N, N, B);
	out = fopen(filename, "w");
	fprintf(out, "\n");
	fclose(out);
	out = fopen(filename, "a");
	fprintf(out, "%lld\n", M.size());

	auto iter = M.begin();
	for (; iter != M.end(); ++iter)
	{
		coorblk = iter->first;
//		show_coorblk();
		fprintf(out, "%d %lld\n", P[coorblk], coorblk);
	}
	printf("%lld\n", cnt);
	for (int i = 1; i <= cnt; i++)
	{
		int siz = V[i].size();
		fprintf(out, "%d ", i);
		fprintf(out, "%d ", V[i].size());
		for (int j = 0; j < siz; j++)
			fprintf(out, "%d ", V[i][j]);
		fprintf(out, "\n");
	}
	fclose(out);

	out = fopen("show_dfs.txt", "w");
	dfs(1);
	fclose(out);

	return 0;
}
