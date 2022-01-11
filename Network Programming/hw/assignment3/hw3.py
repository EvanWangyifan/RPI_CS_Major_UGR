#!/usr/bin/env python3

from concurrent import futures
import sys  # For sys.argv, sys.exit()
import socket  # for gethostbyname()
import math

import grpc

import csci4220_hw3_pb2
import csci4220_hw3_pb2_grpc

buckets_size = 4

def show_buckets(buckets):
	'''show the k-bucket information'''
	for i in range(buckets_size):
		print(i, end=":")
		for node in reversed(buckets[i]):
			print(" " + str(node.id) + ":" + str(node.port), end="")
		print()

def update_servicer(servicer, node,change=True):
	'''add the new node to the k-bucket
		if node exist,change its content
	'''
	if node.id != servicer.current_node.id:
		i = math.floor(math.log(node.id ^ servicer.current_node.id, 2))
		if node not in servicer.buckets[i]:
			servicer.buckets[i].insert(0,node)
		else:
			if change:
				servicer.buckets[i].remove(node)
				servicer.buckets[i].insert(0,node)
	if len(servicer.buckets[i]) > servicer.k:
		servicer.buckets[i].pop()

def get_closed_nodes(servicer,distance):
	'''get the closed nodes'''
	nodes = []
	for bucket in servicer.buckets:
		for node in bucket:
			nodes.append(node)
	nodes.sort(key=lambda node: node.id ^ distance)
	if len(nodes) <= servicer.k:
		return nodes
	else:
		return nodes[:servicer.k]

class KadImplServicer(csci4220_hw3_pb2_grpc.KadImplServicer):

	def __init__(self):
		''' init the kadImpServicer '''
		self.current_node = None
		self.buckets = []
		self.k = 0
		self.table = {}
		for i in range(buckets_size):
			self.buckets.append([])

	def FindNode(self,request,context):
		''' handle FIND_NODE command '''
		print("Serving FindNode({}) request for {}".format(request.idkey,request.node.id))
		close_nodes = []
		for bucket in self.buckets:
			for node in bucket:
				close_nodes.append(node)
		if request.node in close_nodes:
			close_nodes.remove(request.node)
		while len(close_nodes) > self.k:
			temp = None
			distance = 0
			for node in close_nodes:
				length = node.id ^ request.idkey
				if length > distance:
					distance = length
					temp = node
			close_nodes.remove(temp)
		nodeList = csci4220_hw3_pb2.NodeList(
			responding_node = self.current_node,
			nodes = close_nodes
		)
		update_servicer(self, request.node)
		return nodeList

	def Store(self,request,context):
		'''handle the STORE command'''
		print('Storing key {} value "{}"'.format(str(request.key),request.value))
		self.table[request.key] = request.value
		update_servicer(self,request.node)
		return csci4220_hw3_pb2.IDKey(
			node = self.current_node,
			idkey = request.key
		)

	def FindValue(self,request,context):
		''' handle the FIND_VALUE command'''
		print("Serving FindKey({}) request for {}".format(request.idkey,request.node.id))

		if request.idkey not in self.table:
			closed_nodes = []
			for bucket in self.buckets:
				for node in bucket:
					closed_nodes.append(node)
			if request.node in closed_nodes:
				closed_nodes.remove(request.node)
			while len(closed_nodes) > self.k:
				temp = None
				distance = 0
				for node in closed_nodes:
					length = node.id ^ request.idkey
					if length > distance:
						distance = length
						temp = node
				closed_nodes.remove(temp)
			update_servicer(self,request.node)
			return csci4220_hw3_pb2.KV_Node_Wrapper(
				responding_node = self.current_node,
				mode_kv = False,
				nodes = closed_nodes,
			)
		else:
			update_servicer(self,request.node)
			return csci4220_hw3_pb2.KV_Node_Wrapper(
				responding_node = self.current_node,
				mode_kv = True,
				kv = csci4220_hw3_pb2.KeyValue(
					node = self.current_node,
					key = request.idkey,
					value = self.table[request.idkey]
				)
			)


	def Quit(self,request,context):
		''' handle the QUIT command '''
		for i in range(buckets_size):
			if request.node in self.buckets[i]:
				print("Evicting quitting node {} from bucket {}".format(str(request.node.id),str(i)))
				self.buckets[i].remove(request.node)
				return csci4220_hw3_pb2.IDKey(
					node = self.current_node,
					idkey = request.node.id
				)
		print("No record of quitting node {} in k-buckets.".format(str(request.idkey)))
		return None


