function draw_treap() {
	get_e("cnvup").innerHTML = '';
	get_e("cnvdown").innerHTML = '';
	get_e("cnv").innerHTML = '';
	let cur = visualisation[cur_tic];
	if (!cur.first && !cur.second && !cur.up && !cur.arr) {
		get_e("cnv").style.height = "0px";
		vis(get_e("cnv"));
		hid(get_e("cnvup"));
		hid(get_e("cnvdown"));
		draw(cur);
		get_e("cnv").style.height = (100 + parseInt(getComputedStyle(get_e(cur.chart.container.substr(1)).getElementsByTagName("svg")[0]).height)) + "px";
		return;
	}
	hid(get_e("cnv"));
	vis(get_e("cnvup"));
	vis(get_e("cnvdown"));
	let downh = 0, uph = 0;
	let func = function(cur) {
		return parseInt(getComputedStyle(get_e(cur.chart.container.substr(1)).getElementsByTagName("svg")[0]).height);
	}
	get_e("cnvdown").style.height = "0px";
	get_e("cnvup").style.height = "0px";
	if (cur.first) {draw(cur.first); uph = Math.max(uph, func(cur.first)); }
	if (cur.second) {draw(cur.second); downh = Math.max(downh, func(cur.second)); }
	if (cur.up) {draw(cur.up); uph = Math.max(uph, func(cur.up)); }
	if (cur.arr) {
		let father = get_e("cnvdown");
		let width = parseInt(parseInt(getComputedStyle(father).width) / cur.arr.length);
		for (let i = 0; i < cur.arr.length; ++i) {
			let div = document.createElement("div");
			div.className = "chart";
			div.style.width = width + "px";
			div.style.position = "absolute";
			div.id = "cnvdown" + i;
			div.style.left = (width * i) + "px";
			get_e("cnvdown").appendChild(div);
			draw(cur.arr[i]);
			downh = Math.max(downh, func(cur.arr[i]));
		}
	}
	uph += 100;
	downh += 100;
	get_e("charts").style.height = (uph + downh) + "px";
	get_e("cnvup").style.height = uph + "px";
	get_e("cnvdown").style.height = downh + "px";
}
function draw(cur) {
	if (!cur) return;
	new Treant(cur);
	clear_edges(cur);
	for(let i = 0; i < split_colors.length; ++i) {
		$(".color" + i).css('backgroundColor', split_colors[i]);
	}
	for (let i = 0; i < versions.length; ++i) {
		$(".verscolor" + i).css('backgroundColor', vers_colors[i]);
	}
	$('.node').mouseover(function() {
		let id = $(this).attr('id');
		if (nodes[id]) this.setAttribute("title", "Size: " + nodes[id].sz + "\nPrior: " + nodes[id].prior);
		let x = split_string(this.style.backgroundColor);
		this.style.border = "4px solid #" + (x[0] - 60).toString(16) + (x[1] - 60).toString(16) + (x[2] - 60).toString(16);
		this.style.lineHeight = "46px";
	});
	$('.node').mouseout(function(){
		this.style.lineHeight = "50px";
		this.style.border = "1px solid #000";
	});
}

function create_config(roots, name) {
	this.chart = {
		container: "#" + name, 
		hideRootNode: roots.length > 1, 
		levelSeparation: 20, 
        scrollbar: "fancy", 
		siblingSeparation: 30, 
		subTeeSeparation: 30, 
		hideRootNode: true, 
		connectors: {
            type: "straight",
            style: {
                "stroke-width": 2,
                "stroke-linecap": "round",
                "stroke": "#CCCCCC"
            }
        }
	};
	this.nodeStructure = {children: roots};
}

function treantify(cur) {
	if (!cur) return;
    let r = {text: {name: cur.val == "?" ? cur.val : (cur.val + " : " + cur.prior)}, HTMLclass: cur.color, HTMLid: cur.node, children: [] };
	if (cur.left) r.children.push(treantify(cur.left)); else r.children.push({HTMLclass: "zeronode", children: []});
	if (cur.right) r.children.push(treantify(cur.right)); else r.children.push({HTMLclass: "zeronode", children: []});
    return r;
}
function treantify_full(cur) {
	if (!cur) return;
	let r = {text: {name: cur.val + " : " + cur.prior}, HTMLclass: "verscolor" + cur.vers, HTMLid: cur.node, children: [] };
	if (cur.left) r.children.push(treantify_full(cur.left)); else r.children.push({HTMLclass: "zeronode", children: []});
	if (cur.right) r.children.push(treantify_full(cur.right)); else r.children.push({HTMLclass: "zeronode", children: []});
    return r;
}

function get_treap(ths) {
	let tp = [];
	for (let i = 0; i < ths.children.length; ++i) {
		tp.push(get_t(ths.children[i], 0));
	}
	return tp;
}
function get_t(cur) {
	if (cur.HTMLclass == "zeronode") return null;
	return new Treap({val: 0, color: "color1", prior: "123"}, get_t(cur.children[0]), get_t(cur.children[1]));
}

function clear_edges(cur) {
	let name = cur.chart.container;
	name = name.substr(1);
	let tp = get_treap(cur.nodeStructure);
	let del_edges = [], del_nodes = [], sz = 0, sz1 = 0;
	for (let i = 0; i < tp.length; ++i) {
		del_edges = del_edges.concat(get_edges(tp[i], sz));
		del_nodes = del_nodes.concat(get_edges(tp[i], sz1));
		sz += tp[i].sz * 2;
		sz1 += tp[i].sz * 2 + 1;
	}
	let edges = get_e(name).getElementsByTagName('svg')[0].getElementsByTagName('path');
	let nodes = get_e(name).getElementsByTagName('div');
	for (let i = 0; i < del_edges.length; ++i) edges[del_edges[i] - 1].style.display = "none";
	for (let i = 0; i < del_nodes.length; ++i) {
		nodes[nodes.length - del_nodes[i] - 1].className = nodes[nodes.length - del_nodes[i] - 1].className.replace("node", "");
		nodes[nodes.length - del_nodes[i] - 1].display = "none";
	}
}
function get_edges(cur, sz) {
	if (!cur.left && !cur.right) return [sz + 1, sz + 2];
	if (cur.left && cur.right) {
		return get_edges(cur.left, sz + 1).concat(get_edges(cur.right, sz + cur.left.sz * 2 + 2));
	}
	if (cur.right) {
		return [sz + 1].concat(get_edges(cur.right, sz + 2));
	}
	if (cur.left) return get_edges(cur.left, sz + 1).concat(sz + cur.left.sz * 2 + 2);
}
