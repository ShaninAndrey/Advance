import Tkinter as tk
from DataType import *
from Voronoi import Voronoi

class MainWindow:
    points = []
    to_delete = []
    vp = Voronoi([])
    real_output = []
    beach_line = []
    # radius of drawn points on canvas
    RADIUS = 3

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

        self.w = tk.Canvas(self.frmMain, width=1000, height=600)
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

    def drawBeachLine(self, points, x2, fill="black"):
        last_x = 0.
        self.beach_line = []
        for y in range(self.w.winfo_height()):
            x = 0.
            for point in points:
                x = max(x, (x2 ** 2 - point.x ** 2 - (point.y - y) ** 2) / 2. / (x2 - point.x))
            self.to_delete.append(self.w.create_line(x, y, last_x, y - 1, fill=fill))
            last_x = x
            self.beach_line.append(x)

    def drawLinesVSBeachLine(self, lines, fill="blue"):
        for l in lines:
            if (l[1] > l[3]):
                l = (l[2], l[3], l[0], l[1])
            last_x = l[0]
            for i in range(max(0, int(l[1]) + 1), min(int(l[3]) + 1, self.w.winfo_height())):
                x = (l[2] - l[0]) * (i - l[1]) / (l[3] - l[1]) + l[0]
                if (x <= self.beach_line[i] and last_x <= self.beach_line[i - 1]):
                    self.to_delete.append(self.w.create_line(last_x, i - 1, x, i, fill=fill))
                elif (x <= self.beach_line[i]):
                    self.to_delete.append(self.w.create_line(self.beach_line[i - 1], i - 1, x, i, fill=fill))
                elif (last_x <= self.beach_line[i - 1]):
                    self.to_delete.append(self.w.create_line(last_x, i - 1, self.beach_line[i], i, fill=fill))
                last_x = x

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
            self.to_delete.append(self.w.create_line(l[0], l[1], l[2], l[3], fill=fill))

def main(): 
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
