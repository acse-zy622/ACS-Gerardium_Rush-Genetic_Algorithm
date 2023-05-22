import graphviz
import numpy as np

# circuit_conf = np.array([0, 1, 3, 2, 0, 4, 3, 0, 5])
circuit_conf = np.array([4, 5, 1, 2, 4, 0, 1, 1, 6, 1, 3])

# Create a diagram
dot = graphviz.Digraph()

dot.node('Feed', 'Feed', shape='box')
dot.node('Concentrate', 'Concentrate', shape='box')
dot.node('Tailings', 'Tailings', shape='box')

for n in range(int((circuit_conf.size - 1) / 2)):
    dot.node("Unit{}".format(n), "Unit {}".format(n), shape='box')

dot.edge("Feed", "Unit{}".format(circuit_conf[0]), constraint='false', color='blue')

for n in range(circuit_conf.size):
    if (n > 0):
        if circuit_conf[n] == (circuit_conf.size - 1) / 2:
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Concentrate", constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Concentrate", constraint='false', color='red')
        elif circuit_conf[n] == (circuit_conf.size - 1) / 2 + 1:
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Tailings", constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Tailings", constraint='false', color='red')
        else:
            if n % 2 == 0:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Unit{}".format(circuit_conf[n]), constraint='false', color='blue')
            else:
                dot.edge("Unit{}".format(int((n - 1) / 2)), "Unit{}".format(circuit_conf[n]), constraint='false', color='red')

# Show image
dot.render('flowchart', format='png', view=True)
