var split_treaps1 = [], split_treaps2 = [], queue = [];

function Pair(f, s) {
	this.first = f;
	this.second = s;
}

function get_sz(cur) {
	if (cur == null) return 0;
	return cur.sz;
}
function update(cur) {
    if (cur == null) return;
    cur.sz = get_sz(cur.left) + get_sz(cur.right) + 1;
}
function update_full(cur) {
	if (cur.left) update_full(cur.left);
	if (cur.right) update_full(cur.right);
	update(cur);
}
function Treap_problem() {
	this.val = "?";
	this.sz = 0;
	this.color = "color4";
	this.left = null;
	this.right = null;
}
function Treap_by_val(val) {
	this.val = val;
	this.sz = 1;
	this.vers = versions.length;
	this.color = "color0";
	this.key = 0;
	this.prior = Math.round(Math.random() * Number.MAX_VALUE) % 50;
	this.left = null;
	this.right = null;
	this.node = nodes.length;
	nodes.push(this);
}
function Treap(cur, l, r) {
	this.val = cur.val;
	this.prior = cur.prior;
	this.left = l;
	this.vers = versions.length;
	this.color = cur.color;
	this.right = r;
	update(this);
	this.key = 0;
	this.node = nodes.length;
	nodes.push(this);
}
function get(cur, pos) {
	if (cur.left == null) {
		if (pos == 1) return cur.val;
		return get(cur.right, pos - 1);
	}
	if (pos <= cur.left.sz) return get(cur.left, pos);
	if (pos == cur.left.sz + 1) return cur.val;
	return get(cur.right, pos - cur.left.sz - 1);
}
function write(cur) {
	if (cur == null) return [];
	let res = write(cur.left), res1 = write(cur.right);
	return res.concat(cur.val, res1);
}
function update_keys(cur, val) {
	if (cur == null) return 0;
	cur.key = get_sz(cur.left) + 1 + val;
	return Math.max(update_keys(cur.left, val), update_keys(cur.right, cur.key)) + 1;
}
function fill_treap(cur, keys, sz) {
	if (!cur) return;
	if (cur.left) fill_treap(cur.left, keys, sz);
	if (typeof(keys) != "number") {
		let g = upper_bound(keys, get_sz(cur.left) + sz);
		cur.color = "color" + g;
	} else cur.color = "color" + keys;
	if (cur.right) fill_treap(cur.right, keys, sz + get_sz(cur.left) + 1);
}

function set(t, s) {
    if (t.second == -1) t.first.left = s; else
    if (t.second == 1) t.first.right = s; else
        t.first = s;
    update(t.first);
}

function vis_and_split(cur, szs, res) {
	reses = res;
	split_treaps1.splice(0);
	split_treaps2.splice(0);
	queue = [{cur: cur, szs: szs, res: res, height: 0}];
	fill_treap(cur, szs, 0);
	while (queue.length) {
		height = queue[0].height;
		split();
		split_treaps1[height + 1] = [];
		for (let i = 0; i < res.length; ++i) 
		if (reses[i] && res[i].first) {split_treaps1[height + 1].push(treantify(res[i].first));}
	}
	visualisation.push(new create_config(split_treaps2[0], "cnv"));
	for (let i = 1; i < split_treaps2.length; ++i) {
		visualisation.push({second: new create_config(split_treaps1[i], "cnvdown"), first: new create_config(split_treaps2[i], "cnvup")});
	}
	visualisation.push({second: new create_config(last(split_treaps1), "cnvdown")});
}
function vis_and_merge(t) {
	split_treaps1.splice(0);
	split_treaps2.splice(0);
	let res = new Pair(null, 0);
	let t1 = [];
	for (let i = 0; i < t.length; ++i) if (t[i]) t1.push(treantify(t[i]));
	visualisation.push({second: new create_config(t1, "cnvdown")});
	queue = [{t: t, height: 0, res: res}];
	while (queue.length) {
		let height = queue[0].height;
		merge();
		split_treaps2[height] = treantify(res.first);
		update_full(res.first);
	}
	for (let i = 0; i < split_treaps1.length; ++i) {
		let arr = [];
		for (let q = 0; q < split_treaps1[i].length; ++q)
			arr.push(new create_config(split_treaps1[i][q], "cnvdown" + q));
		visualisation.push({up: new create_config([split_treaps2[i]], "cnvup"), arr: arr});
	}
	visualisation.push(new create_config([treantify(res.first)], "cnv"));
	return res.first;
}
function not_vis_and_merge(t) {
	let res = new Pair(null, 0);
	queue = [{t: t, height: 0, res: res}];
	while (queue.length) {merge(); update_full(res.first);}
	return res.first;
}

