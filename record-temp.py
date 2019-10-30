#!/usr/bin/env python3
import bluetooth
from time import sleep
from datetime import datetime
s = None

def connect():
  print("in connect")
  global s

  if s:
    print("if s")
    try:
      if s.send('1') <= 0:
        print("send not work")
        s.close()
        s = None
        print("done close")
      else:
        print("returning active s")
        return s
    except Exception:
        s = None
        print("getting new s due to exc")

  bd_addr = '00:14:03:06:1B:51'
  port = 1

  while not s:
    sleep(1)
    try:
      print("getting socket")
      s = bluetooth.BluetoothSocket( bluetooth.RFCOMM )
      print("trying to connect")
      s.settimeout(60*5+1)
      s.connect((bd_addr, port))
    except Exception:
      print("error connect, try again")
      pass
  print('Connected')
  return s

tempInfo = []
ready = False

with open("temp.csv", "wb") as fh:
  sock = connect()
  while sock:
    data = b""
    print("gettin data")
    try:
      data = sock.recv(1000)
    except Exception:
      print("reconnecting")
      sock = connect()
      pass

    if ready:
      tempInfo.append(data)

    if b"\r\n" in data and ready:
      ti = b"".join(tempInfo).replace(b"\r\n", b"," + bytes(str(datetime.now()), 'utf8')) + b"\n"
      fh.write(ti)
      fh.flush()
      tempInfo = []

    if b"\r\n" in data and not ready:
      ready = True

sock.close()
