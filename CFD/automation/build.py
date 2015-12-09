import os

# Define the data storage directory, make sure this directory exists
data = os.path.join(os.getcwd(), 'sim_data')
if not os.path.exists(data):
	   os.makedirs(data)

# Read in from config all of the parameters

# Generate folders that describe the combinations of parameters

# Create a list of all the paths of the directories to create
scaffold = []
for parm in ['w1', 'w2', 'w3', 'w4', 'w5']:
	this_path = os.path.join(data, parm)
	print(this_path)
	scaffold.append(this_path)

# Create all paths in scaffold
for directory in scaffold:
	print(directory)
	if not os.path.exists(directory):
	    os.makedirs(directory)

# Copy the general case into each directory