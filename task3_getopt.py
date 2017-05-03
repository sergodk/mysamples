# Напишите простую реализацию функции для разбора параметров командной строки (getopt).
# Полагаем, что нам требуется структура данных, содержащая заранее известный допустимый набор параметров различного типа - строки, целые, числа с плавающей точкой и т. д.,
# а также признак, является ли этот параметр обязательным. Полагаем, что параметры могут передаваться только в "длинном" формате с обязательным "--" перед именем параметра
# (то есть "--my-option-name my-option-val", "--my-option-name=my-option-val", "--my-boolean-option"). Параметров может быть несколько. Функция должна обязательно уметь
# обрабатывать параметр "--help" (если он указан в списке параметров при вызове функции), выводящий пример использования (необязательные параметры должны выводиться
# в квадратных скобках).


# Класс заглушка для наполнения
class DummyForArgs:
    def __init__(self):
        # -66666 значение по умолчанию (не валидное для параметров типа integer)
        self.integer1=int(-66666)
        self.integer2=int(-66666)
        self.float1=float("NaN")
        self.flag1=False
        self.flag2=False

    def __str__(self):
        return "Int1="+self.integer1.__str__()+" Int2="+self.integer2.__str__()+" Float1="+self.float1.__str__()+" Flag1="+self.flag1.__str__()+" Flag2="+self.flag2.__str__()

# объект заглушка
dummy=DummyForArgs();

# --------------------------------------------функция, проверяющая наличие значения для опции в списке аргументов args на позиции pos-------------------------------------------
def checkvalue(argslist, pos):
    # если аргументы кончились в командной строке
    if pos == len(argslist) - 1:
        print("Error: No value for", argslist[pos])
        return False
    else:
        value = argslist[pos + 1]
        # если следующий аргумент это опция (начинается с --)
        if value.find("--") == 0:
            print("Error: No value for", argslist[pos])
            return False
        else:
            return  True
# --------------------------------------------функция, проверяющая наличие опций и их значений в списке аргументов args на позиции pos-------------------------------------------
def checkargs(argslist, pos):
    stroption=argslist[pos]
    if stroption.find("--")==0:
        if stroption=="--inputfilename":
            if checkvalue(argslist,pos):
                value = argslist[pos + 1]
                #print("Option:",argslist[pos],"Value:",value)
                return pos+2
            else:
                return pos+1
        elif stroption == "--outputfilename":
            if checkvalue(argslist,pos):
                value = argslist[pos + 1]
                #print("Option:",argslist[pos],"Value:",value)
                return pos+2
            else:
                return pos+1
        elif stroption == "--myintoption1":
            if checkvalue(argslist,pos):
                value = argslist[pos + 1]
                #print("Option:",argslist[pos],"Value:",value)
                try:
                    dummy.integer1=int(value)
                except:
                    print("Error: Option",argslist[pos],"need valid integer value. Use --help")
                return pos+2
            else:
                return pos+1
        elif stroption == "--myintoption2":
            if checkvalue(argslist,pos):
                value = argslist[pos + 1]
                #print("Option:",argslist[pos],"Value:",value)
                try:
                    dummy.integer2=int(value)
                except:
                    print("Error: Option",argslist[pos],"need valid integer value. Use --help")
                return pos+2
            else:
                return pos+1
        elif stroption == "--myfloatoption1":
            if checkvalue(argslist,pos):
                value = argslist[pos + 1]
                #print("Option:",argslist[pos],"Value:",value)
                try:
                    dummy.float1=float(value)
                except:
                    print("Error: Option",argslist[pos],"need valid float value. Use --help")
                return pos+2
            else:
                return pos+1
        elif stroption == "--help":
            print("Usage: --myintoption1=value1 --myintoption2=value2 --myfloatoption1=value3 [--inputfilename=filename1 --outputfilename=filename2 --myflag1on --myflag1off --myflag2on --myflag2off]")
            print("--myintoption1,--myintoption2 - Setup integer1 and integer2 parameter of dummy object. Values must not be equals -66666 ")
            print("--myfloatoption1 - Setup float1 parameter of dummy object")
            print("--inputfilename, --outputfilename - Setup input or output filename (No real files used, demonstration only)")
            print("--myflag1on --myflag1off --myflag2on --myflag2off - Turn on/off flag1 or flag2 parameters of dummy object")
            print("--help - Print this help")
            return pos+1
        elif stroption == "--myflag1on":
            #print("Option: MyFlag1 set On")
            dummy.flag1=True
            return pos+1
        elif stroption == "--myflag1off":
            #print("Option: MyFlag1 set Off")
            dummy.flag1 = False
            return pos+1
        elif stroption == "--myflag2on":
            #print("Option: MyFlag2 set On")
            dummy.flag2 = True
            return pos+1
        elif stroption == "--myflag2off":
            #print("Option: MyFlag2 set Off")
            dummy.flag2 = False
            return pos+1
        else:
            print("Unknown option:",argslist[pos])
            return pos+1
    else:
        print("Unknown arg:", argslist[pos])
        return pos+1

#-----------------------------------------------Функция принимающая и обрабатывающая аргументы в виде одной строки----------------------------------------
def GetOpt(argstr):
    argstr=str.replace(argstr,"="," ")
    argslist=str.split(argstr)
    #print(argslist)
    #если указан параметр --help то просто выводим справку (остальные параметры не учитываем)
    if "--help" in argslist:
        checkargs(["--help"], 0)
    else:
        # проверка наличия в списке ключей обязательных опций
        if "--myintoption1"in argslist and "--myintoption2" in argslist and "--myfloatoption1" in argslist:
            i = 0
            while i<len(argslist):
                i=checkargs(argslist,i)
            # проверка заполненности полей объекта заглушки
            if dummy.integer1!=-66666 and dummy.integer2!=-66666 and dummy.float1==dummy.float1:
                print("Dummy object is OK:",dummy)
            else:
                print("Error: Dummy object parameters values are invalid. Use --help")
        else:
            print("Error: Options missed. Use --help")

def main():


    #GetOpt("--inputfilename input.txt --myintoption1=4 --myfloatoption1 3.456 --myflag1on --myflag2off --outputfilename=output.txt")
    #GetOpt("--inputfilename --myintoption1=400 --myfloatoption1 3.456 --myflag1on --myflag2off --outputfilename= --myintoption2=10")
    #GetOpt("")
    GetOpt("--inputfilename input.txt --myintoption1=4 --myfloatoption1 3.456 --myflag1on --myflag2off --outputfilename=output.txt --myintoption2=10")




if __name__=='__main__':
    main()