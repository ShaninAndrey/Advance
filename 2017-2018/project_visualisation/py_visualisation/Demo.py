import Tkinter as tk
import tkFileDialog
import sys, time, math
from Voronoi import computeVoronoiDiagram, Site
from random import randint, shuffle

class Const:
    random_colors = [
        [0.267004, 0.004874, 0.329415],
        [0.269944, 0.014625, 0.341379],
        [0.273809, 0.031497, 0.358853],
        [0.276022, 0.044167, 0.370164],
        [0.278791, 0.062145, 0.386592],
        [0.280267, 0.073417, 0.397163],
        [0.281924, 0.089666, 0.412415],
        [0.28291, 0.105393, 0.426902],
        [0.283197, 0.11568, 0.436115],
        [0.283072, 0.130895, 0.449241],
        [0.282623, 0.140926, 0.457517],
        [0.281412, 0.155834, 0.469201],
        [0.279574, 0.170599, 0.479997],
        [0.278012, 0.180367, 0.486697],
        [0.275191, 0.194905, 0.496005],
        [0.273006, 0.20452, 0.501721],
        [0.269308, 0.218818, 0.509577],
        [0.26658, 0.228262, 0.514349],
        [0.262138, 0.242286, 0.520837],
        [0.257322, 0.25613, 0.526563],
        [0.253935, 0.265254, 0.529983],
        [0.248629, 0.278775, 0.534556],
        [0.244972, 0.287675, 0.53726],
        [0.239346, 0.300855, 0.540844],
        [0.233603, 0.313828, 0.543914],
        [0.229739, 0.322361, 0.545706],
        [0.223925, 0.334994, 0.548053],
        [0.220057, 0.343307, 0.549413],
        [0.214298, 0.355619, 0.551184],
        [0.210503, 0.363727, 0.552206],
        [0.204903, 0.375746, 0.553533],
        [0.19943, 0.387607, 0.554642],
        [0.19586, 0.395433, 0.555276],
        [0.190631, 0.407061, 0.556089],
        [0.187231, 0.414746, 0.556547],
        [0.182256, 0.426184, 0.55712],
        [0.177423, 0.437527, 0.557565],
        [0.174274, 0.445044, 0.557792],
        [0.169646, 0.456262, 0.55803],
        [0.166617, 0.463708, 0.558119],
        [0.162142, 0.474838, 0.55814],
        [0.157729, 0.485932, 0.558013],
        [0.154815, 0.493313, 0.55784],
        [0.150476, 0.504369, 0.55743],
        [0.147607, 0.511733, 0.557049],
        [0.143343, 0.522773, 0.556295],
        [0.140536, 0.530132, 0.555659],
        [0.136408, 0.541173, 0.554483],
        [0.132444, 0.552216, 0.553018],
        [0.129933, 0.559582, 0.551864],
        [0.126453, 0.570633, 0.549841],
        [0.124395, 0.578002, 0.548287],
        [0.121831, 0.589055, 0.545623],
        [0.120092, 0.600104, 0.54253],
        [0.119512, 0.607464, 0.540218],
        [0.119699, 0.61849, 0.536347],
        [0.120638, 0.625828, 0.533488],
        [0.123444, 0.636809, 0.528763],
        [0.126326, 0.644107, 0.525311],
        [0.132268, 0.655014, 0.519661],
        [0.14021, 0.665859, 0.513427],
        [0.146616, 0.67305, 0.508936],
        [0.157851, 0.683765, 0.501686],
        [0.166383, 0.690856, 0.496502],
        [0.180653, 0.701402, 0.488189],
        [0.196571, 0.711827, 0.479221],
        [0.20803, 0.718701, 0.472873],
        [0.226397, 0.728888, 0.462789],
        [0.239374, 0.735588, 0.455688],
        [0.259857, 0.745492, 0.444467],
        [0.281477, 0.755203, 0.432552],
        [0.296479, 0.761561, 0.424223],
        [0.319809, 0.770914, 0.411152],
        [0.335885, 0.777018, 0.402049],
        [0.360741, 0.785964, 0.387814],
        [0.377779, 0.791781, 0.377939],
        [0.404001, 0.800275, 0.362552],
        [0.430983, 0.808473, 0.346476],
        [0.449368, 0.813768, 0.335384],
        [0.477504, 0.821444, 0.318195],
        [0.496615, 0.826376, 0.306377],
        [0.525776, 0.833491, 0.288127],
        [0.555484, 0.840254, 0.269281],
        [0.575563, 0.844566, 0.256415],
        [0.606045, 0.850733, 0.236712],
        [0.626579, 0.854645, 0.223353],
        [0.657642, 0.860219, 0.203082],
        [0.678489, 0.863742, 0.189503],
        [0.709898, 0.868751, 0.169257],
        [0.741388, 0.873449, 0.149561],
        [0.762373, 0.876424, 0.137064],
        [0.79376, 0.880678, 0.120005],
        [0.814576, 0.883393, 0.110347],
        [0.845561, 0.887322, 0.099702],
        [0.876168, 0.891125, 0.09525],
        [0.89632, 0.893616, 0.096335],
        [0.926106, 0.89733, 0.104071],
        [0.945636, 0.899815, 0.112838],
        [0.974417, 0.90359, 0.130215],
        [0.993248, 0.906157, 0.143936]
    ]
    @staticmethod
    def GET_BEACH_LINE_COLOR():
        return "black"
    @staticmethod
    def GET_BORDER_COLOR():
        return "red"
    @staticmethod
    def GET_CONST_POINTS_COLOR():
        return "black"
    @staticmethod
    def GET_EDGES_COLOR():
        return "blue"
    @staticmethod
    def GET_UNSTARTED_EDGES_COLOR():
        return "yellow"
    @staticmethod
    def GET_POINT_TYPE():
        return 0
    @staticmethod
    def GET_LINE_TYPE():
        return 1
    @staticmethod
    def GET_RANDOM_COLOR(color):
        return Const.random_colors[color % len(Const.random_colors)]