def run():
	if len(sys.argv) != 4:
		print("Error, correct usage is {} [my id] [my port] [k]".format(sys.argv[0]))
		sys.exit(-1)

	local_id = int(sys.argv[1])
	my_port = str(int(sys.argv[2])) # add_insecure_port() will want a string
	k = int(sys.argv[3])
	my_hostname = socket.gethostname() # Gets my host name
	#print(my_hostname)
	my_address = socket.gethostbyname(my_hostname) # Gets my IP address from my hostname
	#print(my_address)
	#remote_addr = socket.gethostbyname(remote_addr_string)
	#remote_port = int(remote_port_string)
	#channel = grpc.insecure_channel(remote_addr + ':' + str(remote_port))

	server = grpc.server(futures.ThreadPoolExecutor(max_workers=10))
	servicer = KadImplServicer()
	csci4220_hw3_pb2_grpc.add_KadImplServicer_to_server(servicer,server)
	server.add_insecure_port('[::]:'+my_port)
	server.start()
	#init the servicer
	servicer.k = k
	servicer.current_node = csci4220_hw3_pb2.Node(
		id = local_id,
		port = int(my_port),
		address = my_address
	)

	while True:
		#keep read the command and handle
		line = input()
		cmds = line.split(' ')
		if cmds[0] == 'BOOTSTRAP':
			#handle the BOOTSTRAP command
			remote_hostname = str(cmds[1])
			remote_port = int(cmds[2])
			remote_addr = socket.gethostbyname(remote_hostname)
			channel = grpc.insecure_channel(remote_addr + ':' + str(remote_port))
			stub = csci4220_hw3_pb2_grpc.KadImplStub(channel)
			nodeList = stub.FindNode(csci4220_hw3_pb2.IDKey(
				node = servicer.current_node,
				idkey = local_id
			))
			update_servicer(servicer, nodeList.responding_node)
			for node in nodeList.nodes:
				update_servicer(servicer, node)

			print("After BOOTSTRAP({}), k-buckets are:".format(str(nodeList.responding_node.id)))
			show_buckets(servicer.buckets)
			channel.close()
		elif cmds[0] == 'FIND_NODE':
			#handle the FIND_NODE command
			print("Before FIND_NODE command, k-buckets are:")
			show_buckets(servicer.buckets)

			node_id = int(cmds[1])
			is_found = False
			S = get_closed_nodes(servicer,node_id)
			closed_nodes = S
			has_found = []
			while closed_nodes and is_found !=True :
				for closed_node in closed_nodes:
					channel = grpc.insecure_channel(closed_node.address + ':' + str(closed_node.port))
					stub = csci4220_hw3_pb2_grpc.KadImplStub(channel)
					R = stub.FindNode(csci4220_hw3_pb2.IDKey(
						node = servicer.current_node,
						idkey = node_id
					))

					update_servicer(servicer, closed_node)
					for node in R.nodes:
						#if a node in R was already in bucket,don't change the postion
						update_servicer(servicer, node, False)
					has_found.append(closed_node)
					#channel.close()
					for bucket in servicer.buckets:
						if closed_node in bucket:
							is_found = True
							print("Found destination id {}".format(str(node_id)))
							break
				if is_found != True:
					S = get_closed_nodes(servicer,node_id)
					closed_nodes.clear()
					for node in S:
						if node not in has_found:
							closed_nodes.append(node)

			if is_found != True:
				print("Could not find destination id {}".format(str(node_id)))
			print("After FIND_NODE command, k-buckets are:")
			show_buckets(servicer.buckets)

		elif cmds[0] == 'FIND_VALUE':
			#handle the FIND_VALUE command
			print("Before FIND_VALUE command, k-buckets are:")
			show_buckets(servicer.buckets)

			key = int(cmds[1])
			is_found = False
			if key in servicer.table:
				print('Found data "{}" for key {}'.format(servicer.table[key],str(key)))
				is_found = True
			else:
				S = get_closed_nodes(servicer,key)
				closed_nodes = S
				has_found = []
				while closed_nodes and is_found !=True :
					for closed_node in closed_nodes:
						channel = grpc.insecure_channel(closed_node.address + ':' + str(closed_node.port))
						stub = csci4220_hw3_pb2_grpc.KadImplStub(channel)
						R = stub.FindValue(csci4220_hw3_pb2.IDKey(
							node = servicer.current_node,
							idkey = key
						))

						update_servicer(servicer, closed_node)
						has_found.append(closed_node)
						channel.close()
						if R.mode_kv:
							print('Found value "{}" for key {}'.format(R.kv.value,str(key)))
							is_found = True
							break
						else:
							for node in R.nodes:
								#if a node in R was already in bucket,don't change the postion
								update_servicer(servicer, node,False)

					if is_found != True:
						S = get_closed_nodes(servicer,key)
						closed_nodes.clear()
						for node in S:
							if node not in has_found:
								closed_nodes.append(node)

			if is_found != True:
				print("Could not find key {}".format(str(key)))
			print("After FIND_VALUE command, k-buckets are:")
			show_buckets(servicer.buckets)
		elif cmds[0] == 'STORE':
			#handle the store command
			key = int(cmds[1])
			value = cmds[2]
			closed_node = servicer.current_node
			distance = abs(servicer.current_node.id - key)
			for bucket in servicer.buckets:
				for node in bucket:
					if abs(node.id - key) < distance:
						closed_node = node
						distance = abs(node.id - key)

			if closed_node.id != servicer.current_node.id:
				channel = grpc.insecure_channel(closed_node.address + ':' + str(closed_node.port))
				stub = csci4220_hw3_pb2_grpc.KadImplStub(channel)

				stub.Store(csci4220_hw3_pb2.KeyValue(
					node = servicer.current_node,
					key = key,
					value = value
				))
				channel.close()
			else:
				servicer.table[key] = value
			print("Storing key {} at node {}".format(str(key),str(closed_node.id)))
		elif cmds[0] == 'QUIT':
			#handle the QUIT command
			for bucket in reversed(servicer.buckets):
				for node in reversed(bucket):
					channel = grpc.insecure_channel(node.address + ':' + str(node.port))
					stub = csci4220_hw3_pb2_grpc.KadImplStub(channel)

					print("Letting {} know I'm quitting.".format(str(node.id)))
					stub.Quit(csci4220_hw3_pb2.IDKey(
						node = servicer.current_node,
						idkey = local_id
					))
					channel.close()

			print("Shut down node {}".format(str(local_id)))
			break
		else:
			#error command
			print("error command")

if __name__ == '__main__':
	run()
