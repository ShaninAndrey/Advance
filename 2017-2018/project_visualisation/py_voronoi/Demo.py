import Tkinter as tk
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
    history = []


class MainWindow:
    history = []
    curr_frame = -1
    to_delete = []
    vp = Voronoi([])
    # radius of drawn points on canvas
    RADIUS = 3
    random_points = 20
    width = 1000
    height = 600

    # flag to lock the canvas when drawn
    LOCK_FLAG = False
    LOCK_NEXT = False

    def clear_lines(self):
        for item in self.to_delete:
            self.w.delete(item)

    def getPoints(self):
        pObj = self.w.find_all()
        points = []
        for p in pObj:
            coord = self.w.coords(p)
            points.append((coord[0] + self.RADIUS, coord[1] + self.RADIUS))
        return points


    def __init__(self, master):
        self.master = master
        self.master.title("Voronoi")

        self.frmMain = tk.Frame(self.master, relief=tk.RAISED, borderwidth=1)
        self.frmMain.pack(fill=tk.BOTH, expand=1)

        self.w = tk.Canvas(self.frmMain, width=self.width, height=self.height)
        self.w.config(background='white')
        self.w.bind('<Double-1>', self.onDoubleClick)
        self.w.pack()

        self.frmButton = tk.Frame(self.master)
        self.frmButton.pack()
        
        self.btnCalculate = tk.Button(self.frmButton, text='Calculate', width=25, command=self.onClickCalculate)
        self.btnCalculate.pack(side=tk.LEFT)
        
        self.btnClear = tk.Button(self.frmButton, text='Clear', width=25, command=self.onClickClear)
        self.btnClear.pack(side=tk.LEFT)

        self.btnNextStep = tk.Button(self.frmButton, text="Next step", width=25, command=self.nextStep)
        self.btnNextStep.pack(side=tk.LEFT)

        self.btnRndomPoints = tk.Button(self.frmButton, text="Create random points", width=25, command=self.createRandomPoints)
        self.btnRndomPoints.pack(side=tk.LEFT)

    def getFinalOutput(self, points):
        vp = Voronoi(points)
        while vp.process():
            asd = 0
        return vp.get_output()

    def getHistory(self):
        const_points = self.getPoints()
        final_output = self.getFinalOutput(const_points)
        vp = Voronoi(const_points)
        p = True
        history = []
        while p is not None:
            if p == True:
                p = vp.process()
                continue
            points = []
            for point in const_points:
                if point[0] < p.x:
                    points.append(Point(point[0], point[1]))
            history.append([])
            history[-1].append(DrawedItem((p.x, -123123, p.x, 123123), Const.GET_LINE_TYPE(), Const.GET_BORDER_COLOR()))
            beach_line = self.appendBeachLine(history[-1], points, p.x)
            self.appendLinesVSBeachLine(history[-1], final_output, beach_line)

            p = vp.process()

        history.append([])
        self.appendFinalLines(history[-1], final_output)

        return history

    def createRandomPoints(self):
        self.onClickClear()
        for i in range(self.random_points):
            x = randint(30, self.width - 30)
            y = randint(30, self.height - 30)
            self.w.create_oval(x - self.RADIUS, y - self.RADIUS, x + self.RADIUS,
                               y + self.RADIUS, fill="black")
        self.history = self.getHistory()
        self.curr_frame = -1

    def nextStep(self):
        if self.curr_frame + 1 < len(self.history):
            self.curr_frame += 1
            self.clear_lines()
            self.drawFrame(self.history[self.curr_frame])

    def onClickCalculate(self):
        self.LOCK_FLAG = True
        self.curr_frame = len(self.history) - 1
        self.clear_lines()
        self.drawFrame(self.history[self.curr_frame])

    def onClickClear(self):
        self.LOCK_FLAG = False
        self.LOCK_NEXT = False
        self.points = []
        self.real_output = []
        self.w.delete(tk.ALL)

    def onDoubleClick(self, event):
        if not self.LOCK_FLAG:
            self.w.create_oval(event.x-self.RADIUS, event.y-self.RADIUS, event.x+self.RADIUS, event.y+self.RADIUS, fill="black")



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
                                                     point[0] - self.RADIUS,
                                                     point[1] - self.RADIUS,
                                                     fill=fill))
        except:
            self.to_delete.append(self.w.create_oval(point.x - self.RADIUS,
                                                     point.y - self.RADIUS,
                                                     point.x - self.RADIUS,
                                                     point.y - self.RADIUS,
                                                     fill=fill))

    def drawLine(self, line, fill):
        try:
            self.to_delete.append(self.w.create_line(line[0].x, line[0].y, line[1].x, line[1].y, fill=fill))
        except:
            try:
                self.to_delete.append(self.w.create_line(line[0][0], line[0][1], line[1][0], line[1][1], fill=fill))
            except:
                self.to_delete.append(self.w.create_line(line[0], line[1], line[2], line[3], fill=fill))



    def appendBeachLine(self, history, points, x2):
        last_x = 0.
        beach_line = []
        for y in range(self.height):
            x = 0.
            for point in points:
                x = max(x, (x2 ** 2 - point.x ** 2 - (point.y - y) ** 2) / 2. / (x2 - point.x))
            history.append(DrawedItem((x, y, last_x, y - 1),
                                               Const.GET_LINE_TYPE(),
                                               Const.GET_BEACH_LINE_COLOR()))
            last_x = x
            beach_line.append(x)
        return beach_line

    def appendLinesVSBeachLine(self, history, lines, beach_line):
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
                        history.append(DrawedItem(((l[0], l[1]), mn), Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))
                    else:
                        history.append(DrawedItem(((l[2], l[3]), mn), Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))

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

    def appendFinalLines(self, history, lines):
        for l in lines:
            history.append(DrawedItem(l, Const.GET_LINE_TYPE(), Const.GET_EDGES_COLOR()))

def main(): 
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
