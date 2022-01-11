import sys  # For sys.argv, sys.exit()
import socket  # for gethostbyname()
import math
import threading
import ast
import time
import select

def data_message(origin, next, dest, hoplist):
    # Generate the DATAMESSAGE
    return "DATAMESSAGE {} {} {} {} {}".format(origin, next, dest, len(hoplist), hoplist)

def get_distance(n1, n2):
    # Find the distance between 2 points
    return math.sqrt((n1[0] - n2[0])**2 + (n1[1] - n2[1])**2)

class Sensor:
    def __init__(self, c_addr, c_port, id, range, x, y) -> None:
        # A Sensor will use its own socket to communicate with the control server
        self.c_port = c_port
        self.id = id
        self.range = range
        self.posX = x
        self.posY = y
        self.pos = [x, y]
        self.sock = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.addr = socket.gethostbyname(c_addr)
        # Record all reachable nodes
        self.reachable_list = {}
        # Record all known nodes
        self.known = {}
        self.sock.connect((self.addr, self.c_port))
        self.sock.send(self.update_msg().encode())
        self.on_recv(self.sock.recv(1024).decode())

    def on_recv(self, msg):
        # Response to the incomming message
        msg = msg.split(' ')
        if msg[0] == "REACHABLE":
            # Update the reachable list
            num_node = int(msg[1])
            reachable_nodes = ' '.join(msg[2:])
            node_list = ast.literal_eval(reachable_nodes)
            self.reachable_list = {}
            for i in node_list:
                self.reachable_list[i[0]] = (float(i[1]), float(i[2]))
            out_put = "{}: After reading REACHABLE message, I can see: {}".format(self.id, sorted(self.reachable_list.keys()))
            print(out_put)

        if msg[0] == "THERE":
            # Update the known list
            self.known[msg[1]] = (float(msg[2]), float(msg[3]))

        if msg[0] == "DATAMESSAGE":
            if msg[3] == self.id:
                # If it is the destination
                print("{}: Message from {} to {} successfully received.".format(self.id, msg[1], self.id))
            else:
                # First send the update message and update the reachable list
                self.sock.send(self.update_msg().encode())
                self.on_recv(self.sock.recv(1024).decode())
                hop_list = ' '.join(msg[5:])
                hop_list = ast.literal_eval(hop_list)
                hop_list.append(self.id)
                reachable_set = set(self.reachable_list.keys())
                
                if reachable_set.issubset(set(hop_list)):
                    # If every reachable node is in the hop list
                    print("{}: Message from {} to {} could not be delivered.".format(self.id, msg[1], msg[3]))
                else:
                    # Forward the message to the next recepient
                    print("{}: Message from {} to {} being forwarded through {}".format(self.id, msg[1], msg[3], self.id))
                    self.send_where(msg[3])
                    next = self.find_min(self.known[msg[3]], hop_list)
                    msg = data_message(msg[1], next, msg[3], hop_list)
                    self.sock.send(msg.encode())
                    # print("sent")


    def update_msg(self):
        # Generate the UPDATEPOSITION message
        return "UPDATEPOSITION {} {} {} {}".format(self.id, self.range, self.posX, self.posY)

    def send_where(self, id):
        # Generate the WHERE message
        self.sock.send("WHERE {}".format(id).encode())
        there_msg = self.sock.recv(1024).decode()
        self.on_recv(there_msg)

    def move(self, posX, posY):
        # Move to posX, posY and send update message to control server
        self.posX = posX
        self.posY = posY
        self.pos = [posX, posY]
        self.sock.send(self.update_msg().encode())
        self.on_recv(self.sock.recv(1024).decode())

    def find_min(self, pos, h_list):
        # Find the node closest to the destination 
        min_dist = 1000
        ret = ''
        for i in self.reachable_list:
            if i not in h_list:
                dist = get_distance(self.reachable_list[i], pos)
                if dist < min_dist:
                    min_dist = dist
                    ret = i
        return ret

    def send_data(self, dest):
        # update the rechable list before sending any data
        self.sock.send(self.update_msg().encode())
        self.on_recv(self.sock.recv(1024).decode())
        if dest in self.reachable_list:
            # If the destination is reachable
            print("{}: Sent a new message directly to {}.".format(self.id, dest))
            msg = data_message(self.id, dest, dest, [self.id])
            self.sock.send(msg.encode())
        else:
            # Frist acquire the position of the destination
            self.send_where(dest)
            if dest in self.known and len(self.reachable_list) != 0:
                # Forward the message if there are some reachable nodes
                print("{}: Sent a new message bound for {}.".format(self.id, dest))
                next = self.find_min(self.known[dest], [])
                msg = data_message(self.id, next, dest, [self.id])
                self.sock.send(msg.encode())
            else:
                print("{}: Message from {} to {} could not be delivered.".format(self.id, self.id, dest))



if __name__ == '__main__':
    c_addr = sys.argv[1]
    c_port = int(sys.argv[2])
    s_id = sys.argv[3]
    s_range = float(sys.argv[4])
    posX = float(sys.argv[5])
    posY = float(sys.argv[6])
    sensor = Sensor(c_addr, c_port, s_id, s_range, posX, posY)
    while(True):
        input_list = [sys.stdin, sensor.sock]
        rd, wd, _ = select.select(input_list, [], [])
        for r in rd:
            if r == sensor.sock:
                # Handle incoming message
                msg = r.recv(1024).decode()
                sensor.on_recv(msg)
            else:
                #Handle user input
                user_cmd = input()
                user_cmd = user_cmd.split(" ")
                if user_cmd[0] == "WHERE":
                    sensor.send_where(user_cmd[1])

                if user_cmd[0] == "MOVE":
                    sensor.move(float(user_cmd[1]), float(user_cmd[2]))

                if user_cmd[0] == "SENDDATA":
                    sensor.send_data(user_cmd[1])

                if user_cmd[0] == "QUIT":
                    # TODO: clean up any memory and any sockets that are in use, and then terminate.
                    sensor.sock.close()
                    exit()