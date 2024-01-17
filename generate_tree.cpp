#include <bits/stdc++.h>
#include <unordered_map>
#pragma gcc optimize("O3")

using namespace std;
typedef long long ll;

unordered_map<ll, vector<ll>> M;
set<ll> vst;
queue<ll> Q;

int N, B;

ll bitmask;
int turn, p1bn, p2bn;
int px1, py1, px2, py2;
int pbk[6][6];

int dx[4] = { -1,0,1,0 }, dy[4] = { 0,1,0,-1 };
int px[4][2] = { {-1,-1}, {-1,0}, {0,0}, {0,-1} }, py[4][2] = { {-1,0}, {0,0}, {0,-1}, {-1,-1} };
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
void f(ll msk)
{
	bitmask = msk;
	decode_bitmask();
	if (vst.find(msk) != vst.end()) return;
	vst.insert(msk);
	
	ll prev = msk;
	M[prev] = vector<ll>();

	if (py1 == N || py2 == 1) return;
	if (turn == 1 && py1 == N - 1 && can_go(px1, py1, 1))
	{
		encode_bitmask(turn, px1, N, px2, py2);
		M[prev].push_back(bitmask);
		return;
	}
	if (turn == 2 && py2 == 2 && can_go(px2, py2, 3))
	{
		encode_bitmask(turn, px1, py1, px2, 1);
		M[prev].push_back(bitmask);
		return;
	}

	if (turn == 1)
	{
		for (int d = 0; d < 4; d++)
			if (can_go(px1, py1, d % 4))
			{
				if (px1 + dx[d % 4] == px2 && py1 + dy[d % 4] == py2)
				{
					if (can_go(px1 + dx[d % 4], py1 + dy[d % 4], d % 4))
					{
						encode_bitmask(3 - turn, px1 + 2 * dx[d % 4], py1 + 2 * dy[d % 4], px2, py2);
						M[prev].push_back(bitmask);
						if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
					}
					else
					{
						if (can_go(px1 + dx[d % 4], py1 + dy[d % 4], (d + 1) % 4))
						{
							encode_bitmask(3 - turn, px1 + dx[d % 4] + dx[(d + 1) % 4], py1 + dy[d % 4] + dy[(d + 1) % 4], px2, py2);
							M[prev].push_back(bitmask);
							if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
						}
						if (can_go(px1 + dx[d % 4], py1 + dy[d % 4], (d + 3) % 4))
						{
							encode_bitmask(3 - turn, px1 + dx[d % 4] + dx[(d + 3) % 4], py1 + dy[d % 4] + dy[(d + 3) % 4], px2, py2);
							M[prev].push_back(bitmask);
							if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
						}
					}
				}
				else
				{
					encode_bitmask(3 - turn, px1 + dx[d % 4], py1 + dy[d % 4], px2, py2);
					M[prev].push_back(bitmask);
					if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
				}
			}
	}
	else
	{
		for (int d = 0; d < 4; d++)
			if (can_go(px2, py2, d % 4))
			{
				if (px2 + dx[d % 4] == px1 && py2 + dy[d % 4] == py1)
				{
					if (can_go(px2 + dx[d % 4], py2 + dy[d % 4], d % 4))
					{
						encode_bitmask(3 - turn, px1, py1, px2 + 2 * dx[d % 4], py2 + 2 * dy[d % 4]);
						M[prev].push_back(bitmask);
						if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
					}
					else
					{
						if (can_go(px2 + dx[d % 4], py2 + dy[d % 4], (d + 1) % 4))
						{
							encode_bitmask(3 - turn, px1, py1, px2 + dx[d % 4] + dx[(d + 1) % 4], py2 + dy[d % 4] + dy[(d + 1) % 4]);
							M[prev].push_back(bitmask);
							if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
						}
						if (can_go(px2 + dx[d % 4], py2 + dy[d % 4], (d + 3) % 4))
						{
							encode_bitmask(3 - turn, px1, py1, px2 + dx[d % 4] + dx[(d + 3) % 4], py2 + dy[d % 4] + dy[(d + 3) % 4]);
							M[prev].push_back(bitmask);
							if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
						}
					}
				}
				else
				{
					encode_bitmask(3 - turn, px1, py1, px2 + dx[d % 4], py2 + dy[d % 4]);
					M[prev].push_back(bitmask);
					if (vst.find(bitmask) == vst.end()) Q.push(bitmask);
				}
			}
	}

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			for (int l = 1; l <= 2; l++)
				if (blk_ok(turn, i, j, l))
				{
					pbk[i][j] = l;
					turn == 1 ? p1bn-- : p2bn--;
					
					encode_bitmask(3 - turn, px1, py1, px2, py2);
					M[prev].push_back(bitmask);
					if (vst.find(bitmask) == vst.end()) Q.push(bitmask);

					pbk[i][j] = 0;
					turn == 1 ? p1bn++ : p2bn++;
				}
}
int main(void)
{
	in = fopen("input.txt", "r");
	fscanf(in, "%d %d", &N, &B);
	fclose(in);

	p1bn = p2bn = B;
	encode_bitmask(1, N / 2 + 1, 1, N / 2 + 1, N);
	Q.push(bitmask);

	while(!Q.empty()) f(Q.front()), Q.pop();

	sprintf(filename, "%dx%d_%d.txt", N, N, B);
	out = fopen(filename, "w");
	fprintf(out, "%d\n", M.size());

	for (auto mi : M)
	{
		fprintf(out, "%lld : %d\n", mi.first, mi.second.size());
		if (mi.second.size())
		{
			for (ll vi : mi.second) fprintf(out, "%lld ", vi);
			fprintf(out, "\n");
		}
	}

	fclose(out);

	printf("%d %d : %d\n", N, B, vst.size());

	return 0;
}
