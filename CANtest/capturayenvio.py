import json
import threading, time
import redis
datos=[]
def lectura():
    a=0
    while 1:
        global datos
        nombre=['f005','feea','fef5','fe4e','1ff25','ff25','fef1','f009','1ff27','feec','fef3','fef1','feee','feef','f004','fd7d','fec1','fef2','fee9','fefc']
        for x in range(20):
            if x==0:
                f = open ('f005.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==1:
                f = open ('feea.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==2:
                f = open ('fef5.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==3:
                f = open ('fe4e.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==4:
                f = open ('1ff25.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==5:
                f = open ('ff25.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==6:
                f = open ('f009.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==7:
                f = open ('fef1.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==8:
                f = open ('1ff27.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==9:
                f = open ('feec.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==10:
                f = open ('fef3.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==11:
                f = open ('fef1.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==12:
                f = open ('feee.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==13:
                f = open ('feef.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==14:
                f = open ('f004.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==15:
                f = open ('fd7d.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==16:
                f = open ('fec1.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==17:
                f = open ('fef2.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==18:
                f = open ('fee9.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
                print(datos[a])
                print(a)
                f.close()
                a=a+1
            if x==19:
                f = open ('fefc.txt','r')
                pgn = f.readline()
                pgn = pgn.strip('\n')
                data = f.readline()
                data = data.strip('\n')
                fecha = f.readline()
                fecha = fecha.strip('\n')
                datos.append(pgn+"-"+data+"-"+fecha)
                print("PGN:"+pgn+" Data:"+data)
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
             'Data':data
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

