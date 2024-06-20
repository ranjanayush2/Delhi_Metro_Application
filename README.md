This is c++ Delhi Metro Application that will take information (name) of the source station and the destination station, of Delhi Metro, from the user and display the shortest metro route to reach the destination. It will also be having a metro map for commuter’s better navigation.

The idea is implemented using Graph and Heap data structures. The graph has nodes and edges. Nodes represent a metro station that will be containing certain information regarding that station like its name, its metro corridor, and the lines which it connects. Edges (the connection between two nodes) represent the distance between the two stations and the cost of each edge will be equal to the distance between the two of its connecting stations(nodes).

By using different algorithms like Dijkstra, breadth-first search etc, the shortest path between the source station and the destination station is determined, and accordingly, the most optimal route is being calculated on the basis of shortest distance between the two stations.Finally,the metro route between the two stations and details about intersection stations is displayed.

To run the application,run the metrooo.cpp file in the terminal,make sure that all the txts file should be in the same folder as the "metroooo.cpp" file.
