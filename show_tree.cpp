#include <bits/stdc++.h>
#include <unordered_map>
#pragma gcc optimize("O3")

using namespace std;
typedef long long ll;

unordered_map<ll, vector<ll>> M;

int N, B;

ll bitmask;
int turn, p1bn, p2bn;
int px1, py1, px2, py2;
int pbk[6][6];

char treefile[128], filename[128];

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
int main(void)
{
	in = fopen("input.txt", "r");
	fscanf(in, "%d %d", &N, &B);
	fclose(in);

	sprintf(treefile, "%dx%dx_%d.txt", N, B);
	in = fopen(treefile, "r");

	ll msiz, vsiz, msk;
	fscanf(in, "%lld", msiz);
	for (int i = 0; i < msiz; i++)
	{
		fscanf(in, "%lld : %lld", msk, vsiz);
		M[msk] = vector<ll>();
		for (int j = 0; j < vsiz; j++)
			fscanf(in, "%lld", bitmask), M[msk].push_back(bitmask);
	}
	fclose(in);

	sprintf(filename, "%dx%d_%d_show.txt", N, N, B);
	out = fopen(filename, "w");

	p1bn = p2bn = B;
	encode_bitmask(1, N / 2 + 1, 1, N / 2 + 1, N);

	fprintf(out, "%d\n", M.size());
	fclose(out);

	return 0;
}
