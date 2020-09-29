import threading, time
import redis
datos=[]
def lectura():
    a=0
    while 1:
        global datos
        nombre=['f005','feea','fef5','fe4e','1ff25','ff25','fef1','f009','1ff27','feec','fef3','fef1','feee','feef','f004','fd7d','fec1','fef2','fee9','fefc']
        for x in nombre:
            f = open (x+'.txt','r')
            pgn = f.readline()
            pgn = pgn.strip('\n')
            data = f.readline()
            data = data.strip('\n')
            fecha = f.readline()
            datos.append(pgn+"-"+fecha+"-"+data)
            print("PGN:"+pgn+"Fecha:"+fecha+" Data:"+data)
            print(datos[a])
            print(a)
            f.close()
            a=a+1
 

#db = Redis(db=10)
db=redis.StrictRedis('127.0.0.1', 6379, db=10, charset="utf-8", decode_responses=True)

def envio():
    b = 0
#    global datos
    #while b<10:
    for row in datos:
        pgn,data,fecha = row.split("-")
        #pgn = row.split("-")[1]
        #print("hola"+pgn+data)
        o = {'PGN':pgn ,
             'T':fecha,
             'Data':json.dumps(data)
             }
        db.set(pgn,json.dumps(o))
        #b=b+1
        time.sleep(0.2)

#lectura()
#contar2()

hilo1 = threading.Thread(target=lectura)
hilo2 = threading.Thread(target=envio)
hilo1.start()
#time.sleep(0.1)
hilo2.start()
