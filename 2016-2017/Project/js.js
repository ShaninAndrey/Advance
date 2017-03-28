var visualisation = [], versions = [], ptr = [], nodes = [];
var split_colors = ["#f7c760", "#f7f260", "#60f7b5", "#60e5f7", "#ffffff"], vers_colors = [];
var split_colors1 = ["#bb8b24", "#bbb624", "#24bb79", "#24a9bb", "#c3c3c3"], vers_colors1 = [];
var cur_tic, speed = 500;

function start() {
	change_vis(get_e("header"));
	change_vis(get_e("add_array"));
	change_vis(get_e("charts"));
	cur_tic = 0;
	draw_treap();
	check_write();
}

function get_next_int(str, st) {
	for (; st < str.length && str[st] >= '0' && str[st] <= '9'; ++st) {}
	for (; st < str.length && (str[st] < '0' || str[st] > '9'); ++st) {}
	return st;
}
function split_string(str) {
	let st = 0;
	let res = [];
	if (str.length == 0) return null;
	if (str[0] < '0' || str[0] > '9') st = get_next_int(str, st);
	while (st < str.length) {
		res.push(parseInt(str.substr(st)));
		st = get_next_int(str, st);
	}
	return res;
}
function set_array() {
	let str = get_e("write_array").value;
	let array = split_string(str);
	let t = [];
    for (let i = 0; i < array.length; ++i) {
        t.push(new Treap_by_val(array[i]));
    }
	push_vers(not_vis_and_merge(t));
	start();
}
function push_vers(cur) {
	versions.push(cur);
	var r=Math.floor(Math.random() * (90)) + 120;
	var g=Math.floor(Math.random() * (90)) + 120;
	var b=Math.floor(Math.random() * (90)) + 120;
	vers_colors.push('#' + r.toString(16) + g.toString(16) + b.toString(16));
	vers_colors1.push("#" + (r - 60).toString(16) + (g - 60).toString(16) + (b - 60).toString(16));
	ptr.push(visualisation.length);
	select_add();
	visualisation.push(new create_config([treantify_full(cur)], "cnv"));
}

function start_move() {
	let cur = versions[parseInt(get_e("versions").value)];
	let st = parseInt(get_e("start_position").value);
	let fn = parseInt(get_e("finish_position").value);
	let len = parseInt(get_e("length").value);
	if (st + len > cur.sz || fn + len > cur.sz) return;
	move(cur, st, fn, len);
	visual();
}
function visual() {
	if (get_e("auto").checked) {
		if (cur_tic == visualisation.length - 1) return;
		cur_tic++;
		on_write_dis();
		setTimeout(function ths() {
			if (!get_e("auto").checked) return;
			draw_treap();
			if (cur_tic != visualisation.length - 1) {
				cur_tic++;
				setTimeout(ths, speed);
			} else {
				check_write();
			}
		}, speed);
		return;
	}
	if (get_e("skip").checked) {
		cur_tic = visualisation.length - 1;
		draw_treap();
		check_write();
	}
}
function review_click() {
	if (get_e("auto").checked || get_e("skip").checked) {
		if (get_e("skip").checked) on_dis(get_e("versions")); else off_dis(get_e("versions"));
		on_steps_dis();
		visual();
		return;
	}
	off_dis(get_e("versions"));
	check_write();
	off_steps_dis();
}
function check_write() {
	let x = ptr.indexOf(cur_tic);
	if (x >= 0) {
		off_write_dis();
		$(".selectpicker").selectpicker('val', x);
	} else on_write_dis();
}

//-----------------------------------------------   helper   ------------------------------------------

function get_e(str) {
	return document.getElementById(str);
}
function create(str) {
	return document.createElement(str);
}

function hid(cur) {
	cur.style.visibility = "hidden";
}
function vis(cur) {
	cur.style.visibility = "visible";
}
function change_vis(cur) {
	if (cur.style.visibility == "hidden") {
		cur.style.visibility = "visible";
	} else {
		cur.style.visibility = "hidden";
	}
}
function on_dis(cur) {
	cur.disabled = 1;
}
function off_dis(cur) {
	cur.disabled = 0;
}
function on_steps_dis() {
	on_dis(get_e("prev_tic"));
	on_dis(get_e("next_tic"));
}
function off_steps_dis() {
	off_dis(get_e("prev_tic"));
	off_dis(get_e("next_tic"));
}
function on_write_dis() {
	on_dis(get_e("versions"));
	on_dis(get_e("start_position"));
	on_dis(get_e("finish_position"));
	on_dis(get_e("length"));
	on_dis(get_e("move"));
}
function off_write_dis() {
	off_dis(get_e("versions"));
	off_dis(get_e("start_position"));
	off_dis(get_e("finish_position"));
	off_dis(get_e("length"));
	off_dis(get_e("move"));
}

function select_add() {
	var slct = get_e("versions");
	var opt = create("option");
	opt.value = versions.length - 1;
	opt.setAttribute("data-content", (versions.length - 1) + "<span></span>");
	opt.style.backgroundColor = vers_colors[versions.length - 1];
	slct.add(opt);
	$('#versions').selectpicker('refresh');
}
function update_treap() {
	let vers = parseInt(get_e("versions").value);
	cur_tic = ptr[vers];
	draw_treap();
}
function prev_tic() {
	if (cur_tic == 0) return;
	cur_tic--;
	draw_treap();
	check_write();
}
function next_tic() {
	if (cur_tic == visualisation.length - 1) return;
	cur_tic++;
	draw_treap();
	check_write();
}

function upper_bound(arr, val) {
	let ans = 0;
	while (ans < arr.length && arr[ans] <= val) ans++;
	return ans;
}
function swapInt(a, b) {
	a ^= b;
	b ^= a;
	a ^= b;
}
function last(a) {
	return a[a.length - 1];
}