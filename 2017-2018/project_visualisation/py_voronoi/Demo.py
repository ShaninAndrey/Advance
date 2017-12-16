import Tkinter as tk
from DataType import Point
from Voronoi import Voronoi
from random import randint

class MainWindow:
    points = []
    to_delete = []
    vp = Voronoi([])
    real_output = []
    beach_line = []
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

    def processPoints(self):
        pObj = self.w.find_all()
        self.points = []
        for p in pObj:
            coord = self.w.coords(p)
            self.points.append((coord[0] + self.RADIUS, coord[1] + self.RADIUS))
        vp = Voronoi(self.points)
        while vp.process():
            asd = 0
        self.real_output = vp.get_output()


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

    def createRandomPoints(self):
        self.onClickClear()
        for i in range(self.random_points):
            x = randint(30, self.width - 30)
            y = randint(30, self.height - 30)
            self.w.create_oval(x - self.RADIUS, y - self.RADIUS, x + self.RADIUS,
                               y + self.RADIUS, fill="black")

    def nextStep(self):
        if not self.LOCK_FLAG:
            self.LOCK_FLAG = True
            self.processPoints()
            self.vp = Voronoi(self.points)

        if not self.LOCK_NEXT:
            p = self.vp.process()
            self.clear_lines()

            if not p:
                self.LOCK_NEXT = True
                self.drawLinesOnCanvas(self.real_output)
            else:
                self.drawLinesOnCanvas([(p.x, -123123, p.x, 123123)], "red")
                points = []
                for point in self.points:
                    if point[0] < p.x:
                        points.append(Point(point[0], point[1]))
                self.drawBeachLine(points, p.x)
                self.drawLinesVSBeachLine(self.real_output)

    def onClickCalculate(self):
        if not self.LOCK_FLAG:
            self.LOCK_FLAG = True
            self.processPoints()
        self.LOCK_NEXT = True
        self.clear_lines()
        self.drawLinesOnCanvas(self.real_output)

    def onClickClear(self):
        self.LOCK_FLAG = False
        self.LOCK_NEXT = False
        self.points = []
        self.real_output = []
        self.w.delete(tk.ALL)

    def onDoubleClick(self, event):
        if not self.LOCK_FLAG:
            self.w.create_oval(event.x-self.RADIUS, event.y-self.RADIUS, event.x+self.RADIUS, event.y+self.RADIUS, fill="black")



    def drawLine(self, line, fill):
        try:
            self.to_delete.append(self.w.create_line(line[0].x, line[0].y, line[1].x, line[1].y, fill=fill))
        except:
            try:
                self.to_delete.append(self.w.create_line(line[0][0], line[0][1], line[1][0], line[1][1], fill=fill))
            except:
                self.to_delete.append(self.w.create_line(line[0], line[1], line[2], line[3], fill=fill))

    def drawBeachLine(self, points, x2, fill="black"):
        last_x = 0.
        self.beach_line = []
        for y in range(self.height):
            x = 0.
            for point in points:
                x = max(x, (x2 ** 2 - point.x ** 2 - (point.y - y) ** 2) / 2. / (x2 - point.x))
            self.drawLine((x, y, last_x, y - 1), fill)
            last_x = x
            self.beach_line.append(x)

    def drawLinesVSBeachLine(self, lines, fill="blue"):
        for l in lines:
            if (l[1] > l[3]):
                l = (l[2], l[3], l[0], l[1])
            if (l[1] < 0):
                l = ((l[2] - l[0]) * (0 - l[1]) / (l[3] - l[1]) + l[0], 0, l[2], l[3])
            if (l[3] > self.height):
                l = (l[0], l[1], (l[2] - l[0]) * (self.height - l[1]) / (l[3] - l[1]) + l[0], self.height)

            last_x = l[0]
            fl = False
            for i in range(max(0, int(l[1]) + 1), min(int(l[3]) + 1, self.height)):
                x = (l[2] - l[0]) * (i - l[1]) / (l[3] - l[1]) + l[0]
                if (last_x <= self.beach_line[i - 1] and x > self.beach_line[i] or
                    last_x > self.beach_line[i - 1] and x <= self.beach_line[i]):
                    fl = True
                    if (last_x <= self.beach_line[i - 1]):
                        self.drawLine(((l[0], l[1]), min([(self.beach_line[i - 1], i - 1),
                                                          (self.beach_line[i + 1], i + 1),
                                                          (self.beach_line[i], i)])), fill)
                    else:
                        self.drawLine(((l[2], l[3]), min([(self.beach_line[i - 1], i - 1),
                                                          (self.beach_line[i + 1], i + 1),
                                                          (self.beach_line[i], i)])), fill)

                    break
                if x > self.beach_line[i]:
                    fl = True
                last_x = x
            if l[1] == l[3]:
                if l[0] > self.beach_line[int(l[1])] and l[2] > self.beach_line[int(l[1])]:
                    continue
                if l[0] <= self.beach_line[int(l[1])] and l[2] <= self.beach_line[int(l[1])]:
                    self.drawLine(l, fill)
                elif l[0] <= self.beach_line[int(l[1])]:
                    self.drawLine(((l[0], l[1]), (self.beach_line[int(l[3])], l[3])), fill)
                else:
                    self.drawLine(((l[2], l[3]), (self.beach_line[int(l[1])], l[1])), fill)
                continue
            if not fl:
                self.drawLine(l, fill)

    def drawPointsOnCanvas(self, points, fill="red"):
        for p in points:
            self.to_delete.append(self.w.create_oval(p.x - self.RADIUS,
                                                     p.y - self.RADIUS,
                                                     p.x + self.RADIUS,
                                                     p.y + self.RADIUS,
                                                     fill=fill
                                                     ))

    def drawLinesOnCanvas(self, lines, fill="blue"):
        for l in lines:
            self.drawLine(l, fill)

def main(): 
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
