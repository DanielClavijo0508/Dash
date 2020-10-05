for x in range(1, 1001, 1):
    archivo = str(x)+".log"
    busqueda = "PGN:f004"
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
        print("Nombre:"+archivo+"-"+busqueda+"-Cantidad:"+str(count))
