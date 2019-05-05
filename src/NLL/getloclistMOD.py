#Este script se ejecuta en la carpeta "final"
import re,os
cmd = "ls *.hyp > list1.txt"
os.system(cmd)
cmd2 = 'sed \'/last/ d\' list1.txt > list2.txt'
os.system(cmd2)
cmd2b = 'sed \'/sum/ d\' list2.txt > listhyp.txt'
os.system(cmd2b)
f1=open("listhyp.txt","r")
for line in f1:
   leng=len(line)-1
   namefile=line[0:leng]
   cmd3 = 'awk \'/GEOGRAPHIC/{print $3,$4,$5,$6,$7,$8,$10,$12,$14}\' '+namefile+' >> GEO.temp'
   os.system(cmd3)
   cmd4 = 'awk \'/QUALITY/{print $9}\' '+namefile+' >> QUA.temp'
   os.system(cmd4)
   cmd5 = 'awk \'/STATISTICS/{print sqrt($9),sqrt($15),sqrt($19)}\' '+namefile+' >> STA.temp'
   os.system(cmd5)
f1.close()
f2=open('header.txt','a')
f2.write("yyyy mm dd hh mm ss.ssss    lat   long   depth              RMS          Dx         Dy        Dz\n")
f2.close()
cmd6='paste GEO.temp QUA.temp STA.temp > catalogoprev.txt'
os.system(cmd6)
cmd6b='cat header.txt catalogoprev.txt > catalogo.dat'
os.system(cmd6b)
cmd7='rm *.temp *.txt'
os.system(cmd7)
