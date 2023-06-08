from guizero import App, Text, Combo, PushButton, info, TextBox,ButtonGroup
from smbus import SMBus
from time import sleep
from firebase import firebase
addr = 0x8
bus = SMBus(1)
ids=["13322865231"]
def tobytes(wrd):
  for c in wrd:
    bus.write_byte(addr,ord(c))

def dets(wrd):
  tobytes(wrd)
  if(wrd=="id"):
    x=11
  elif(wrd=="SOC"):
    x=5
    sleep(10)
  elif(wrd=="START"):
    x=8
  elif(wrd=="STOP"):
    x=7
  sleep(3)
  resp="".join(map(chr, bus.read_i2c_block_data(addr,0,x)))
  return resp

def done():
  stopped=dets("STOP")
  print(stopped)
  print("SOC:"+"{}".format(r)+"%")
  info("Completed", " Charged to {}".format(fs.value)+"%")
  fb_id=firebase.FirebaseApplication('https://caas-soc.firebaseio.com/')
  result=fb_id.post("https://caas-soc.firebaseio.com/{}".format(id),{"Charge Consumed:":float(fs.value)-float(soc_init)})
  print(result)
  quit()
def check():
  global final
  global  r
  final=fs.value
  r=float(dets("SOC"))
  print("{}".format(r)+"%")
  if(r<float(fs.value)):
    dets("START")
    print("CHARGING")
    while(r<float(fs.value)):
      r=float(dets("SOC"))
      print(r)
  elif(r>=float(fs.value)):
      info("STATUS","Already charged upto or more than {}".format(fs.value)+"%")
  elif(r>=100):
      info("STATUS", "Charge Full")
  done()

def get_charge(id,soc):
  global soc_final
  global charge
  global fs
  app1.hide()
  app.show()
  model = Text(app, text="ID:"+id, grid=[1, 0], align="left")
  charge=Text(app, text="State of charge: "+"{}".format(float(soc))+"%",grid=[1,1], align="left")
  final=Text(app,text="Desired final SOC:",grid=[1,2],align="left")
  fs=TextBox(app,grid=[2,2],align="left")
  ch = PushButton(app,command=check, text="Charge", grid=[1,3])

def do_check():
  if user_in.value == "caas":
    if pas_in.value == "caas":
      info("LOGIN","Successfully logged in")
      get_charge(id,soc_init)
id="NA"
while(id not in ids):
  id=dets("id")
  print(id)
soc_init=float(dets("SOC"))
app1 = App(title="Coimbatore Charging Station")
user = Text (app1,text="Enter your Username")
user_in = TextBox(app1)
pas = Text(app1,text="Enter your Password")
pas_in = TextBox(app1)
login = PushButton(app1,command = do_check,text="LOGIN")
app = App(title="Coimbatore Charging Station", layout="grid")
app.hide()
app1.display()





