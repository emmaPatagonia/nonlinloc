#Permite juntar todos los archivos *.pick que contienen las fases marcadas para cada evento en uno solo con el formato que pide NonLinLoc. El nombre de tales archivos debe ser el nombre del directorio.

import re,os
#year = raw_input('Ingrese agno de los datos:\n')
#cmd = 'ls -d '+year[2:4]+'* > listdir.txt'
#os.system(cmd)
pwd=os.getcwd()
#f1=open("listdir.txt","r")
for line in glob.glob('????-??-??-????/'):
#   leng=len(line)-1
   namedir=line.split('/')[0]
   os.chdir(pwd+'/'+namedir)
   cmd = 'cp '+namedir+'.pick '+namedir+'.temp'
   os.system(cmd)
   cmd1 = 'echo \" \" >> '+namedir+'.temp | mv '+namedir+'.temp temp'+namedir
   os.system(cmd1)
   cmd2 = 'mv temp'+namedir+' '+pwd
   os.system(cmd2)
#f1.close() 
os.chdir(pwd)
cmd4='cat temp* > gatherpickfiles.dat'
os.system(cmd4)
cmd5='rm temp*'
os.system(cmd5)
