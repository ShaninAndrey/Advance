import math


class point:
    __slots__ = ['x', 'y', 'ind']

    # get 2 coordinates, create point with these coordinates
    def __init__(self, x, y, ind=-1):
        self.x = x
        self.y = y
        self.ind = ind

    def __gt__(self, other):
        return self.x > other.x or self.x == other.x and self.y > other.y


class line:
    __slots__ = ['a', 'b', 'c']

    # get 2 points, construct line based on these points
    def __init__(self, p1, p2):
        if not (p1 > p2):
            p1, p2 = p2, p1
        self.a = p2.y - p1.y
        self.b = p1.x - p2.x
        self.c = -self.a * p2.x - self.b * p2.y

    # takes side of point pp with relation of line
    def get_res(self, pp):
        return self.a * pp.x + self.b * pp.y + self.c

    def on_diff_sides(self, p1, p2):
        return self.get_res(p1) * self.get_res(p2) < 0

    def is_parallel(self, other):
        return self.b * other.a == self.a * other.b


class convex_hull:
    __slots__ = ['up', 'down']

    # creates convex hull with gracham algorithm
    # see similar realisation and algorithm there http://e-maxx.ru/algo/convex_hull_graham
    def __init__(self, points):
        self.up = []
        self.down = []
        n = len(points)

        if (n <= 1):
            return
        sorted_points = []
        for point in points:
            sorted_points.append(point)
        sorted_points.sort()
        start_point = sorted_points[0]
        end_point = sorted_points[n - 1]
        self.up.append(start_point)
        self.down.append(start_point)

        for i in range(1, n):
            if (i == n - 1 or line(start_point, sorted_points[i]).get_res(end_point) > 0):
                while (len(self.up) >= 2):
                    if (line(self.up[-2], self.up[-1]).get_res(sorted_points[i]) <= 0):
                        self.up.pop()
                    else:
                        break
                self.up.append(sorted_points[i])
            if (i == n - 1 or line(start_point, sorted_points[i]).get_res(end_point) < 0):
                while (len(self.down) >= 2):
                    if (line(self.down[-2], self.down[-1]).get_res(sorted_points[i]) >= 0):
                        self.down.pop()
                    else:
                        break
                self.down.append(sorted_points[i])
        self.down.reverse()

    # find such points P1 and P2, that line l id exactly between points P1 and P2
    # return idexes of such points, of (0, 0) if such points aren't exists
    # works with O(n * log(n))
    def find_points_on_diff_sides_of_line(self, l):
        # find such point P, that line ln id exactly between points P and pp
        # return index of point P, or 0 if such point isn't exists
        #
        # ------------------------------------ for developers ------------------------------------
        # use bynary search with related of current line based on 2 neighbor points on convex hull and initial point
        def get(side, ln, p):
            if (len(side) <= 2):
                return 0
            l = 1
            r = len(side) - 1
            while (l + 1 < r):
                mid = (l + r) / 2
                if (ln.on_diff_sides(side[mid], p)):
                    return side[mid].ind
                if (ln.on_diff_sides(side[mid + 1], p)):
                    return side[mid + 1].ind
                tmp = line(side[mid], side[mid + 1])
                if (ln.is_parallel(tmp)):
                    return 0
                if (math.fabs(ln.get_res(side[mid])) <
                        math.fabs(ln.get_res(side[mid + 1]))):
                    r = mid
                else:
                    l = mid + 1
            if (ln.on_diff_sides(p, side[ln])):
                return side[ln].ind
            return 0

        if (l.on_diff_sides(self.up[0], self.up[-1])):
            return (self.up[0].ind, self.up[-1].ind)
        if (l.get_res(self.up[0]) > 0):
            return (get(self.up, l, self.up[0]), self.up[0].ind)
        if (l.get_res(self.up[-1]) > 0):
            return (get(self.up, l, self.up[-1]), self.up[-1].ind)
        if (l.get_res(self.up[0]) < 0):
            return (get(self.down, l, self.up[0]), self.up[0].ind)
        if (l.get_res(self.up[-1]) < 0):
            return (get(self.down, l, self.up[-1]), self.up[-1].ind)
        if (len(self.up) == 2 or len(self.down) == 2):
            return (0, 0)
        return (self.up[1].ind, self.down[1].ind)


# checks if points xx and yy are on different sides of line l
# works with O(n ^ 2)
# used only for debugging
def simple_check(points, l, xx, yy):
    if (xx == 0):
        aa = 0
        bb = 0
        for i in range(n):
            if (l.get_res(points[i]) < 0):
                aa = 1
            if (l.get_res(points[i]) > 0):
                bb = 1
        return not (aa and bb)
    return l.on_diff_sides(points[xx - 1], points[yy - 1])


if __name__ == "__main__":
    def read_all(points, lines):
        n = int(raw_input())
        for i in range(n):
            f, g = raw_input().split()
            points.append(point(int(f), int(g), i + 1))

        m = int(raw_input())
        for i in range(m):
            x1, y1, x2, y2 = raw_input().split()
            lines.append(line(point(int(x1), int(y1)), point(int(x2), int(y2))))

    points = []
    lines = []
    read_all(points, lines)
    hull = convex_hull(points)

    for l in lines:
        if (len(points) < 2):
            print 0
            continue
        res = hull.find_points_on_diff_sides_of_line(l)
        if (res[0] == 0):
            print 0
        else:
            print res[0], res[1]
