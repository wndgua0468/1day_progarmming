#include <bits/stdc++.h>
#define x first
#define y second
using namespace std;

typedef long long ll;
typedef pair<ll, ll> p;

vector<p> a, b;
vector<p> aHull, bHull;

int ccw(p a, p b, p c){
	ll res = a.x*b.y + b.x*c.y + c.x*a.y;
	res -= b.x*a.y + c.x*b.y + a.x*c.y;
	if (res > 0) return 1;
	if (res)return -1;
	return 0;
}

ll dist(p a, p b){
	ll dx = a.x - b.x;
	ll dy = a.y - b.y;
	return dx*dx + dy*dy;
}

void init(){
	a.clear();
	b.clear();
	aHull.clear();
	bHull.clear();
}

void getHull(vector<p> &v, vector<p> &hull){
	for(auto &i : v) cin >> i.x >> i.y;
	swap(v[0], *min_element(v.begin(), v.end()));
	sort(v.begin() + 1, v.end(), [&](p &a, p &b){
		int cw = ccw(v[0], a, b);
		if (cw) return cw > 0;
		return dist(v[0], a) < dist(v[1], b);
	});
	for (auto i : v){
		while (hull.size() >= 2 && ccw(hull[hull.size() - 2], hull.back(), i) <= 0) hull.pop_back();
		hull.push_back(i);
	}
}

bool chk(p now, const vector<p> &v){
	int cnt = 0;
	pair<double, double> ii, jj;
	pair<double, double> q = {now.x, now.y};
	for(int i=0; i<v.size(); i++){
		int j = (i + 1) % v.size();
		ii = v[i], jj = v[j];
		if((ii.y > q.y) != (jj.y > q.y)){
			double X = (jj.x - ii.x) * (q.y - ii.y) / (jj.y - ii.y) + ii.x;
			if(q.x < X) cnt++;
		}
	}
	return cnt % 2 > 0;
}

bool disjoint(ll a, ll b, ll c, ll d){
	if(a > b) swap(a, b);
	if(c > d) swap(c, d);
	return b < c || d < a;
}

bool crash(p a, p b, p c, p d){
	int ab = ccw(a, b, c) * ccw(a, b, d);
	int cd = ccw(c, d, a) * ccw(c, d, b);
	if(ab == 0 && cd == 0){
		return !disjoint(a.x, b.x, c.x, d.x) && !disjoint(a.y, b.y, c.y, d.y);
	}
	return ab <= 0 && cd <= 0;
}

void solve(){
	init();
	int n, m; cin >> n >> m;
	a.resize(n); b.resize(m);
	getHull(a, aHull); getHull(b, bHull);

	int ans = 0;

	if(chk(aHull[0], bHull) || chk(bHull[0], aHull)) ans = 1;

	for(auto i : aHull) if(chk(i, bHull)) ans = 1;
	for(auto i : bHull) if(chk(i, aHull)) ans = 1;

	n = aHull.size();
	m = bHull.size();
	for(int i=0; i<n; i++){
		for(int j=0; j<m; j++){
			if(crash(aHull[i], aHull[(i+1)%n], bHull[j], bHull[(j+1)%m])) ans = 1;
		}
	}
	if(ans) cout << "NO\n";
	else cout << "YES\n";
}

int main(){
	ios_base::sync_with_stdio(0); cin.tie(0);
	int t; cin >> t;
	while(t--) solve();
}
