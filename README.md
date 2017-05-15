ISIMA ZZ3 Project

Mobile objects identification is a common vehicle routing optimisation problem with several applications. For instance, it can be used to model and organise safety or humanitarian operations. In both cases, time may be critical and solutions must maximise the amount of visited mobiles and minimise the total needed time. This problem needs bi-criteria optimisation and is called an STDVRP.
 
To solve this problem, we implemented two route-building heuristics to generate feasible but not optimal solutions and a memory cache strategy was designed to speed-up the heuristics. Then, local search procedures were coded to look for improvements on the solutions and a Vertical Neighbourhood Descent was built to combine these procedures. After that, BRKGA and multi-start metaheuristics were used to generate better solutions on both time and amount criteria.

Development was made with the C++ programming language. We provide text output for all our executables. But we also have some adapters for LaTeX code output (with TikZ) on spatial and temporal points of view.

This project is under GPLv3 licence as it uses [BRKGA](https://github.com/rfrancotoso/brkgaAPI) as a dependency.

You will need [CMake](https://cmake.org/) and [Doxygen](http://www.stack.nl/~dimitri/doxygen/).

To compile the executables, please run the following command: `(test -d build || mkdir build) && cd build && cmake .. && make all`.

To build the doc, run the previous command and then `make doc`. Resulting files will be in `doc/html`.