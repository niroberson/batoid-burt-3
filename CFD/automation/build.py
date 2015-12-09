import os
import json

# Define the data storage directory, make sure this directory exists
base = os.getcwd()
data = os.path.join(base, 'sim_data')
if not os.path.exists(data):
		print('Created directory: ' + data)
		os.makedirs(data)

# Read in from config all of the parameters
config_path = os.path.join(base, 'config.json')
with open(config_path) as config_file:
	config = json.load(config_file)

# Generate C input files that combines parameters
lines = open(os.path.join(base,'setup','Finmotion.c'), 'r').readlines()
Amax = lines[7]
wave = lines[8]
freq = lines[9]

lines[7] = 'double Amax = ' + str(1) + ';\n'
lines[8] = 'double lambda = ' + str(1) + ';\n'
lines[9] = 'double w = ' + str(1) + ';\n'

out = open(os.path.join(base,'setup','Finmotion_new.c'), 'w')
out.writelines(lines)
out.close()

# Generate strings that combines all parameters

# Create a list of all the paths of the directories to create
scaffold = []
for parm in ['w1', 'w2', 'w3', 'w4', 'w5']:
	this_path = os.path.join(data, parm)
	scaffold.append(this_path)

# Create all paths in scaffold
for directory in scaffold:
	if not os.path.exists(directory):
		print('Created directory: ' + directory)
		os.makedirs(directory)

# Copy the general case into each directory