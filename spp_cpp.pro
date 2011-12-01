TARGET = spp_cpp
CONFIG += stl \
    debug_and_release
QMAKE_CXXFLAGS_DEBUG += -ggdb
QMAKE_CXXFLAGS_DEBUG += -Wno-unused-but-set-variable
HEADERS += common/EdgeIterator.hpp \
    Dijkstra.hpp \
    common/Timer.hpp \
    common/HeapItem.hpp \
    common/CalculationResult.hpp \
    algos/DialDijkstra.hpp \
    algos/StandardDijkstra.hpp \
    common/PriorityQueue.hpp \
    common/BinaryHeap.hpp \
    common/Node.hpp \
    common/Edge.hpp \
    common/AdjacencyArray.hpp
SOURCES += common/EdgeIterator.cpp \
    common/Timer.cpp \
    common/AdjacencyArray.cpp \
    common/Node.cpp \
    common/Edge.cpp \
    common/CalculationResult.cpp \
    common/BinaryHeap.cpp \
    Dijkstra.cpp
FORMS += 
RESOURCES += 
