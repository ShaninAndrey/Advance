import Tkinter as tk
import sys, time
from threading import Timer
import multiprocessing
from DataType import Point
from Voronoi import Voronoi
from random import randint

class Const:
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
    def GET_POINT_TYPE():
        return 0
    @staticmethod
    def GET_LINE_TYPE():
        return 1

class DrawedItem:
    __slots__ = ['item', 'type', 'color']
    # type: 0 - point, 1 - line, 2 - oval
    def __init__(self, item, type, fill):
        self.type = type
        self.item = item
        self.color = fill

class History:
    p = []
    const_points = []
    final_output = []
    hints = []
    history = []
    gettedHistory = []
    curr_frame = 0
    height = 0
    def __init__(self, height):
        self.height = height

    def updateHistoryFrame(self, frame):
        def getHistory(const_points, final_output, p, height):
            def appendBeachLine(history, points, x2, height):
                last_x = 0.
                beach_line = []
                for y in range(height):
                    x = 0.
                    for point in points:
                        x = max(x, (x2 ** 2 - point.x ** 2 - (point.y - y) ** 2) / 2. / (x2 - point.x))
                    history.append(DrawedItem((x, y, last_x, y - 1),
                                              Const.GET_LINE_TYPE(),
                                              Const.GET_BEACH_LINE_COLOR()))
                    last_x = x
                    beach_line.append(x)
                return beach_line
            def appendLinesVSBeachLine(history, lines, beach_line):
                height = len(beach_line)
                for l in lines:
                    if (l[1] > l[3]):
                        l = (l[2], l[3], l[0], l[1])
                    if (l[1] < 0):
                        l = ((l[2] - l[0]) * (0 - l[1]) / (l[3] - l[1]) + l[0], 0, l[2], l[3])
                    if (l[3] > height):
                        l = (l[0], l[1], (l[2] - l[0]) * (height - l[1]) / (l[3] - l[1]) + l[0], height)

                    last_x = l[0]
                    fl = False
                    if l[0] > beach_line[max(0, min(height - 1, int(l[0])))] or \
                       l[2] > beach_line[max(0, min(height - 1, int(l[3])))]:
                        fl = True
                    for i in range(max(0, int(l[1]) + 1), min(int(l[3]) + 1, height)):
                        x = (l[2] - l[0]) * (i - l[1]) / (l[3] - l[1]) + l[0]
                        if (last_x <= beach_line[i - 1] and x > beach_line[i] or
                                        last_x > beach_line[i - 1] and x <= beach_line[i]):
                            fl = True
                            mn = (beach_line[i], i)
                            if (i):
                                mn = min(mn, (beach_line[i - 1], i - 1))
                            if (i + 1 < height):
                                mn = min(mn, (beach_line[i + 1], i + 1))
                            if (last_x <= beach_line[i - 1]):
                                history.append(
                                    DrawedItem(((l[0], l[1]), mn), Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                            else:
                                history.append(
                                    DrawedItem(((l[2], l[3]), mn), Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))

                            break
                        if x > beach_line[i]:
                            fl = True
                        last_x = x
                    if l[1] == l[3]:
                        if l[0] > beach_line[int(l[1])] and l[2] > beach_line[int(l[1])]:
                            continue
                        if l[0] <= beach_line[int(l[1])] and l[2] <= beach_line[int(l[1])]:
                            history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                        elif l[0] <= beach_line[int(l[1])]:
                            history.append(DrawedItem(((l[0], l[1]), (beach_line[int(l[3])], l[3])),
                                                      Const.GET_LINE_TYPE(),
                                                      Const.GET_EDGES_COLOR()))
                        else:
                            history.append(DrawedItem(((l[2], l[3]), (beach_line[int(l[1])], l[1])),
                                                      Const.GET_LINE_TYPE(),
                                                      Const.GET_EDGES_COLOR()))
                        continue
                    if not fl:
                        history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))

            points = []
            history = []
            x = p[0]
            for point in const_points:
                if point[0] < x:
                    points.append(Point(point[0], point[1]))
            history.append(DrawedItem((x, -123123, x, 123123), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))
            history.append(DrawedItem(p[1], Const.GET_POINT_TYPE(), Const.GET_BORDER_COLOR()))
            beach_line = appendBeachLine(history, points, x, height)
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
            self.history[frame] = getHistory(self.const_points, self.final_output, self.p[frame - 1], self.height)
    def updateHistory(self, const_points):
        def getHint(event):
            res = "X: " + "{0:.2f}".format(event.x) + "\n"
            try:
                tmp = event.a
                res = res + "Type: Circle\n"
            except:
                res = res + "Type: Side\n"
            res = res + str(event) + "\n--------------------------------------------\n\n\n"
            return res
        def getFinalOutput(points):
            vp = Voronoi(points)
            while vp.process():
                asd = 0
            return vp.get_output()

        if len(const_points) < 1:
            return
        self.final_output = getFinalOutput(const_points)
        self.const_points = const_points
        vp = Voronoi(const_points)
        self.p = []
        self.hints = ['Points are initialised\n--------------------------------------------\n\n\n']
        while True:
            res = vp.process()
            if res is not None:
                try:
                    self.p.append((res.x, res.p))
                except:
                    self.p.append((res.x, res))
                self.hints.append(getHint(res))
            else:
                break
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
        if len(self.history) < 1:
            return []
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

        self.frmMain = tk.Frame(self.master, relief=tk.RAISED, borderwidth=1)
        self.frmMain.pack(side=tk.LEFT, fill=tk.BOTH, expand=1)

        canvas = tk.Canvas(self.frmMain, width=self.width, height=self.height)
        canvas.config(background='white')
        canvas.bind('<Double-1>', self.onDoubleClick)
        canvas.pack()
        self.canvas = Canvas(canvas)

        width = 25
        height = self.height

        self.frmButton = tk.Frame(self.master, width=50)
        self.frmButton.pack(side='right', expand=True)

        self.labelRandomPoints = tk.Label(self.frmButton, text='Number of random points', width=35, height=1)
        self.labelRandomPoints.pack()
        self.textRandomPoints = tk.Text(self.frmButton, width=4, height=1, wrap=tk.WORD)
        self.textRandomPoints.insert(1.0, "50")
        self.textRandomPoints.pack()
        self.btnRandomPoints = tk.Button(self.frmButton, text="Create random points", width=25, command=self.createRandomPoints)
        self.btnRandomPoints.pack()
        self.btnClear = tk.Button(self.frmButton, text='Clear', width=25, command=self.onClickClear)
        self.btnClear.pack()

        self.hints = tk.Text(self.frmButton, width=35, height=50, bg='gray')
        self.hints.pack(side=tk.BOTTOM, expand=True, fill='both')
        self.hints.config(state=tk.DISABLED)

        self.history = History(self.height)

    def createRandomPoints(self):
        self.onClickClear()
        for i in range(int(self.textRandomPoints.get('1.0', tk.END))):
            x = randint(30, self.width - 30)
            y = randint(30, self.height - 30)
            self.canvas.addPoint(x, y, Const.GET_CONST_POINTS_COLOR())

    def updateFrame(self, func):
        if not self.LOCK_FLAG:
            self.history.updateHistory(self.canvas.getPoints())
        self.LOCK_FLAG = True
        frame, hint = func()
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
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
