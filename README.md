# Gerardium Rush: Applying Computational Science Third Project 
The developed software executes a genetic algorithm to optimise a speicification vector for the circuit, checks the validity of the circuit and evaluates its performance. Additionally, the software incorporates a script to convert the circuit vector into an image of the circuit.

### **Contents**

<!-- TOC -->
* [About](#about)
* [Installation Guide](#installation-guide)
* [User Instructions](#user-instructions)
* [Documentation](#documentation)
* [Testing](#testing)
* [Authors](#authors)
* [License](#license)
<!-- TOC -->

### **About**

Separation technologies are widely used to improve the purity of products. Thesetechnologiesusually take the form of identical or near identical separation units(referred to as unitsfor brevity) that are arranged in circuits. While a single separation unit in isolation will only recover a small proportion of the valuable material, multiple units can be combined together in circuits that can enhance recovery of the valuable material. The basic challenge is to design the circuit for optimal recovery. These circuits can have simple rows of units with the tailings or concentrate being passed to the next unit along(but not both). The circuits can also involve recycles, where a stream is passed back to a unit nearer the beginning of the circuit (but not the same unit). This means that the number of potential circuits increases factorially with the number of units in the circuits.

Genetic algorithms are a popular approach to problems with discrete variables. Instead of proposing a single solution to an optimisation problem, a genetic algorithm generates many possible solutionsthat form a population. Then solutions are scored using a fitness function (objective function) to decide which solutions are better than others.These candidate solutions are recombined so that the best solutions reproduceto create a new generation of solutions with the best traits of the previous solutions. This continues until improvement stops or the max number of generations is reached.

#### **Inputs**
The program takes one input, i.e., the number of units to be present in a circuit.

#### **Outputs**
The software outputs the directory that contains the image of the optimised circuit. Moreover, the program also prints the evaluation score of each optimised circuit.

#### **Additional Options**
- Efficiently utilizes multi-threaded capabilities by emplying OpenMP function
- Provides output for a user-generated input using a basic command line interface

### **Installation Guide**

Before installing the software, it is worth ensuring your environment meets the dependencies below:

- Python > 3.9
- numpy >= 1.13.0
- scipy
- jupyter
- sympy
- pandas
- matplotlib
- yaml
- graphviz

If using conda, a new environment can be set up with the below command, after navigating to the tool directory:

```bash
conda env create -f environment.yml
```

After activating this environment,

```bash
conda activate fluorite
```

flood_tool can be installed using:

```bash
pip install .
```

### **Executing the Program**

To access the GUI that we have developed please run the code below:
```bash
g++ src/main.cpp
./a.out
```

Once the code above is run, you will be directed to a 
If any of these attributes are missing, you'll be directed to a webpage that displays information about the project.
Otherwise, you'll be directed to a webpage that displays plots of detected craters in the image.
                                     
### **Documentation**

The tool `docs/html/` directory contains detailed documentation for this package, including examples and details of all functions and inputs.

This can be viewed in your browser through the `index.html` file, or directly by the `flourite.pdf` file.

### **Testing**

The tool includes functional tests that located in `/tests`. The tests are automatically run by GitHub Actions workflow.

### **Authors**

- Pablo Basterrechea Roca
- Rubab Atwal
- Jiachen Dong
- Harry Wang
- Jiajia Zheng
- Boxuan Zhu
- Zhouran Yang

With thanks to James Percival & Thomas Davidson for their support.
 
### **License**
[MIT](https://choosealicense.com/licenses/mit/)
