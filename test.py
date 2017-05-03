class node:
    def __init__(self):
        self.next=None
        self.value=int(22)
    def __str__(self):
        return self.value.__str__()

class mylist:
    def __init__(self):
        self.top=None
    def getlast(self):
        p=self.top
        while not p is None:
            p=p.next
        return p

    def printlist(self):
        p=self.top
        while not p is None:
            print(p.value," ")
            p=p.next

    def add(self,anynode):
        if self.top is None:
            self.top=anynode
            anynode.next=None
        else:
            p=self.getlast()
            p.next=anynode
            anynode.next=None

def main():
    m=mylist()
    m.add(node())
    #m.add(node())
    m.printlist()



if __name__=="__main__":
    main()