# ERIS
A tool for the safety and security analysis of complex critical systems, such as the autonomous vehicle.
This tool has been developed in a doctoral research project. It was used as a proof of concept for a theoretical methodology that
combines the analysis of safety and security dependencies.

## Installation

For the installation of ERIS, first make sure that all relevant requirements are installed in your
system.

### Build from source
The current ERIS build is only tested and validated on Linux-based distributions (namely Kubuntu 20.04, Kubuntu 22.04, Debian 11).
This guide merely provides information for building on Linux, however, it has been tested on Windows and should work accordingly.

Make sure your system has all requirements installed as listed underneath! 
To create the Makefiles and dependencies perform:
```
cd /eris
cmake .
```
This will create all necessary Makefiles and CMakeFile.
Now run 
```
make
```
in the same directory to build ERIS.
This will create an executable '/bin/eris' and library files.

#### Requirements
The following requirements have to be installed in order to build, install and run ERIS.

##### QT Framework
[Qt >= 5.15.2](https://www.qt.io/) is required in order to build ERIS. 
Installing via apt repositories:
```
sudo apt-get install qtbase5-devs
```
ERIS requires packages Core, Gui, Widgets, PrintSupport, Svg.
The packages Charts, Svg are not part of the qtbase5 and require to be installed manually:
```
sudo apt-get install libqt5charts5-dev
sudo apt-get install libqt5svg5-dev

```

##### Pthread
The Pthrad lib is required for compiling ERIS. Pthreads are used for running the simulation part in the hybrid
mode.

##### Google Test
Unittests are developed using the [googletest](https://github.com/google/googletest) framework.

##### PRISM
In order to evaluate a modelled architecture, the PRISM model checker has to be intalled in the system.
ERIS expects the GUI version to be part of the system path as 'xprism' and the non-GUI version to be part of
the system path as 'prism'.

To add prism to your system, simply download it via the official webpage https://www.prismmodelchecker.org/
and follow the installation guide.

To add prism to your path (under linux) run:
```
export PATH="$PATH:/path/to/prism/bin/"

```
With "/path/to/prism/bin/" being the actual path to the prism /bin directory in your system.
Note that this only temporarily adds prism to your path. In order to make it permanent you can add this line into your .bashrc!


#### Octave
If the modelled architecture contains simulated nodes or structures, Octave has to be installed in the system.
ERIS expects (the non-GUI version of) octave to be included in the path as 'octave'. Note that ERIS may be used
without an octave installation, however, the simulation feature cannot be used and an error will be displayed
if tried regardless.

To add octave to your system, simply download it via the official webpage https://www.gnu.org/software/octave/index
and follow the installation guide.

To add octave to your path (under linux) run:
```
export PATH="$PATH:/path/to/octave/bin/"

```
With "/path/to/octave/bin/" being the actual path to the octave /bin directory in your system.
Note that this only temporarily adds octave to your path. In order to make it permanent you can add this line into your .bashrc!


## Usage
ERIS can be used to model critical systems and the safety and security dependencies of the individual components.
The modeled system can be translated into a Markov Decision Process (MDP) or a Continuous-Time Markov Chain in
the PRISM language, in order to evaluate it with the model checker PRISM.

### General

The following drop-down menus exist

#### File
- New File

  Deletes and clears the current scene.

- save File

  Saves the current scene to an .xml file. A default file name or the name of the previously opened file is
  used.

- save File As

  Saves the current scene to an .xml file with a user-specific name.

- Open File

  Opens a file browser window to select the file that shall be opened. If a file is selected, the .xml of
  the scene is parsed and displayed in ERIS.

- exportAs File As

  Exports the scene to a PNG, PDF or SVG. These files are solely for display in documents etc, they cannot be
  reloaded/imported into ERIS.

#### Item

- Delete

  Deletes the currently selected item(s)

#### Help

- Help

  Opens the help menu (NOT IMPLEMENTED YET)

#### Options

- Model

  Chooses the model that shall be used for evaluation in PRISM. MDP for Markov Decision Processes, CTMC for Continuous-Time Markov Chains.

- Mode Of Operation

  NOT IMPLEMENTED YET

- Set File Name

  Sets the name of the PRISM output file.

- Add Redundancy

  Add a global redundancy definition. This is prior used for critical nodes, non-critical nodes are determined redundant by the functional
  dependencies and the dependency definition in the target node.

### Nodes

1. Environment Node

   The environment node is a special type of node denoting the outside of the vehicle. This can be needed
   to express that a component receive commands (reach links) from the outside. However, also attackers
   reside in the environment that may misuse given connections.

2. Normal Node

   The normal node is a node that is not mandatory for the system but may provide essential data to
   other nodes, such as a sensor.

3. Critical Node

   The critical node is mandatory for the core functionality of the system, hence, if no redundancy definition
   is present, the failure of a critical node leads to the failure of the overall system.

Redundancy definitions of critical nodes are defined as a global definition in the tab Option -> Add Redundancy
and have the form ´n1=n2´ expressing n1 and n2 are redundant.

### States
Each node of the system has a state, which is needed for the analysis with the model checker-

1. Ok - 0

   The component is working as intended.

2. Defective - 1

   The component turned non-operational due to a safety incident e.g. hardware failure, software error etc.

3. Corrupted - 2

   The component was occupied by an attacker rendering it non-operational as it cannot be trusted any more.
   The component however can be misused by the attacker in order to attack further components (via reach-edges).


### Links/Edges

The following dependencies can be expressed:

1. Functional dependencies via functional links and a dependency definition.

   A functional dependency expresses that a component depends on the output of another component in order to work correctly.
   This means, that the providing node has to be in the state ok (0). In the settings of the receiving node,
   the dedicated functional dependency has to be set.

   Example: node n1 functionally depends on node n2 and n3

   Settings: `n2=0 | n3=0` expresses that n2 and n3 are redundant and one of them has to be ok (0) in order
   for n1 to work correctly.

   Settings: `n2=0 & n3=0` expresses that n2 and n3 do not provide redundant output and thus both of them
   have to be ok for n1 to work correctly.

2. security (access) dependencies via reach links

   A reach dependency expresses that one component can access another component and command it.
   A reach link coming from the environment node expresses that the target component may be accessed by the
   outside e.g. in order to provide software updates. Reach links can be misused by an attacker and are
   essential do describe attacks on the system. Since the attacker is initially residing on the environment
   node, an attack on a component can only happen if a backwards-reach path to environment node exists.

3. security guarantees via sec links

   Sec dependencies describe a selective class of links. A component targeting another component via
   sec links expresses that the component is offering security guarantees to it in e.g. a HSM, Firewall etc.
   or some module that is capable of providing authentication and encryption features.

### Dynamic

State changes are dictated by the probabilities/rates determined in the setting of the node. A safety incident
given by the set ´Failure Probability/Rate´ may always occur. A security incident given by 'Intrusion Probability/Rate'
may only occur if the node is targeted by a reach edge and the targeting node itself is corrupted or the environment node.
Additionally to that, the offered security guarantees ´Security Probability/Rate´ have to be taken into account. If a
node is about to get attacked, the current intrusion probability is substracted by the received security guarantees, expressing
that the node has some sort of protection against attacks.


