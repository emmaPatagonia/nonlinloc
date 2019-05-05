#!/usr/bin/env python

#Este programa ejecuta SG2K y permite crear archivos .pick  para un conjunto de carpetas 
#de nombre aammdd_hhmm (si es otra la configuracion, ir a las partes comentadas con #CAMBIAR y modificar segun formato).
#Dentro de estas carpetas, que representan distintos eventos, hay archivos MSEED o SAC para distintas estaciones (tambien ir a #CAMBIAR y cambiar si es necesario).
#Setear numero minimo de estaciones a leer por evento en la variable nstamin.

#IMPORTANTE: ESTA MODIFICACION PERMITE OBTENER LA LOCALIZACION A MEDIDA QUE SE LEE CADA CARPETA (EVENTO). TAMBIEN ES POSIBLE CORREGIR ESTACIONES QUE ENTREGUEN 
#UN RMS ALTO. ESTE PROGRAMA FUNCIONA TENIENDO UNA CARPETA LLAMADA "LOC" EN EL MISMO LUGAR DONDE SE ENCUENTRAN LAS CARPETAS CON LOS EVENTOS QUE EN SU INTERIOR
#CONTENGA LAS CARPETAS "TIME" Y "GRIDMODEL" YA GENERADAS CON EL PROGRAMA NONLINLOC (comandos Vel2Grid y Grid2Time), LA CARPETA "FINAL" CREADA Y VACIA Y EL
#ARCHIVO DE CONFIGURACION "NLLOC.IN".

import re,os,time,datetime,glob,subprocess,numpy
from obspy.core import read
from pylab import *
#numero minimo de estaciones que se leeran por evento
nstamin = 3
#yyyy = raw_input('Entrega el agno de los datos:\n')


#Comienza el loop por los directorios

path = os.getcwd()+'/'
loc = path+'LOC'
#CAMBIAR AQUI DEPENDIENDO EL TIPO DE CARPETAS
#for dire in sorted(glob.glob('???????_????/')):
for dire in sorted(glob.glob('????-??-??-????/')):	
        os.chdir(path+dire)
#CAMBIAR AQUI SEGUN EL TIPO DE ARCHIVOS 	
#	listfiles = sorted(glob.glob('*.SAC'))
	listfiles = sorted(glob.glob('????-??-??-????*..H*'))

        sta = [] 
	for file in listfiles:
		st = read(file)
	        tr = st[0]
                stac = tr.stats.station

		sta.append(stac)
	
	uniq = []
	for staelem in sta:
		if staelem not in uniq:
			uniq.append(staelem)
                largo = len(uniq)
	
	if largo >= nstamin:
		print 'Trabajando en el directorio '+dire+'\n'
		print '\nLA ULTIMA ESTACION PARA ESTE EVENTO ES '+uniq[largo-1]+'\n'
		print 'EL NUMERO DE ESTACIONES ES: '+str(largo)+'\n'
		time.sleep(2)
