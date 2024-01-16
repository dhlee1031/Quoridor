#include <bits/stdc++.h>
#include <unordered_map>
#pragma gcc optimize("O3")

using namespace std;
typedef long long ll;

unordered_map<ll, int> M, P;
unordered_map<int, ll> K;
vector<vector<int> > V;

ll bitmask, cnt, prev;
int N, B;
int p1bn, p2bn, ccnt;
int px1, py1, px2, py2;
int pbk[6][6];
int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
int px[4][2] = { {-1,-1}, {-1,0}, {0,0}, {0,-1} }, py[4][2] = { {-1,0}, {0,0}, {0,-1}, {-1,-1} };
int turn, vst[10000000];
char filename[128];
FILE* in, * out;

void encode_bitmask(int k, int x1, int y1, int x2, int y2)
{
	bitmask = k - 1;
	bitmask <<= 3;	bitmask |= p1bn;
	bitmask <<= 3;	bitmask |= p2bn;
	bitmask <<= 3;	bitmask |= x1;
	bitmask <<= 3;	bitmask |= y1;
	bitmask <<= 3;	bitmask |= x2;
	bitmask <<= 3;	bitmask |= y2;
	bitmask <<= 32;

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			if (pbk[i][j] == 1)
				bitmask |= ((ll)1 << ((i - 1) * (N - 1) + j - 1));
			else if (pbk[i][j] == 2)
				bitmask |= ((ll)1 << ((i - 1) * (N - 1) + j - 1 + (N - 1) * (N - 1)));
}
void decode_bitmask(void)
{
	ll temp = bitmask;

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			pbk[i][j] = 0;
	int m = (N - 1) * (N - 1);
	for (int i = 0; i < m; i++)
		if (((ll)1 << i) & bitmask)
			pbk[i / (N - 1) + 1][i % (N - 1) + 1] = 1;	//세로
	for (int i = m; i < m * 2; i++)
		if (((ll)1 << i) & bitmask)
			pbk[(i - m) / (N - 1) + 1][(i - m) % (N - 1) + 1] = 2;	//가로

	bitmask >>= 32;
	py2 = bitmask & 7;	bitmask >>= 3;
	px2 = bitmask & 7;	bitmask >>= 3;
	py1 = bitmask & 7;	bitmask >>= 3;
	px1 = bitmask & 7;	bitmask >>= 3;
	p2bn = bitmask & 7;	bitmask >>= 3;
	p1bn = bitmask & 7; bitmask >>= 3;
	turn = (bitmask & 1) + 1;

	bitmask = temp;
}
void show_bitmask(void)
{
	decode_bitmask();
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
inline bool range_s(int x)
{
	return 1 <= x && x < N;
}
inline bool range_l(int x)
{
	return 1 <= x && x <= N;
}
bool pbk_empty(int x, int y, int k)
{
	return !(range_s(x) && range_s(y) && pbk[x][y] == k);
}
bool can_go(int x, int y, int d)
{
	int x1 = x + px[d][0], y1 = y + py[d][0], x2 = x + px[d][1], y2 = y + py[d][1];
	return range_l(x + dx[d]) && range_l(y + dy[d]) && pbk_empty(x1, y1, d % 2 + 1) && pbk_empty(x2, y2, d % 2 + 1);
}
bool can_win(int x0, int y0)
{
	int win = 0, vst[6][6] = { 0 };
	queue<int> Q;
	Q.push(x0); Q.push(y0); vst[x0][y0] = 1;
	while (!Q.empty() && !win)
	{
		int xx = Q.front(); Q.pop();
		int yy = Q.front(); Q.pop();
		if (yy == N)
		{
			win = 1;
			continue;
		}
		for (int i = 0; i < 4; i++)
			if (can_go(xx, yy, i) && vst[xx + dx[i]][yy + dy[i]] == 0) {
				Q.push(xx + dx[i]); Q.push(yy + dy[i]); vst[xx + dx[i]][yy + dy[i]] = 1;
			}
	}
	return win;
}
bool blk_ok(int z, int x, int y, int d)
{
	decode_bitmask();

	if (z == 1 && p1bn == 0 || z == 2 && p2bn == 0 || pbk[x][y])
		return false;
	if (!(d == 1 && pbk_empty(x, y - 1, 1) && pbk_empty(x, y + 1, 1)
		|| d == 2 && pbk_empty(x - 1, y, 2) && pbk_empty(x + 1, y, 2)))
		return false;

	pbk[x][y] = d;
	bool ret = can_win(px1, py1) && can_win(px2, py2);
	pbk[x][y] = 0;

	return ret;
}
void f(int);
int insert(ll prev, int i, int y, int n, int k)
{
	if (M.find(bitmask) == M.end())
	{
		P.insert(make_pair(bitmask, ++cnt));
		K.insert(make_pair(cnt, bitmask));
		V.push_back(vector<int>());
		V[P[prev]].push_back(P[bitmask]);
		if (y == n)
		{
			M.insert(make_pair(bitmask, k));
			return -10;
		}
		else
		{
			M.insert(make_pair(bitmask, 0));
			f(3 - k);
		}
	}
	else
		V[P[prev]].push_back(P[bitmask]);
	return i;
}
void f(int k)
{
	decode_bitmask();

	ll prev = bitmask;
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
						encode_bitmask(3 - k, tx1 + 2 * dx[i % 4], ty1 + 2 * dy[i % 4], tx2, ty2);
						i = insert(prev, i, ty1 + 2 * dy[i % 4], N, k);
						encode_bitmask(k, tx1, ty1, tx2, ty2);
					}
					else if (i > 0)
					{
						if (i > 0 && can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 1) % 4))
						{
							encode_bitmask(3 - k, tx1 + dx[i % 4] + dx[(i + 1) % 4], ty1 + dy[i % 4] + dy[(i + 1) % 4], tx2, ty2);
							i = insert(prev, i, ty1 + dy[i % 4] + dy[(i + 1) % 4], N, k);
							encode_bitmask(k, tx1, ty1, tx2, ty2);
						}
						if (i > 0 && can_go(tx1 + dx[i % 4], ty1 + dy[i % 4], (i + 3) % 4))
						{
							encode_bitmask(3 - k, tx1 + dx[i % 4] + dx[(i + 3) % 4], ty1 + dy[i % 4] + dy[(i + 3) % 4], tx2, ty2);
							i = insert(prev, i, ty1 + dy[i % 4] + dy[(i + 3) % 4], N, k);
							encode_bitmask(k, tx1, ty1, tx2, ty2);
						}
					}
				}
				else
				{
					if (i > 0)
					{
						encode_bitmask(3 - k, tx1 + dx[i % 4], ty1 + dy[i % 4], tx2, ty2);
						i = insert(prev, i, ty1 + dy[i % 4], N, k);
						encode_bitmask(k, tx1, ty1, tx2, ty2);
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
						encode_bitmask(3 - k, tx1, ty1, tx2 + 2 * dx[i % 4], ty2 + 2 * dy[i % 4]);
						i = insert(prev, i, ty2 + 2 * dy[i % 4], 1, k);
						encode_bitmask(k, tx1, ty1, tx2, ty2);
					}
					else if (i > 0)
					{
						if (i > 0 && can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 1) % 4))
						{
							encode_bitmask(3 - k, tx1, ty1, tx2 + dx[i % 4] + dx[(i + 1) % 4], ty2 + dy[i % 4] + dy[(i + 1) % 4]);
							i = insert(prev, i, ty2 + dy[i % 4] + dy[(i + 1) % 4], 1, k);
							encode_bitmask(k, tx1, ty1, tx2, ty2);
						}
						if (i > 0 && can_go(tx2 + dx[i % 4], ty2 + dy[i % 4], (i + 3) % 4))
						{
							encode_bitmask(3 - k, tx1, ty1, tx2 + dx[i % 4] + dx[(i + 3) % 4], ty2 + dy[i % 4] + dy[(i + 3) % 4]);
							i = insert(prev, i, ty2 + dy[i % 4] + dy[(i + 3) % 4], 1, k);
							encode_bitmask(k, tx1, ty1, tx2, ty2);
						}
					}
				}
				else
				{
					if (i > 0)
					{
						encode_bitmask(3 - k, tx1, ty1, tx2 + dx[i % 4], ty2 + dy[i % 4]);
						i = insert(prev, i, ty2 + dy[i % 4], 1, k);
						encode_bitmask(k, tx1, ty1, tx2, ty2);
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
						encode_bitmask(3 - k, tx1, ty1, tx2, ty2);

						t = insert(prev, 0, 0, 1, k);

						tbk[i][j] = 0;
						if (k == 1) t1bn++;
						else t2bn++;
						p1bn = t1bn; p2bn = t2bn;
						for (int a = 1; a < N; a++)
							for (int b = 1; b < N; b++)
								pbk[a][b] = tbk[a][b];
						encode_bitmask(k, tx1, ty1, tx2, ty2);
					}
	}
}
void dfs(int idx)
{
	vst[idx] = 1;
	bitmask = K[idx];
	fprintf(out, "%d\n", idx);
	show_bitmask();

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
	encode_bitmask(1, N / 2 + 1, 1, N / 2 + 1, N);
	M.insert(make_pair(bitmask, 0));
	P.insert(make_pair(bitmask, ++cnt));
	K.insert(make_pair(cnt, bitmask));
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
		bitmask = iter->first;
		//		show_coorblk();
		fprintf(out, "%d %lld\n", P[bitmask], bitmask);
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
