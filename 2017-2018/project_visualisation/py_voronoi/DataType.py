import heapq
import itertools

class Point:
    x = 0.0
    y = 0.0

    def __init__(self, x, y):
        self.x = x
        self.y = y
    def __str__(self):
        return "Point(" + "{0:.2f}".format(self.x) + ", y=" + "{0:.2f}".format(self.y) + ")"
    def __repr__(self):
        return str(self)
    def copy(self):
        return Point(self.x, self.y)

class Event:
    x = 0.0
    p = None
    a = None
    valid = True
    
    def __init__(self, x, p, a):
        self.x = x
        self.p = p
        self.a = a
        self.valid = True

    def __str__(self):
        return str(self.p) + "\n" + str(self.a) + "\nis_valid: " + str(self.valid)
    def __repr__(self):
        return str(self)

class Arc:
    p = None
    pprev = None
    pnext = None
    e = None
    s0 = None
    s1 = None
    
    def __init__(self, p, a=None, b=None):
        self.p = p
        self.pprev = a
        self.pnext = b
        self.e = None
        self.s0 = None
        self.s1 = None

    def __str__(self):
        return str(self.s0) + "\n" + str(self.s1)

    def copy(self, a=None):
        arc = Arc(self.p.copy())
        arc.pprev = a
        if self.pnext:
            arc.pnext = self.pnext.copy(arc)
            arc.s1 = arc.pnext.s0
        else:
            if (self.s1):
                arc.s1 = self.s1.copy()
        if (self.s0):
            arc.s0 = self.s0.copy()
        return arc


class Segment:
    start = None
    end = None
    done = False
    
    def __init__(self, p):
        self.start = p
        self.end = None
        self.done = False

    def finish(self, p):
        if self.done: return
        self.end = p
        self.done = True

    def copy(self):
        segm = Segment(self.start.copy())
        if (self.end):
            segm.end = self.end.copy()
        segm.done = self.done
        return segm

    def __str__(self):
        return "Segm(" + str(self.start) + ", " + str(self.end) + ")"

class PriorityQueue:
    def __init__(self):
        self.pq = []
        self.entry_finder = {}
        self.counter = itertools.count()

    def push(self, item):
        # check for duplicate
        if item in self.entry_finder: return
        count = next(self.counter)
        # use x-coordinate as a primary key (heapq in python is min-heap)
        entry = [item.x, count, item]
        self.entry_finder[item] = entry
        heapq.heappush(self.pq, entry)

    def remove_entry(self, item):
        entry = self.entry_finder.pop(item)
        entry[-1] = 'Removed'

    def pop(self):
        while self.pq:
            priority, count, item = heapq.heappop(self.pq)
            if item is not 'Removed':
                del self.entry_finder[item]
                return item
        raise KeyError('pop from an empty priority queue')

    def top(self):
        while self.pq:
            priority, count, item = heapq.heappop(self.pq)
            if item is not 'Removed':
                del self.entry_finder[item]
                self.push(item)
                return item
        raise KeyError('top from an empty priority queue')

    def empty(self):
        return not self.pq
            
    def __str__(self):
        return self.pq.__str__()
        str = "("
        for item in self.pq:
            str = str + str(item) + ", "
        return str[:-2] + ")"

    def __repr__(self):
        return str(self)
