// v is the pointset, w is auxiliary with size at least equal to v's
ll closest_pair (ptl * v, ptl * w, int l, int r, bool sorted = 0) { // nlg | r is exclusive 
	if (l + 1 >= r) return INF;
	if (!sorted) sort(v+l,v+r,[](ptl a, ptl b){ return a.x < b.x; });
	int m = (l+r)/2; ll x = v[m].x;
	ll res = min(closest_pair(v,w,l,m,1),closest_pair(v,w,m,r,1));
	merge(v+l,v+m,v+m,v+r,w+l,[](ptl a, ptl b){ return a.y < b.y; });
	for (int i = l, s = l; i < r; i++) if (sq((v[i] = w[i]).x - x) < res) {
		for (int j = s-1; j >= l && sq(w[i].y - w[j].y) < res; j--)
			res = min(res, w[i].dist_sqr(w[j]));
		w[s++] = v[i];
	}
	return res;
}
