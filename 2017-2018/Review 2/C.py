import math
import sys
from random import randint


def line(p1, p2):
    if not (p1 > p2):
        p1, p2 = p2, p1
    a = p2[1] - p1[1]
    b = p1[0] - p2[0]
    return (a, b, -a * p2[0] - b * p2[1])


def get_res(l, pp):
    return l[0] * pp[0] + l[1] * pp[1] + l[2]

a = []
up = []
down = []


def convex_hull():
    if (n <= 1):
        return
    b = []
    for aa in a:
        b.append(aa)
    b.sort()
    p1 = b[0]
    p2 = b[n - 1]
    up.append(p1)
    down.append(p1)
    for i in range(1, n):
        if (i == n - 1 or get_res(line(p1, b[i]), p2) > 0):
            while (len(up) >= 2):
                if (get_res(line(up[-2], up[-1]), b[i]) <= 0):
                    up.pop()
                else:
                    break
            up.append(b[i])
        if (i == n - 1 or get_res(line(p1, b[i]), p2) < 0):
            while (len(down) >= 2):
                if (get_res(line(down[-2], down[-1]), b[i]) >= 0):
                    down.pop()
                else:
                    break
            down.append(b[i])
    down.reverse()


def chk(l, p1, p2):
    return get_res(l, p1) * get_res(l, p2) < 0


def get(vv, ln, pp):
    if (len(vv) <= 2):
        return 0
    l = 1
    r = len(vv) - 1
    while (l + 1 < r):
        mid = (l + r) / 2
        if (chk(ln, vv[mid], pp)):
            return vv[mid][2]
        if (chk(ln, vv[mid + 1], pp)):
            return vv[mid + 1][2]
        tmp = line(vv[mid], vv[mid + 1])
        if (tmp[1] * ln[0] == tmp[0] * ln[1]):
            return 0
        if (math.fabs(get_res(ln, vv[mid])) <
                math.fabs(get_res(ln, vv[mid + 1]))):
            r = mid
        else:
            l = mid + 1
    if (chk(ln, pp, vv[l])):
        return vv[l][2]
    return 0


def check(l):
    if (chk(l, up[0], up[-1])):
        return (up[0][2], up[-1][2])
    if (get_res(l, up[0]) > 0):
        return (get(up, l, up[0]), up[0][2])
    if (get_res(l, up[-1]) > 0):
        return (get(up, l, up[-1]), up[-1][2])
    if (get_res(l, up[0]) < 0):
        return (get(down, l, up[0]), up[0][2])
    if (get_res(l, up[-1]) < 0):
        return (get(down, l, up[-1]), up[-1][2])
    if (len(up) == 2 or len(down) == 2):
        return (0, 0)
    return (up[1][2], down[1][2])


def simple_check(l, xx, yy):
    if (xx == 0):
        aa = 0
        bb = 0
        for i in range(n):
            if (get_res(l, a[i]) < 0):
                aa = 1
            if (get_res(l, a[i]) > 0):
                bb = 1
        return not (aa and bb)
    return chk(l, a[xx - 1], a[yy - 1])

n = int(raw_input())
for i in range(n):
    f, g = raw_input().split()
    # f = randint(-1000000000, 1000000000)
    # g = randint(-1000000000, 1000000000)
    a.append((int(f), int(g), i + 1))

convex_hull()

m = int(raw_input())

for i in range(m):
    x1, y1, x2, y2 = raw_input().split()
    # x1 = randint(-1000000000, 1000000000)
    # x2 = randint(-1000000000, 1000000000)
    # y1 = randint(-1000000000, 1000000000)
    # y2 = randint(-1000000000, 1000000000)
    l = line((int(x1), int(y1)), (int(x2), int(y2)))
    if (n < 2):
        print 0
        continue
    res = check(l)
    if (res[0] == 0):
        print 0
    else:
        print res[0], res[1]

    # if not(simple_check(l, res[0], res[1])):
    #     print("No No No")
    #     sys.exit(0)
