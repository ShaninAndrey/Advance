

class Point(object):
    """ Represent a single point in 2d space """

    def __init__(self, x_, y_):
        super(Point, self).__init__()
        self.x = x_
        self.y = y_

    """ Formatted printing """

    def __str__(self):
        return '%s %s' % (int(round(self.x)), int(round(self.y)))

    def __repr__(self):
        return '%s %s' % (int(round(self.x)), int(round(self.y)))

    def __cmp__(self, other):
        if self.y < other.y or (self.y == other.y and self.x < other.x):
            return 1
        elif self.y > other.y or (self.y == other.y and self.x > other.x):
            return -1
        else:
            return 0


class Edge(object):  # Edge to be in the final voronoi diagram
    def __init__(self, start, left, right):
        self.start = start
        self.left = left
        self.right = right
        # k and m are expected line direction, y = kx + m
        self.k = 2 ** 31 if left.y == right.y else float(right.x - left.x) / float(left.y - right.y)
        self.m = float(start.y - self.k * start.x)
        self.xdir = float(right.y - left.y)
        self.ydir = float(left.x - right.x)
        self.neighbour = self.end = None

    """ Formatted printing """

    def __str__(self): return '{%s, %s}' % (self.start, self.end)

    def __repr__(self): return '{%s, %s}' % (self.start, self.end)


class Event(Point):
    """ Generic event type. """

    def __init__(self, x_, y_): super(Event, self).__init__(x_, y_)


class SiteEvent(Event):
    """ Site event """

    def __init__(self, x, y): super(SiteEvent, self).__init__(x, y)


class CircleEvent(Event):
    """ Circle event """
    parabola = None

    def __init__(self, x, y): super(CircleEvent, self).__init__(x, y)


class Parabola(object):
    """ A parabola connected to a point used in the beachline """
    left = right = edge = ce = parent = None

    def __init__(self, site=None):
        self.site = site
        self.leaf = site is not None

    def set_left(self, par):
        self.left = par
        par.parent = self

    def set_right(self, par):
        self.right = par
        par.parent = self
