import threading, time
datos=[]
def lectura():
    a=0
    while 1:
        f = open ('PGN.txt','r')
        global datos
        pgn = f.readline()
        pgn = pgn.strip('\n')
        data = f.readline()
        datos.append(pgn+"-"+data)
        print("PGN:"+pgn+" Data:"+data)
        print(datos[a])
        print(a)
        f.close()
        a=a+1
#        time.sleep(0.1)

def envio():
    b = 0
    global datos
    while 1:
        print("TTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTTT"+datos[b])
        print(b)
        b=b+1
        time.sleep(0.1)

#lectura()
#contar2()

hilo1 = threading.Thread(target=lectura)
hilo2 = threading.Thread(target=envio)
hilo1.start()
hilo2.start()

