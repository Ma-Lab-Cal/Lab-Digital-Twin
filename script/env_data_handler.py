import datetime
import sqlite3
import json
import time

import serial

ENV_DB_PATH = "C:\\Users\\ma-la\\Documents\\Digital-Twin-Inventory-System\\data\\environmental_data.db"
COM_PORT = "COM13"

class DataBase:
    def __init__(self, path):
        self.path = path
        
        self._db = sqlite3.connect(self.path, check_same_thread=False)

    def createTable(self, table_name, structure):
        self.table_name = table_name
        self.keys = [entry[0] for entry in structure]
        self.types = [entry[1] for entry in structure]
        
        exec_str = "CREATE TABLE IF NOT EXISTS "
        exec_str += table_name
        exec_str += "("

        entries = []
        for i, key in enumerate(self.keys):
            entries.append("{0} {1}".format(key, self.types[i]))
        exec_str += ",".join(entries)
        
        exec_str += ");"

        self._db.execute(exec_str)

    def insert(self, entry):
        assert len(entry) == len(self.keys)

        entry = ["\"{0}\"".format(elem) if type(elem) == str else str(elem) for elem in entry]

        exec_str = "INSERT INTO EnvironmentalData"
        exec_str += "({0})".format(",".join(self.keys))
        exec_str += "VALUES ("
        exec_str += ",".join(entry)
        exec_str += ");"
        
        cur = self._db.cursor()
        try:
            cur.execute(exec_str)
        except sqlite3.IntegrityError:
            #seldomly we can have timestamp collision
            return
        self._db.commit()

    
    def getData(self, start_time, end_time):
        cur = self._db.cursor()
        cur.execute("""SELECT * FROM EnvironmentalData WHERE timestamp BETWEEN {start_time} AND {end_time};""".format(start_time=start_time, end_time=end_time))
        data = cur.fetchall()
        processed_data = []
        for entry in data:
            processed_data.append(dict(zip(self.keys, entry)))
        
        return processed_data


def openSerialPort():
    print("connecting to COM4")
    ser = None
    while not ser:
        try:
            ser = serial.Serial(COM_PORT, baudrate=115200)
        except serial.serialutil.SerialException:
            print("cannot open {0}, waiting...".format("COM4"))
            time.sleep(5)
            
    print("connected")
    return ser

db = DataBase(ENV_DB_PATH)

structure = [
    ["timestamp",   "DATETIME PRIMARY KEY"],
    ["node",        "TINYTEXT NOT NULL"],
    ["temperature", "DOUBLE"], 
    ["humidity",    "DOUBLE"],
    ["num_particles_0_3_um",    "DOUBLE"],
    ["num_particles_1_um",      "DOUBLE"],
    ["num_particles_10_um",     "DOUBLE"],
    ["magnetic_x",  "DOUBLE"],
    ["magnetic_y",  "DOUBLE"],
    ["magnetic_z",  "DOUBLE"],
    ["lux",         "DOUBLE"],
    ]

db.createTable("EnvironmentalData", structure)

ser = openSerialPort()

prev_time = time.time()

while True:
    try:
        buffer = ser.readline()
    except:
        ser = openSerialPort()
        continue
    
    data = buffer.decode()
    print(data)
    try:
        data = json.loads(data)
    except:
        continue

    if len(data) != len(db.keys) - 1:
        continue
    
    timestamp = str(time.time())

    data.insert(0, timestamp)  # add timestamp
    
    #print("log data", data[1])
    db.insert(data)

    if (time.time() - prev_time) > 120:
        print("{0}\tenv data handler still running...".format(time.time()))
        prev_time = time.time()
        
