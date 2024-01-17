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
void show_bitmask(ll msk)
{
	bitmask = msk;
	decode_bitmask();
	fprintf(out, "raw: %lld\n", msk);
	fprintf(out, "turn: %d\n", turn);
	fprintf(out, "b1: %d, b2: %d\n", p1bn, p2bn);

	char a[20][20] = { 0 };

	for (int i = 0; i < 2 * N - 1; i++)
		for (int j = 0; j < 2 * N - 1; j++)
			a[i][j] = i % 2 || j % 2 ? ' ' : '0';
	a[px1 - 1 << 1][py1 - 1 << 1] = '1', a[px2 - 1 << 1][py2 - 1 << 1] = '2';

	for (int i = 1; i < N; i++)
		for (int j = 1; j < N; j++)
			if (pbk[i][j] == 1)
				a[i * 2 - 1][j * 2 - 2] = a[i * 2 - 1][j * 2 - 1] = a[i * 2 - 1][j * 2] = '|';
			else if (pbk[i][j] == 2)
				a[i * 2 - 2][j * 2 - 1] = a[i * 2 - 1][j * 2 - 1] = a[i * 2][j * 2 - 1] = '-';

	for (int i = 0; i < 2 * N - 1; i++)
	{
		for (int j = 0; j < 2 * N - 1; j++)
			fprintf(out, "%c", a[j][2 * N - 2 - i]);
		fprintf(out, "\n");
	}
	fprintf(out, "\n");
}
int main(void)
{
	in = fopen("input.txt", "r");
	fscanf(in, "%d %d", &N, &B);
	fclose(in);

	sprintf(treefile, "%dx%d_%d.txt", N, N, B);
	in = fopen(treefile, "r");

	ll msiz = 0, vsiz = 0, msk = 0;
	fscanf(in, "%lld", &msiz);
	for (int i = 0; i < msiz; i++)
	{
		fscanf(in, "%lld : %lld", &msk, &vsiz);
		M[msk] = vector<ll>();
		for (int j = 0; j < vsiz; j++)
			fscanf(in, "%lld", &bitmask), M[msk].push_back(bitmask);
	}
	fclose(in);

	sprintf(filename, "%dx%d_%d_show.txt", N, N, B);
	out = fopen(filename, "w");
	fprintf(out, "tot siz : %d\n", M.size());

	p1bn = p2bn = B;
	encode_bitmask(1, N / 2 + 1, 1, N / 2 + 1, N);

	fprintf(out, "======================================\n");
	show_bitmask(bitmask);
	fprintf(out, "--------------------------------------\n");
	for (ll vi : M[bitmask]) show_bitmask(vi);
	fprintf(out, "======================================\n");

	fclose(out);

	return 0;
}
