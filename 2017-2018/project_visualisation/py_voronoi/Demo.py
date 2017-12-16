import Tkinter as tk

from Voronoi import Voronoi

class MainWindow:
    points = []
    vp = Voronoi([])
    # radius of drawn points on canvas
    RADIUS = 3

    # flag to lock the canvas when drawn
    LOCK_FLAG = False
    LOCK_NEXT = False

    def getPoints(self):
        pObj = self.w.find_all()
        self.points = []
        for p in pObj:
            coord = self.w.coords(p)
            self.points.append((coord[0] + self.RADIUS, coord[1] + self.RADIUS))

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

        self.btnNextStep = tk.Button(self.frmButton, text="Netx step", width=25, command=self.nextStep)
        self.btnNextStep.pack(side=tk.LEFT)

    def nextStep(self):
        if not self.LOCK_FLAG:
            self.LOCK_FLAG = True
            self.getPoints()
            self.vp = Voronoi(self.points)

        if not self.LOCK_NEXT:
            print(self.points)
            if not self.vp.process():
                self.LOCK_NEXT = True
            lines = self.vp.get_output()
            print(lines)
            self.drawLinesOnCanvas(lines)


    def onClickCalculate(self):
        if not self.LOCK_FLAG:
            self.LOCK_FLAG = True
            self.getPoints()
            self.vp = Voronoi(self.points)
            print (self.points)

        if not self.LOCK_NEXT:
            while (self.vp.process()):
                asd = 0
            self.LOCK_NEXT = True

            self.drawLinesOnCanvas(self.vp.get_output())

    def onClickClear(self):
        self.LOCK_FLAG = False
        self.LOCK_NEXT = False
        self.points = []
        self.w.delete(tk.ALL)

    def onDoubleClick(self, event):
        if not self.LOCK_FLAG:
            self.w.create_oval(event.x-self.RADIUS, event.y-self.RADIUS, event.x+self.RADIUS, event.y+self.RADIUS, fill="black")

    def drawLinesOnCanvas(self, lines):
        for l in lines:
            self.w.create_line(l[0], l[1], l[2], l[3], fill='blue')

def main(): 
    root = tk.Tk()
    app = MainWindow(root)
    root.mainloop()

if __name__ == '__main__':
    main()
