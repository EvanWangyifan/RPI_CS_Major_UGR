import sys  # For sys.argv, sys.exit()
import socket  # for gethostbyname()
import math
import threading
import ast
from hw4_client import data_message

class Node(object):
    # A Node could be a base station or a sensor 
    def __init__(self, info:str):
        info = info.split(" ")
        self.id = info[1]
        self.pos = [float(info[3]), float(info[4])]
        self.posX = float(info[3])
        self.posY = float(info[4])
    
    def get_distance(self, pos):
        # Calculate the distance between self and pos(x, y)
        return math.sqrt((self.posX - pos[0])**2 + (self.posY - pos[1])**2)

class Sensor_Node(Node):
    def __init__(self, info:str, sock):
        # Senor have its own socket
        super().__init__(info)
        # print(info)
        self.sock = sock
        info = info.split(" ")
        self.range = float(info[2])
    
    def in_range(self, pos):
        # Check if pos(x,y) is inside the range of a sensor node
        return self.get_distance(pos) <= self.range

    def update_pos(self, new_x, new_y):
        # Update the position
        self.pos = [new_x, new_y]
        self.posX = new_x
        self.posY = new_y

    def __str__(self) -> str:
        return "{} {} {}".format(self.id, self.pos, self.range)

class Base(Node):
    def __init__(self, info:str):
        self.id = info[0]
        self.pos = [float(info[1]), float(info[2])]
        self.posX = self.pos[0]
        self.posY = self.pos[1]
        self.num_links = int(info[3])
        self.link_list = []
    
    def __str__(self) -> str:
        return "{} {} {} {}".format(self.id, self.pos, self.num_links, self.link_list)


