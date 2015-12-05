from random import randint as ri;import re
class Fighter():
    def __init__(self,n,hit_points=10):self.n=n;self.hit_points=hit_points
    def __repr__(self):return self.n+" (HP: "+str(self.hit_points)+")"
    def take_damage(self,dmg):self.hit_points-=dmg;print("\tAlas,",self.n, "has fallen!" if self.hit_points<=0 else "\t"+self.n,"has",self.hit_points,"hit points remaining.")
    def atk(self,other_fighter):
        print(self.n,"atks",other_fighter.n+"!")
        if ri(0,20)>= 12:dmg = ri(1,6);print("\tHits for",dmg, "hit points!");other_fighter.take_damage(dmg)
        else:print("\tMisses")
def combat_round(fone,ftwo,fone_c=ri(1,6),ftwo_c=ri(1,6)):
    if fone_c==ftwo_c:print("Simultaneous!");fone.atk(ftwo);ftwo.atk(fone)
    elif fone_c>ftwo_c:fone.atk(ftwo)
    else:ftwo.atk(fone)
fone,ftwo,cr=Fighter("Rich"),Fighter("Thompson"),1
while fone.hit_points>0 and ftwo.hit_points>0:print((" Round "+str(cr)+" ").center(35,"="),"\n",fone,"\n",ftwo);combat_round(fone,ftwo);cr+=1+int(0*int(re.sub("\D","",input("Enter to fight!\n"))+str(1)))
print("The battle is over!\n"+str(fone)+"\n"+str(ftwo))
