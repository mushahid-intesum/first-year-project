#include <bits/stdc++.h>
using namespace std;

struct  yuhu
{
	char n[5];
	int score ;
};
int main(int argc, char const *argv[])
{

	yuhu a,b;
	scanf("%s",a.n);
	a.score = 10;
	b=a;
	printf("%s %d\n",b.n,b.score );
	return 0;
}