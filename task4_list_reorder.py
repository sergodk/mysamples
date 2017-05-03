# Имеется простой односвязный список размера N. Необходимо реализовать алгоритм, который перепаковывает список так, чтобы за первым элеметом следовал последний, затем второй,
# затем предпоследний и т. д. Пример работы алгоритма:
# исходный список: 1-2-3-4-5-6-7-8
# перепакованный список: 1-8-2-7-3-6-4-5.

# Класс узел списка
class node:
    def __init__(self, n=int(22)):
        self.next=None
        self.value=n
    def __str__(self):
        return self.value.__str__()

# Класс список
class mylist:

    def __init__(self):
        self.top=None

    # Добавление узла anynode в список
    def add(self,anynode):
        if self.top is None:
            self.top=anynode
            anynode.next=None
        else:
            p=self.getlast(self.top)
            p.next=anynode
            anynode.next=None

    # Вставка узла insnode в список после узла anynode
    def insertafter(self,anynode,insnode):
        if anynode==None:
            return
        else:
            if anynode.next==None:
                anynode.next=insnode
            else:
                insnode.next=anynode.next
                anynode.next=insnode

    # Получить последний узел списка, начиная с узла anynode
    def getlast(self,anynode) -> node:
        if anynode.next is None:
            return anynode
        else:
            return self.getlast(anynode.next)

    # Получить предпоследний узел списка, начиная с узла anynode
    def getprelast(self, anynode) -> node:
        if anynode.next is None:
            return None
        elif anynode.next.next is None:
            return anynode
        else:
            return self.getprelast(anynode.next)

    # Печать списка
    def printlist(self,anynode):
        if not anynode is None:
            if anynode.next is None:
                print(str(anynode.value))
            else:
                print(str(anynode.value),end=',')
            self.printlist(anynode.next)

    # Перепаковка списка в соответствии с заданным правилом
    def reorder(self):
        if self.top==None or self.top.next==None or self.top.next.next==None:
            print("List is empty or too small for reodering")
            return
        p=self.top
        while not p.next is None and not p.next.next is None:
            prelast=self.getprelast(p)
            self.insertafter(p,prelast.next)
            prelast.next=None
            p=p.next.next

def main():
    m=mylist()
    for i in range(1,9):
        m.add(node(i))
    m.printlist(m.top)
    m.reorder()
    m.printlist(m.top)



if __name__=="__main__":
    main()