#CAMBIAR AQUI SEGUN EL TIPO DE ARCHIVOS 		
		for stuni in uniq:
			#filez = glob.glob('*'+stuni+'..*HZ*SAC')
			filez = glob.glob('*'+stuni+'..*HZ')
			if not filez:
				filez = ' '
			else:
				filez = filez[0]
			#filen = glob.glob('*'+stuni+'..*HN*SAC')
			filen = glob.glob('*'+stuni+'..*HN')

			if not filen:
				filen = ' '
			else:
				filen = filen[0]
	
			#filee = glob.glob('*'+stuni+'..*HE*SAC')
			filee = glob.glob('*'+stuni+'..*HE')

			if not filee:
				filee = ' '
			else:
				filee = filee[0]
	
			subprocess.call(['java net.alomax.seisgram2k.SeisGram2K -binarytype=SUN_UNIX '+filez+' '+filen+' '+filee+' -pick.file='+dire[0:len(dire)-1]+'.pick'],shell=True)
		#TERMINA EL LOOP
		print '\n\nSE HA LEIDO LA TOTALIDAD DE ESTACIONES DISPONIBLES PARA EL EVENTO '+dire+'\n\n'
		time.sleep(2)

		with open(dire[0:len(dire)-1]+'.pick','r') as fichero:
			arreglo = []
			for lin in fichero:
				arreglo.append(lin)
			if len(arreglo) > 2:
				#MOSTRAR PICKFILE EN PANTALLA
				print '\n\nARCHIVO PEAKS:\n\n'
				cmd = 'more '+dire[0:len(dire)-1]+'.pick'
				os.system(cmd)
				time.sleep(2)
				#COMIENZA LA LOCALIZACION INDIVIDUAL
				print '\n\nCOMIENZO DE LOCALIZACION NLLOC PARA EL EVENTO '+dire+'\n\n'
				time.sleep(2)
				cmd = 'cp '+dire[0:len(dire)-1]+'.pick '+loc
				os.system(cmd)
			        os.chdir(loc)
				cmd = 'mv '+dire[0:len(dire)-1]+'.pick gatherpickfiles.dat'
				os.system(cmd)
				cmd = 'rm '+loc+'/final/*'
				os.system(cmd)

	
				subprocess.call(['NLLoc nlloc.in'],shell=True)
#CAMBIAR "Iquique" SI ES NECESARIO
				os.chdir(loc+'/final')
#CAMBIAR AQUI SEGUN LA CONFIGURACION DE LA CARPETA
				#hypfile = glob.glob('Iquique.'+dire.split('_')[0][0:4]+'*.hyp')[0]
				hypfile = glob.glob('Iquique.'+dire.split('-')[0]+'*.hyp')[0]

				time.sleep(3)
				print '\n\nRESULTADOS DE LA LOCALIZACION:\n\n'
				i = 1
        			with open(hypfile,'r') as hyp:
					infoxsta = []
					for lin in hyp:
					   line = lin.rstrip()	
					   if line:
						if lin.split()[0] == 'GEOGRAPHIC':
							fechahora = lin.split()[2]+' '+ lin.split()[3] +' '+lin.split()[4]+' ' +lin.split()[5] +' ' + lin.split()[6] +' '+lin.split()[7]
							hipo = lin.split()[9]+' '+ lin.split()[11] +' '+lin.split()[13]
						if lin.split()[0] == 'QUALITY':
							rms = lin.split()[8]
						if lin.split()[0] == 'STATISTICS':
							err = str(sqrt(3.53*float(lin.split()[8]))) + ' '+str(sqrt(3.53*float(lin.split()[14]))) + ' '+ str(sqrt(3.53*float(lin.split()[18]))) 
						if i > 16 and len(lin.split()) > 1:
							infoxsta.append(lin.split()[0]+'    '+lin.split()[2]+'    '+lin.split()[4]+'    '+lin.split()[6]+' '+lin.split()[7]+' '+lin.split()[8]+' '+lin.split()[16])
							

						i = i+1	
				print 'yyyy mm dd hh mm ss.ssss    lat   long   depth        RMS          Dx         Dy        Dz'
				print fechahora+' '+hipo+' '+rms+' '+err+'\n'
				print 'sta   comp   fase  yyyymmdd  hhmm  ss  rms'
				for info in infoxsta:
					print info
					


				resp = raw_input('\nCorregir una estacion [si/no]?:\n')
				while resp != 'si' and resp != 'no':
					resp = raw_input('\nIncorrecto, intente otra vez.\nCorregir una estacion [si/no]?:\n')
					if resp == 'si' or resp == 'no':
						break


					
				while resp == 'si':
					corrsta = raw_input('\nEstacion a corregir?:\n')
					while corrsta not in sta:
						corrsta = raw_input('\nIncorrecto. Ingrese nuevamente.\nEstacion a corregir?:\n')

						