function merge() {
	let t = queue[0].t;
	let height = queue[0].height;
	let res = queue[0].res;
	queue.shift();
    if (t.length == 0) {set(res, null); return;}
    if (t.length == 1) {set(res, t[0]); return;}
    let ind = -1, mx = -1011111111;
    for (let i = 0; i < t.length; ++i) {
        if (t[i] && mx < t[i].prior) {
            mx = t[i].prior;
            ind = i;
        }
    }
	if (ind == -1) {set(res, null); return;}
    let l = [], r = [];
    for (let i = 0; i < ind; ++i) l.push(t[i]);
    if (t[ind].left != null) l.push(t[ind].left);
    if (t[ind].right != null) r.push(t[ind].right);
    for (let i = ind + 1; i < t.length; ++i) r.push(t[i]);
	let cur = new Treap(t[ind], new Treap_problem(), new Treap_problem());
	set(res, cur);
	if (!split_treaps1[height]) split_treaps1[height] = [];
	let lft = [], rgt = [];
	for (let i = 0; i < l.length; ++i) {
		if (l[i]) lft.push(treantify(l[i]));
	}
	for (let i = 0; i < r.length; ++i) {
		if (r[i]) rgt.push(treantify(r[i]));
	}
	split_treaps1[height].push(lft);
	split_treaps1[height].push(rgt);
	queue.push({t: l, height: height + 1, res: new Pair(cur, -1)});
	queue.push({t: r, height: height + 1, res: new Pair(cur, 1)});
}
function split() {
	let cur = queue[0].cur, szs = queue[0].szs, res = queue[0].res, height = queue[0].height;
	queue.shift();
    if (cur == null) {
        for (let i = 0; i < res.length; ++i) {
            set(res[i], null);
        }
        return;
    }
	if (!split_treaps2[height]) split_treaps2[height] = [];
	split_treaps2[height] = split_treaps2[height].concat(treantify(cur));
    if (res.length == 1 || szs[0] >= cur.sz) {
        set(res[0], cur);
        return;
    }
    let l = [], r = [], szs_l = [], szs_r = [];
    let i = 0;
    while (i < szs.length && szs[i] <= get_sz(cur.left)) {
        szs_l.push(szs[i]);
        l.push(res[i]);
        i++;
    }
    let ths = new Treap(cur, null, null);
    let ind = i;
    l.push(new Pair(ths, -1));
    r.push(new Pair(ths, 1));
    for (; i < szs.length; ++i) {
        szs_r.push(szs[i] - get_sz(cur.left) - 1);
        r.push(res[i + 1]);
    }
	set(res[ind], ths);
	queue.push({cur: cur.left, szs: szs_l, res: l, height: height + 1});
	queue.push({cur: cur.right, szs: szs_r, res: r, height: height + 1});
}
function move(cur, st, fn, len) {
    if (st == fn) {
		push_vers(cur);
		return;
	}
    let spl;
    if (fn > st) spl = fn + len - 1; else spl = fn - 1;
    let t = [], t1 = [];
    for (let i = 0; i < 4; ++i)
		t.push(new Pair(null, 0));
    let szs = [st - 1, st + len - 1, spl];
	szs.sort((a, b) => a - b);
    vis_and_split(cur, szs, t);
	let cnt = 0;
    for (let i = 0; i < t.length; ++i) {
		t1.push(t[i].first);
		if (t[i].first != null) cnt++;
	}
	let tmp = t1[1];
	t1[1] = t1[2];
	t1[2] = tmp;
    push_vers(vis_and_merge(t1));
}
