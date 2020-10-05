nombre=['f005','feea','fef5','fe4e','1ff25','ff25','fef1','f009','1ff27','feec','fef3','fef1','feee','feef','f004','fd7d','fec1','fef2','fee9','fefc']

for x in range(1,1001,1):
    for m in nombre:
        archivo = str(x)+".log"
        busqueda ="PGN:"+ str(m)
        lineas_escribir = []
        with open(archivo, "r") as archivo_lectura:
            numero_linea = 0
            count=0
            for linea in archivo_lectura:
                numero_linea += 1
                linea = linea.rstrip()
                separado = linea.split(" ")
                if busqueda in separado:
                    lineas_escribir.append(str(numero_linea) + " - " + linea)
                    count=count+1
            print("Nombre:"+archivo+"-"+busqueda+"-"+"Cantidad:"+str(count))