#CAMBIAR AQUI SEGUN TIPO DE ARCHIVOS


					os.chdir(path+dire)
					#filez = glob.glob('*'+corrsta+'..*HZ*SAC')

				        filez = glob.glob('*'+corrsta+'..*HZ')

					if not filez:
						filez = ' '
					else:
						filez = filez[0]
					#filen = glob.glob('*'+corrsta+'..*HN*SAC')
					filen = glob.glob('*'+corrsta+'..*HN')

					if not filen:
						filen = ' '
					else:
						filen = filen[0]
			
					#filee = glob.glob('*'+corrsta+'..*HE*SAC')
					filee = glob.glob('*'+corrsta+'..*HE')

					if not filee:
						filee = ' '
					else:
						filee = filee[0]
			
					subprocess.call(['java net.alomax.seisgram2k.SeisGram2K -binarytype=SUN_UNIX '+filez+' '+filen+' '+filee+' -pick.file='+dire[0:len(dire)-1]+'.pick'],shell=True)

					cmd = 'cp '+dire[0:len(dire)-1]+'.pick '+loc
	    	    		      	os.system(cmd)
 				        os.chdir(loc)
					cmd = 'mv '+dire[0:len(dire)-1]+'.pick gatherpickfiles.dat'
					os.system(cmd)
					cmd = 'rm '+loc+'/final/*'
					os.system(cmd)

					subprocess.call(['NLLoc nlloc.in'],shell=True)
					os.chdir(loc+'/final')

					print '\n\nRESULTADOS DE LA LOCALIZACION:\n\n'

					i = 1
        				with open(hypfile,'r') as hyp:
					  infoxsta = []
					  for lin in hyp:
					    line = lin.rstrip()	
					    if line:
						if lin.split()[0] == 'GEOGRAPHIC':
							fechahorap = fechahora
							fechahora = lin.split()[2]+' '+ lin.split()[3] +' '+lin.split()[4]+' ' +lin.split()[5] +' ' + lin.split()[6] +' '+lin.split()[7]
							hipop = hipo
							hipo = lin.split()[9]+' '+ lin.split()[11] +' '+lin.split()[13]
						if lin.split()[0] == 'QUALITY':
							rmsp = rms
							rms = lin.split()[8]
						if lin.split()[0] == 'STATISTICS':
							errp = err
							err = str(sqrt(3.53*float(lin.split()[8]))) + ' '+str(sqrt(3.53*float(lin.split()[14]))) + ' '+ str(sqrt(3.53*float(lin.split()[18]))) 
						if i > 16 and len(lin.split()) > 1:
							infoxsta.append(lin.split()[0]+'    '+lin.split()[2]+'    '+lin.split()[4]+'  '+lin.split()[6]+'  '+lin.split()[7]+'  '+lin.split()[8]+'  '+lin.split()[16])
							

						i = i+1
					print '\nRESULTADO PREVIO'	
					print 'yyyy mm dd hh mm ss.ssss    lat   long   depth        RMS          Dx         Dy        Dz'
					print fechahorap+' '+hipop+' '+rmsp+' '+errp+'\n'
					
					print '\nRESULTADO ACTUAL'
					print 'yyyy mm dd hh mm ss.ssss    lat   long   depth        RMS          Dx         Dy        Dz'
					print fechahora+' '+hipo+' '+rms+' '+err+'\n'
					print 'sta  comp  fase  fecha        hora    rms'
					for info in infoxsta:
						print info


					resp = raw_input('\nCorregir una estacion [si/no]?:\n')
					while resp != 'si' and resp != 'no':
						resp = raw_input('\nIncorrecto, intente otra vez.\nCorregir una estacion [si/no]?:\n')
						if resp == 'si' or resp == 'no':
							break

					
		
			else:
				cmd = 'rm '+dire[0:len(dire)-1]+'.pick'
				os.system(cmd)
				print '\nArchivo pick no tiene lecturas y serA borrado...\n'
				time.sleep(2)
		
			
		
	



