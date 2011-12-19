TARGET = Dijkstra
CONFIG += stl \
    debug_and_release
QT -= core gui
QMAKE_CXXFLAGS_DEBUG += -ggdb
# QMAKE_CXXFLAGS_DEBUG += -Wno-unused-but-set-variable
QMAKE_CXXFLAGS_RELEASE += -O3
HEADERS += common/assert.hpp \
    algos/BHDijkstra.hpp \
    common/EdgeIterator.hpp \
    Dijkstra.hpp \
    common/Timer.hpp \
    common/HeapItem.hpp \
    common/CalculationResult.hpp \
    algos/DialDijkstra.hpp \
    common/BinaryHeap.hpp \
    common/Node.hpp \
    common/Edge.hpp \
    common/AdjacencyArray.hpp
SOURCES += algos/DialDijkstra.cpp \
    algos/BHDijkstra.cpp \
    common/EdgeIterator.cpp \
    common/Timer.cpp \
    common/AdjacencyArray.cpp \
    common/Node.cpp \
    common/Edge.cpp \
    common/CalculationResult.cpp \
    common/BinaryHeap.cpp \
    Dijkstra.cpp
FORMS += 
RESOURCES += 
