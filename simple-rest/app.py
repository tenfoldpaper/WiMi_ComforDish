from flask import Flask, jsonify, request, abort, render_template
from flask_restful import Api, Resource, reqparse
from flask_cors import CORS, cross_origin
from flask_socketio import SocketIO, emit
import sys
from time import sleep
import serial

### Flask setups ###
app = Flask(__name__)
app.config['CORS_HEADERS'] = 'Content-Type'
app.config['SECRET_KEY'] = 'secret!'
cors = CORS(app)
socketio = SocketIO(app, cors_allowed_origins="*")
clients = []

### Arduino setups ###
ardSerial = serial.Serial('/dev/ttyACM0', 19200)
ardSerial.close()
ardSerial.open()
print("Arduino ready?")
### Global variables ###
command = {'id': 0, 'message': {
    'on':2,
    'ud':3,
    'pp':4
}}
status_dict = {'time_remaining': 0, 'status': 0, 'elevation': 0}
occupied = False

### Device setups ###
status = "OFF"

# Set StepMotor Driver
elevUpperLimit = 99999999
elevLowerLimit = -99999999
fullyClosed = 0
fullyOpen = 5000
topRack = 2500
programTime = [0, 5, 10, 15]

def emit_status():
    socketio.emit('device_status', {{'elevation': status_dict['elevation'], 'time_remaining': status_dict['time_remaining']}})

@socketio.on('connect')
def connect():
    print('React client connected', file=sys.stderr)


@app.route("/")
def index():
    return render_template('index.html',
        message=command['message'],
        status=status_dict['status'],
        elev=status_dict['elevation'],
        trem=status_dict['time_remaining'])

@app.route('/api/controls', methods=['POST'])
@cross_origin()
def post_command():
    global command
    global programTime
    global status_dict
    global count
    global occupied
    print("Stuff in buffer:")
    print(ardSerial.in_waiting)
    while ardSerial.in_waiting:
        print(ardSerial.read())
    if not request.json:
        print("Not a json request", file=sys.stderr)
        abort(400)
    command = {
        'id': request.json['id'],
        'message': request.json['message']
    }
    if occupied:
        print("OCCUPIED")
        return jsonify({'status':status_dict}), 202
    print(command['message'], file=sys.stderr)
    if(command['message']['on'] == 1):
        status_dict['status'] = 1
        
    else:
        status_dict['status'] = 0
        ardSerial.write(b'-')
        print(ardSerial.read())
        return jsonify({'status':status_dict}), 201

    if(command['message']['pg'] != 4):
        print('program command received', file=sys.stderr)
        socketio.emit('device_log', {'message':'received PG command'})
        if(occupied):
            return jsonify({'status': status_dict}), 201

        if(not occupied):
            comm = command['message']['pg']
            if(comm == 1):
                print('pgcomm = 1')
                ardSerial.write(b'0')
                if(ardSerial.read() == b'A'):
                    print('Program 1 initiated')
                    occupied = True
                    counter = 0
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'D'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                        sleep(1)
                    occupied = False 
                    print('Program complete')
            elif(comm == 2):
                print('pgcomm = 2')
                ardSerial.write(b'1')
                counter = 0
                if(ardSerial.read() == b'B'):
                    print('Program 2 initiated')
                    occupied = True
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'E'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                    occupied = False
                    print('Program complete')
            elif(comm == 3):
                print('pgcomm = 3')
                ardSerial.write(b'2')
                counter = 0
                if(ardSerial.read() == b'C'):
                    print('Program 3 initiated')
                    occupied = True
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'F'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                    occupied = False
                    print('Program complete')
            return jsonify({'status': status_dict}), 201


    elif(command['message']['ud'] != 4):
        comm = command['message']['ud']
        print('updown command received', file=sys.stderr)
        socketio.emit('device_log', {'message':'received UD command'})

        if(occupied):
            sleep(0.2) #Give some time for the device lock to be undone
        #socketio.emit('device_status', {'elevation': 10, 'time_remaining':10})
        if(not occupied): # Lock the variable
            occupied = True
            if(comm == 1):
                print('udcomm = 1')
                ardSerial.write(b'3')
                print(ardSerial.read())
                occupied = False
            elif(comm == 2):
                print('udcomm = 2')
                ardSerial.write(b'4')
                print(ardSerial.read())
                occupied = False
            elif(comm == 3):
                print('udcomm = 3')
                ardSerial.write(b'5')
                print(ardSerial.read())
                occupied = False

            return jsonify({'status': status_dict}), 201

    elif (command['message']['pp'] != 4 and not occupied):
        print('predef command received', file=sys.stderr)
        socketio.emit('device_log', {'message':'received PP command'})
        if(occupied):
            sleep(0.2)
            if(occupied): # Occupied flag hasn't been cleared, so just return
                print('Flag has not been cleared', file=sys.stderr)
                return jsonify({'status': status_dict}), 201

        if(not occupied):
            occupied = True
            if(command['message']['pp'] == 1): #Fully Open
                print('ppcomm = 1')
                ardSerial.write(b'6')
                if(ardSerial.read() == b'\x9a'):
                    print('Fully opening')
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'T'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                    occupied = False
                    print('Fully opened')

            elif(command['message']['pp'] == 2): #Fully Closed
                print('ppcomm = 2')
                ardSerial.write(b'7')
                if(ardSerial.read() == b'\x9b'):
                    print('Fully closing')
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'T'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                    occupied = False
                    print('Fully closed')

            elif(command['message']['pp'] == 3): #Top Rack
                print('ppcomm = 3')
                ardSerial.write(b'8')
                if(ardSerial.read() == b'\x9c'):
                    print('Half opening')
                    check = ardSerial.read()
                    while(ardSerial.in_waiting and check != b'T'):
                        check = ardSerial.read()
                        counter += 1
                        print(counter)
                    occupied = False
                    print('Half closed')
    else:
        print('no comm')
        ardSerial.write(b'9')
        print(ardSerial.read())
        occupied = False
    return jsonify({'status': status_dict}), 201

@app.route('/api/status', methods=['GET'])
@cross_origin()
def get_command():
    global status_dict
    return jsonify({'status': status_dict}), 201

if __name__ == '__main__':
    socketio.run(app, host='0.0.0.0', debug=True)