class DrawedItem:
    __slots__ = ['item', 'type', 'color']
    # type: 0 - point, 1 - line, 2 - oval
    def __init__(self, item, type, fill):
        self.type = type
        self.item = item
        self.color = fill
    def __str__(self):
        res = ""
        if self.type == Const.GET_LINE_TYPE():
            res += "Line(" + str(self.item) + ")"
        else:
            res += "Point(" + str(self.item) + ")"
        return res


class History:
    p = []
    const_points = []
    final_output = []
    hints = []
    history = []
    gettedHistory = []
    curr_frame = 0
    width = 0
    height = 0
    def __init__(self, width, height):
        self.width = width
        self.height = height

    def updateHistoryFrame(self, frame):
        def getHistory(const_points, final_output, p, width, height):
            def appendBeachLine(history, points, y2, width, height):
                last_y = 0.
                last_point = -1
                beach_line = []
                for x in range(width):
                    y = 0.
                    curr_point = 0
                    for i in range(len(points)):
                        point = points[i]
                        if (y < (y2 ** 2 - point.y ** 2 - (point.x - x) ** 2) / 2. / (y2 - point.y)):
                            y = (y2 ** 2 - point.y ** 2 - (point.x - x) ** 2) / 2. / (y2 - point.y)
                            curr_point = i
                    if last_point != -1 and curr_point != last_point:
                        curr_point, last_point = last_point, curr_point
                    else:
                        last_point = curr_point
                    history.append(DrawedItem((x, y, x - 1, last_y),
                                              Const.GET_LINE_TYPE(),
                                              Const.GET_RANDOM_COLOR(curr_point)))
                    last_y = y
                    beach_line.append(y)
                return beach_line
            def appendLinesVSBeachLine(history, lines, beach_line):
                width = len(beach_line)
                def reversed(l):
                    try:
                        return (l[1], l[0], l[3], l[2])
                    except:
                        return (l[0][1], l[0][0], l[1][1], l[1][0])
                for l in lines:
                    if (l[0] > l[2] or (l[0] == l[2] and l[1] < l[3])):
                        l = (l[2], l[3], l[0], l[1])
                    if (l[0] > width or l[2] < 0):
                        continue
                    if (l[0] < 0):
                        l = (0, (l[1] - l[3]) * l[2] / (l[2] - l[0]) + l[3], l[2], l[3])
                    if (l[2] > width):
                        l = (l[0], l[1], width - 1, (l[3] - l[1]) * (width - 1 - l[0]) / (l[2] - l[0]) + l[1])

                    if (l[2] == l[0]):
                        y = beach_line[int(l[0])]
                        if l[1] > y and l[3] > y:
                            continue
                        elif l[1] <= y and l[3] <= y:
                            history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                        else:
                            history.append(DrawedItem((l[0], min(l[1], l[3]), l[0], y), Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                        continue

                    if (beach_line[int(l[0])] >= l[1] and beach_line[int(l[2])] >= l[3]):
                        history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                        continue

                    used = False

                    first_good = (int(l[0]), l[1])
                    last_good = (int(l[0]), l[1])
                    for i in range(max(1, int(l[0]) + 1), min(int(l[2]) + 1, width)):
                        curr_pair = (i, (l[3] - l[1]) * (i - l[0]) / (l[2] - l[0]) + l[1])
                        if i == int(l[2]):
                            curr_pair = (int(l[2]), l[3])
                        nearest_curr_pair1 = (beach_line[i], i)
                        nearest_curr_pair2 = (beach_line[i], i)
                        if (i > 0):
                            nearest_curr_pair1 = min(nearest_curr_pair1, (beach_line[i - 1], i - 1))
                        if (i + 1 < width):
                            nearest_curr_pair2 = min(nearest_curr_pair2, (beach_line[i + 1], i + 1))

                        if beach_line[first_good[0]] < first_good[1]:
                            if beach_line[curr_pair[0]] >= curr_pair[1]:
                                first_good = (nearest_curr_pair1[1], nearest_curr_pair1[0])
                                last_good = (nearest_curr_pair1[1], nearest_curr_pair1[0])
                            continue
                        if beach_line[curr_pair[0]] < curr_pair[1]:
                            if (first_good == (int(l[0]), l[1])):
                                used = True
                                history.append(DrawedItem((first_good, (nearest_curr_pair2[1], nearest_curr_pair2[0])), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))
                            else:
                                used = True
                                history.append(DrawedItem((first_good, (nearest_curr_pair2[1], nearest_curr_pair2[0])), Const.GET_LINE_TYPE(), Const.GET_UNSTARTED_EDGES_COLOR()))
                            last_good = curr_pair
                            first_good = curr_pair
                        else:
                            last_good = curr_pair

                    if beach_line[first_good[0]] >= first_good[1] and beach_line[last_good[0]] >= last_good[1]:
                        history.append(DrawedItem((first_good, last_good), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))
                    elif (not used) and beach_line[int(l[2])] >= l[3]:
                        nearest_curr_pair = (beach_line[int(l[2])], l[2])
                        if (int(l[2]) > 0):
                            nearest_curr_pair = min(nearest_curr_pair, (beach_line[int(l[2]) - 1], l[2] - 1))
                        history.append(DrawedItem(((l[2], l[3]), (nearest_curr_pair[1], nearest_curr_pair[0])), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))

            points = []
            history = []
            y = p[0]
            for point in const_points:
                point = (point.x, point.y)
                if point[1] < y:
                    points.append(Site(point[0], point[1]))
            history.append(DrawedItem((-123123, y, 123123, y), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))
            history.append(DrawedItem(p[1], Const.GET_POINT_TYPE(), Const.GET_BORDER_COLOR()))
            beach_line = appendBeachLine(history, points, y, width, height)
            appendLinesVSBeachLine(history, final_output, beach_line)
            return history
        def getFinalHistory(lines):
            history = []
            for l in lines:
                history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
            return history

        if frame == 0:
            self.history[frame] = []
        elif frame + 1 == len(self.history):
            self.history[frame] = getFinalHistory(self.final_output)
        else:
            self.history[frame] = getHistory(self.const_points, self.final_output, self.p[frame - 1], self.width, self.height)
    def updateHistory(self, const_points):
        def getHint(event):
            return str(event) + "\n--------------------------------------------\n\n\n"
        def getFinalOutput(points):
            asd, dsa, output = computeVoronoiDiagram(points)
            res = []
            for edge in output[2]:
                if edge[1] != -1 and edge[2] != -1:
                    res.append((output[0][edge[1]][0], output[0][edge[1]][1], output[0][edge[2]][0], output[0][edge[2]][1]))
                elif edge[1] == -1 and edge[2] == -1:
                    l = output[1][edge[0]]
                    if l[1] == 0:
                        x = -l[2] / l[0]
                        res.append((x, -123123, x, 123123))
                    else:
                        res.append((-123123, (-l[2] + l[0] * 123123) / l[1],
                                    123123, (-l[2] + l[0] * -123123) / l[1]))
                else:
                    l = output[1][edge[0]]
                    l = (l[0], l[1], -l[2])

                    if (edge[1] == -1):
                        v = output[0][edge[2]]
                    else:
                        v = output[0][edge[1]]

                    if l[0] == 0:
                        if (edge[1] != -1) ^ (l[1] < 0):
                            res.append((v[0], v[1], 123123, -l[2] / l[1]))
                        else:
                            res.append((v[0], v[1], -123123, -l[2] / l[1]))
                    elif l[1] == 0:
                        if (edge[1] == -1) ^ (l[0] < 0):
                            res.append((v[0], v[1], -l[2] / l[0], 123123))
                        else:
                            res.append((v[0], v[1], -l[2] / l[0], -123123))
                    else:
                        if (edge[1] == -1) ^ (l[0] * l[1] > 0):
                            res.append((v[0], v[1], (-l[2] + l[1] * 123123) / l[0], -123123))
                        else:
                            res.append((v[0], v[1], (-l[2] - l[1] * 123123) / l[0], 123123))
            return res

        if len(const_points) < 1:
            return
        self.history = [[], []]
        self.const_points = const_points
        for i in range(len(self.const_points)):
            self.const_points[i] = Site(self.const_points[i][0], self.const_points[i][1])
        self.final_output = getFinalOutput(self.const_points)
        self.p, hints, fdasfasd = computeVoronoiDiagram(self.const_points)
        self.hints = ['Points are initialised\n--------------------------------------------\n\n\n']
        for hint in hints:
            self.hints.append(getHint(hint))
        self.hints.append('Voronnoi is builded. Congratulations!!! ;)\n--------------------------------------------\n\n\n')

        self.history = [None for i in range(len(self.p) + 2)]
        self.gettedHistory = [False for i in range(len(self.p) + 2)]
        self.updateHistoryFrame(0)
        self.updateHistoryFrame(len(self.history) - 1)
        self.curr_frame = 0
        self.checkNearesFrames()

    def checkNearesFrames(self):
        if self.curr_frame - 1 >= 0 and not self.gettedHistory[self.curr_frame - 1]:
            self.updateHistoryFrame(self.curr_frame - 1)
        if self.curr_frame + 1 < len(self.history) and not self.gettedHistory[self.curr_frame + 1]:
            self.updateHistoryFrame(self.curr_frame + 1)
    def getCurrFrame(self):
        try:
            if len(self.history) < 1:
                return []
            return self.history[self.curr_frame], self.hints[self.curr_frame]
        except:
            print (self.curr_frame)
            return self.history[self.curr_frame], self.hints[self.curr_frame]

    def getNextFrame(self):
        if 1 + self.curr_frame < len(self.history):
            self.curr_frame += 1
        return self.getCurrFrame()
    def getPrevFrame(self):
        if self.curr_frame > 0:
            self.curr_frame -= 1
        return self.getCurrFrame()
    def getLastFrame(self):
        self.curr_frame = len(self.history) - 1
        return self.getCurrFrame()
    def getFirstFrame(self):
        self.curr_frame = 0
        return self.getCurrFrame()

class Canvas:
    to_delete = []
    BLACK_BEACH_LINE = True
    RADIUS = 3

    def __init__(self, w):
        self.w = w
    def addPoint(self, x, y, color):
        self.w.create_oval(x - self.RADIUS,
                           y - self.RADIUS,
                           x + self.RADIUS,
                           y + self.RADIUS,
                           fill=color)
    def clearAll(self):
        self.w.delete(tk.ALL)
    def getPoints(self):
        pObj = self.w.find_all()
        points = []
        for p in pObj:
            coord = self.w.coords(p)
            points.append((coord[0] + self.RADIUS, coord[1] + self.RADIUS))
        return points
    def clear_lines(self):
        for item in self.to_delete:
            self.w.delete(item)
    def drawItems(self, items):
        self.clear_lines()
        self.drawFrame(items)
    def drawFrame(self, items):
        for item in items:
            if item.type == Const.GET_POINT_TYPE():
                self.drawPoint(item.item, item.color)
            else:
                self.drawLine(item.item, item.color)
    def drawPoint(self, point, fill):
        try:
            fill = '#%02x%02x%02x' % (fill[0] * 255, fill[1] * 255, fill[2] * 255)
            if self.BLACK_BEACH_LINE:
                fill = Const.GET_BEACH_LINE_COLOR()
        except:
            sdf = 0

        try:
            self.to_delete.append(self.w.create_oval(point[0] - self.RADIUS,
                                                     point[1] - self.RADIUS,
                                                     point[0] + self.RADIUS,
                                                     point[1] + self.RADIUS,
                                                     fill=fill))
        except:
            self.to_delete.append(self.w.create_oval(point.x - self.RADIUS,
                                                     point.y - self.RADIUS,
                                                     point.x + self.RADIUS,
                                                     point.y + self.RADIUS,
                                                     fill=fill))
    def drawLine(self, line, fill):
        try:
            fill = '#%02x%02x%02x' % (fill[0] * 255, fill[1] * 255, fill[2] * 255)
            if self.BLACK_BEACH_LINE:
                fill = Const.GET_BEACH_LINE_COLOR()
        except:
            sdf = 0
        try:
            self.to_delete.append(self.w.create_line(line[0].x, line[0].y, line[1].x, line[1].y, fill=fill))
        except:
            try:
                self.to_delete.append(
                    self.w.create_line(line[0][0], line[0][1], line[1][0], line[1][1], fill=fill))
            except:
                self.to_delete.append(self.w.create_line(line[0], line[1], line[2], line[3], fill=fill))

class MainWindow:
    history = None
    canvas = None

    width = 1000
    height = 900

    # flag to lock the canvas when drawn
    LOCK_FLAG = False

    def __init__(self, master):
        self.master = master
        self.master.title("Voronoi")
        master.bind('<Left>', self.prevStep)
        master.bind('<Shift-Left>', self.firstStep)
        master.bind('<Right>', self.nextStep)
        master.bind('<Shift-Right>', self.lastStep)
        master.bind('<c>', self.colored_beach_line)

        self.frmMain = tk.Frame(self.master, relief=tk.RAISED, borderwidth=1)
        self.frmMain.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)

        canvas = tk.Canvas(self.frmMain, width=self.width, height=self.height)
        canvas.config(background='white')
        canvas.bind('<Double-1>', self.onDoubleClick)
        canvas.pack()
        self.canvas = Canvas(canvas)

        width = 25
        height = self.height

        frmButton = tk.Frame(self.master, width=50)
        frmButton.pack(side='right', expand=True)

        tk.Label(frmButton, text='Number of random points', width=35, height=1).pack()
        self.textRandomPoints = tk.Text(frmButton, width=4, height=1, wrap=tk.WORD)
        self.textRandomPoints.insert(1.0, "50")
        self.textRandomPoints.pack()
        tk.Button(frmButton, text="Create random points", width=25, command=self.createRandomPoints).pack()
        tk.Button(frmButton, text='Clear', width=25, command=self.onClickClear).pack()
        tk.Button(frmButton, text='Open', width=25, command=self.read_from_file).pack()
        tk.Button(frmButton, text='Save', width=25, command=self.save).pack()

        mng = tk.Text(frmButton, width=35, height=10, bg='gray')
        mng.pack(fill='both')
        master.bind('<Left>', self.prevStep)
        master.bind('<Shift-Left>', self.firstStep)
        master.bind('<Right>', self.nextStep)
        master.bind('<Shift-Right>', self.lastStep)
        master.bind('c', self.colored_beach_line)
        mng.insert('1.0', "<Left>, <Right>             \n           shift visualisation step by step\n" +
                   "--------------------------------------------\n" +
                          "<Shift-Left>, <Shift-Right> \n             shift in start/finish position\n" +
                   "--------------------------------------------\n" +
                   "<Double LBM> - make new point (when cleared)\n" +
                   "--------------------------------------------\n" +
                   "<c> - change color of brach line")
        mng.config(state=tk.DISABLED)

        self.hints = tk.Text(frmButton, width=35, height=40, bg='gray')
        self.hints.pack(side=tk.BOTTOM, expand=True, fill='both')
        self.hints.config(state=tk.DISABLED)

        self.history = History(self.width, self.height)

    def colored_beach_line(self, event):
        self.canvas.BLACK_BEACH_LINE = not self.canvas.BLACK_BEACH_LINE
        self.updateFrame(self.history.getCurrFrame)

    def rescale(self, points, min_x, min_y, max_x, max_y):
        new_min_x = self.width * 0.1
        new_min_y = self.height * 0.1
        new_max_x = self.width * 0.9
        new_max_y = self.height * 0.9
        width = max_x - min_x
        new_width = new_max_x - new_min_x
        height = max_y - min_y
        new_height = new_max_y - new_min_y
        for i in range(len(points)):
            points[i] = ((points[i][0] - min_x) * new_width / width + new_min_x,
                         (points[i][1] - min_y) * new_height / height + new_min_y)

    def save(self, filename=""):
        if len(filename) < 1:
            filename = "tests/new_res.txt"
        file = open(filename, 'w')
        points = self.canvas.getPoints()
        file.write(str(len(points)) + "\n")
        for point in points:
            file.write(str(point[0]) + " " + str(point[1]) + "\n")
        file.close()

    def read(self, filename):
        file = open(filename, 'r')
        points = []
        min_x = 1000111222
        min_y = 1000111222
        max_x = -1000111222
        max_y = -1000111222
        try:
            n = int(file.readline())
            for i in range(n):
                x, y = file.readline().split()
                x = float(x)
                y = float(y)
                min_x = min(min_x, x)
                max_x = max(max_x, x)
                min_y = min(min_y, y)
                max_y = max(max_y, y)
                points.append((int(x), int(y)))
            if min_x < self.width * 0.09 or \
                max_x > self.width * 0.91 or \
                min_y < self.height * 0.09 or \
                max_y > self.height * 0.91:
                self.rescale(points, min_x, min_y, max_x, max_y)
            file.close()
            self.onClickClear()
            return points
        except:
            print("File format is not supported.")
            return None

    def read_from_file(self):
        filename = tkFileDialog.askopenfilename()
        if filename is None or filename == u'': return
        points = self.read(filename)
        for point in points:
            self.canvas.addPoint(point[0], point[1], Const.GET_CONST_POINTS_COLOR())

    def createRandomPoints(self):
        self.onClickClear()
        try:
            n = int(self.textRandomPoints.get('1.0', tk.END))
        except:
            n = 100
        points = []
        if n <= 100:
            if (n < 10):
                n = 10
            for i in range(n):
                points.append((randint(self.width * 0.1, self.width * 0.9),
                               randint(self.height * 0.1, self.height * 0.9)))
        else:
            arr = [(100, 2), (500, 5), (1000, 5)]
            my_item = arr[0]
            for item in arr:
                if math.fabs(my_item[0] - n) > math.fabs(item[0] - n):
                    my_item = item
            rnd = randint(1, my_item[1])
            print ("tests/" + str(my_item[0]) + "_" + str(rnd) + ".txt")
            points = self.read("tests/" + str(my_item[0]) + "_" + str(rnd) + ".txt")
        for point in points:
            self.canvas.addPoint(point[0], point[1], Const.GET_CONST_POINTS_COLOR())

    def updateFrame(self, func):
        if not self.LOCK_FLAG:
            self.history.updateHistory(self.canvas.getPoints())
        self.LOCK_FLAG = True
        asd = func()
        try:
            frame, hint = asd
        except:
            frame = asd
            hint = None
        self.canvas.drawItems(frame)
        self.hints.config(state=tk.NORMAL)
        self.hints.insert(1.0, hint)
        self.hints.config(state=tk.DISABLED)
        self.history.checkNearesFrames()

    def firstStep(self, event=None):
        self.updateFrame(self.history.getFirstFrame)
    def prevStep(self, event=None):
        self.updateFrame(self.history.getPrevFrame)
    def nextStep(self, event=None):
        self.updateFrame(self.history.getNextFrame)
    def lastStep(self, event=None):
        self.updateFrame(self.history.getLastFrame)

    def onClickClear(self):
        self.LOCK_FLAG = False
        self.canvas.clearAll()
    def onDoubleClick(self, event):
        if not self.LOCK_FLAG:
            self.canvas.addPoint(event.x, event.y, Const.GET_CONST_POINTS_COLOR())





def main():
    shuffle(Const.random_colors)
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
