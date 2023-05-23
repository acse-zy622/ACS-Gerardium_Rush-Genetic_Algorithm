#####################################################################
# Post processing program to visualize the circuit
# By Jiachen Dong
#####################################################################

import graphviz
import numpy as np

# Define the circuit array
circuit_conf = np.array([4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3])

# Create a directed diagram
dot = graphviz.Digraph()

# Add some common nodes
dot.node('Feed', 'Feed', shape='box')
dot.node('Concentrate', 'Concentrate', shape='box')
dot.node('Tailings', 'Tailings', shape='box')

# Add unit nodes
for n in range(int((circuit_conf.size - 1) / 2)):
    dot.node("Unit{}".format(n), "Unit {}".format(n), shape='box')

# Add an arrow from "feed" to a certain unit
dot.edge("Feed", "Unit{}".format(circuit_conf[0]), constraint='false', color='blue')

# Add some arrows
for n in range(circuit_conf.size):
    if n > 0:
        if circuit_conf[n] == (circuit_conf.size - 1) / 2:  # Add an arrow from a certain unit to "concentrate"
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Concentrate", constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Concentrate", constraint='false', color='red')
        elif circuit_conf[n] == (circuit_conf.size - 1) / 2 + 1:  # Add an arrow from a certain unit to "tailings"
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Tailings", constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Tailings", constraint='false', color='red')
        else:  # Add an arrow from unit to unit
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Unit{}".format(circuit_conf[n]), constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Unit{}".format(circuit_conf[n]), constraint='false', color='red')

# Show image
dot.render('flowchart', format='png', view=True)
