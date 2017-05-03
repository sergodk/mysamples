# Напишите функцию, которая перебирает натуральные числа от 1 до N включительно и раскладывает каждое число на простые множители.
# Результат можно выводить на экран либо копить в любой структуре данных.

# Рекурсивный поиск простых множителей числа
def simpledivs(n,k):
    if k>(n//2):
        print(n)
    elif (n%k)==0:
        print(k)
        simpledivs(n//k,k)
    else:
        k=k+1
        simpledivs(n,k)
# Перебор натуральных чисел и вывод простых их множителей
def TestSimple(N):
    i=1;
    while i<=N:
        print('Number:',i,'dividers')
        simpledivs(i,2)
        i=i+1

def main():
    TestSimple(50)

if __name__=='__main__':
    main()