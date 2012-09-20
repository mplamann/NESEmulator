saveslot=8
loadslot=7

function DEC_HEX(IN)
    local B,K,OUT,I,D=16,"0123456789ABCDEF","",0
    while IN>0 do
        I=I+1
        IN,D=math.floor(IN/B),math.mod(IN,B)+1
        OUT=string.sub(K,D,D)..OUT
    end
    return OUT
end

function dectohex(x)

if x==0 then y="0" 
elseif x==1 then y="1"
elseif x==2 then y="2"
elseif x==3 then y="3"
elseif x==4 then y="4"
elseif x==5 then y="5"
elseif x==6 then y="6"
elseif x==7 then y="7"
elseif x==8 then y="8"
elseif x==9 then y="9"
elseif x==10 then y="A"
elseif x==11 then y="B"
elseif x==12 then y="C"
elseif x==13 then y="D"
elseif x==14 then y="E"
elseif x==15 then y="F"
else y="U" end

end

function dectohex2(x,v,i)

y0="0"
d1=math.mod(x,16)
d2=math.mod(math.floor(x/16),16)
d3=math.floor(x/256)

dectohex(d3)
y0=y0..y
dectohex(d2)
y0=y0..y
dectohex(d1)
y0=y0..y

v[i]=y0

end

function dumper(mem1,mem2)


saver = savestate.create(saveslot+1)
loader = savestate.create(loadslot+1)

t={}

diff=mem2-mem1

savestate.load(saver)

for i=0,diff,1 do
	table.insert(t,memory.readbyte(mem1+i))
end

savestate.load(loader)

for i=0,diff,1 do
	memory.writebyte(mem1+i,t[i+1])
end

end

mem1=0x000
mem2=0x800

maxj=math.log(mem2-mem1)/math.log(2)-1

for j=0,maxj,1 do

k=2^(maxj-j)
if k==0.5 then k=1 end

dumper(mem1,mem2)

c1=0

q={[0]=0,0}

while c1==0 do

dectohex2(mem1,q,0)
dectohex2(mem2,q,1)

gui.text(40,40,"Moved all RAM addresses from " .. DEC_HEX(mem1) .. " to " .. DEC_HEX(mem2))
gui.text(40,48,"from slot " .. saveslot .. " to " .. loadslot .. ". Did your")
gui.text(40,56,"chosen RAM address change value?")
gui.text(10,10,"Iteration " .. j+1 .. " of " .. maxj+1)
gui.text(10,18,"Step length " .. k)
gui.text(40,80,"No")
gui.text(140,80,"Yes")
gui.text(40,88,"Select")
gui.text(140,88,"Start")


key1=joypad.read(1)
if key1.start  then t=1 r=1 end
if key1.select then t=1 r=0 end

if t==1 then

if key1.start then c1=0
elseif key1.select then c1=0
else c1=1 end
end

FCEU.frameadvance()
end

--d1==1,start, means yes
--d1==0,select, means no

if r==1 then 
mem2=mem2-k
end

if r==0 then 
mem2=mem2+k
end

end

if r==1 then res=mem2+1 end
if r==0 then res=mem2 end

while true do 

y0=""
d1=math.mod(res,16)
d2=math.mod(math.floor(res/16),16)
d3=math.floor(res/256)

dectohex(d3)
y0=y0..y
dectohex(d2)
y0=y0..y
dectohex(d1)
y0=y0..y

gui.text(20,30,"Success! The RAM address you're looking for is: ")
gui.text(20,38,"0"..DEC_HEX(res))

FCEU.frameadvance()
end