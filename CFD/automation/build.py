import os

data = os.path.join(os.getcwd(), 'sim_data')

scaffold = [data]

for parm in ['w1', 'w2', 'w3', 'w4', 'w5']:
	this_path = os.path.join(data, parm)
	print(this_path)
	scaffold.append(this_path)

for directory in scaffold:
	print(directory)
	if not os.path.exists(directory):
	    os.makedirs(directory)