class Controller:
    def __init__(self, all_bases, port):
        self.bases = all_bases
        # in_sock will be bind to the given port and waiting for connections from sensor
        self.in_sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.in_sock.bind((socket.gethostname(), port))
        self.all_sock = {}
        self.all_sensor = {}
        threading.Thread(target=self.connection_handler, daemon = True).start()

    def connection_handler(self):
        # Accept incoming connection
        self.in_sock.listen(10)
        while True:
            new_sock, addr = self.in_sock.accept()
            # Wait for the update message
            update = new_sock.recv(1024).decode()
            # Use the update msg to create the Sensor_Node object
            tmp_s = Sensor_Node(update, new_sock)
            # Store the Sensor_Node object and its socket FD
            self.all_sock[tmp_s.id] = new_sock
            self.all_sensor[tmp_s.id] = tmp_s
            m = self.reachable(tmp_s)
            # Send reachable to the sensor
            new_sock.send(m.encode())
            threading.Thread(target=self.message_handler, args=(tmp_s, new_sock), daemon = True).start()

    def there_msg(self, id):
        # print("THERE msg ID : " + id)
        if id in self.bases:
            # Find the target base station
            target = self.bases[id]
        elif id in self.all_sensor:
            # Find the target sensor node
            target = self.all_sensor[id]
        return "THERE {} {} {}".format(id, target.posX, target.posY)

    def message_handler(self, n_sensor:Sensor_Node, n_sock:socket):
        # Waiting for incoming messages
        while True:
            msg = n_sock.recv(1024).decode()
            msg = msg.split(" ")
            if msg[0] == 'WHERE':
                # If receive WHERE message, send THERE message as response
                response = self.there_msg(msg[1])
                n_sock.send(response.encode())
            if msg[0] == "UPDATEPOSITION":
                # Use the UPDATE message to update the Sensor_Node object
                n_sensor.update_pos(float(msg[3]), float(msg[4]))
                m = self.reachable(n_sensor)
                n_sock.send(m.encode())
            if msg[0] == "DATAMESSAGE":
                self.relay_msg(msg)

    def relay_msg(self, msg):
        # Relay the message to the correct recipient
        hop_list = ' '.join(msg[5:])
        hop_list = ast.literal_eval(hop_list)
        dest = msg[3]
        d_node = None
        # Find the destination
        if dest in self.bases:
            d_node = self.bases[dest]
        else:
            d_node = self.all_sensor[dest]
        if msg[2] in self.all_sensor:
            # Relay message to sensor
            r_msg = data_message(msg[1], msg[2], msg[3], hop_list)
            self.all_sock[msg[2]].send(r_msg.encode())
        elif msg[2] in self.bases:
            # Relay message to base station
            next = msg[2]
            while next in self.bases:
                if next == dest:
                    # Message delivered to the correct recipient
                    print("{}: Message from {} to {} successfully received.".format(next, msg[1], dest))
                    return
                tmp = ""
                min_dist = 1000
                n_pos = self.bases[next].pos
                # Find the Node that closest to destination
                for i in self.bases[next].link_list:
                    if i not in hop_list:
                        dist = d_node.get_distance(self.bases[i].pos)
                        # Tie breaker
                        if math.isclose(min_dist, dist):
                            tmp = min(tmp, i)
                        elif dist < min_dist:
                            min_dist = dist
                            tmp = i
                for s in self.all_sensor:
                    if s not in hop_list and self.all_sensor[s].in_range(n_pos):
                        dist = d_node.get_distance(self.all_sensor[s].pos)
                        # Tie breaker
                        if math.isclose(min_dist, dist):
                            tmp = min(tmp, s)
                        elif dist < min_dist:
                            min_dist = dist
                            tmp = s

                if tmp in self.bases:
                    if next == dest:
                        # Message delivered to the correct recipient
                        print("{}: Message from {} to {} successfully received.".format(next, msg[1], dest))
                        return
                    else:
                        # Forward the message to next base station
                        print("{}: Message from {} to {} being forwarded through {}".format(next, msg[1], dest, next))
                        hop_list.append(next)
                        next = tmp
                elif tmp in self.all_sensor:
                    # Forward the message to the sensor
                    print("{}: Message from {} to {} being forwarded through {}".format(next, msg[1], dest, next))
                    hop_list.append(next)
                    r_msg = data_message(msg[1], tmp, msg[3], hop_list)
                    self.all_sock[tmp].send(r_msg.encode())
                    return
                else:
                    # Destination is not reachable
                    print("{}: Message from {} to {} being forwarded through {}".format(next, msg[1], dest, next))
                    print("{}: Message from {} to {} could not be delivered.".format(next, msg[1], dest))
                    break



    def reachable(self, sensor: Sensor_Node):
        # Return all reachable stations and sensors
        ret = []
        for i in self.bases:
            b = self.bases[i]
            if sensor.in_range(b.pos):
                ret.append([b.id, b.posX, b.posY])

        for i in self.all_sensor:
            s = self.all_sensor[i]
            if s.id != sensor.id and sensor.in_range(s.pos) and s.in_range(sensor.pos):
                ret.append([s.id, s.posX, s.posY])
        
        message = "REACHABLE {} {}".format(len(ret), ret)
        return message
        


def read_base(file):
    # Read txt file into a Diction of Diction
    # Dict[base_id] = Base object (pos: (posX, posY), numlink: (number of links), link_list: (list of strings(base_id)))
    # First read the file into a single string
    lines = file.readlines()
    base_dict = {} # Key type: (string) Value type: (Base)
    for line in lines:
        # Split the string and remove the '\n'
        line = line.split(" ")
        line[-1] = line[-1][0:-1]
        tmp = Base(line)
        for i in range(4, len(line)):
            tmp.link_list.append(line[i])
        base_dict[line[0]] = tmp
        # print(tmp)
    return base_dict


if __name__ == '__main__':
    #Frist read the command line argument
    c_port = int(sys.argv[1])
    base_station_file = open(sys.argv[2])
    # Read the base stations file
    base_dict = read_base(base_station_file)
    control_server = Controller(base_dict, c_port)
    # Wait for user input
    while(True):
        user_cmd = input()

        if user_cmd == "QUIT":
			# TODO: clean up any memory and any sockets that are in use, and then terminate.
            control_server.in_sock.close()
            for i in control_server.all_sock:
                control_server.all_sock[i].close()
            break



