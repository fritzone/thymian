set (tinyxml_src
    tinystr.cpp
    tinyxml.cpp
    tinyxmlerror.cpp
    tinyxmlparser.cpp
)

add_library(tinyxml STATIC ${tinyxml_src})
