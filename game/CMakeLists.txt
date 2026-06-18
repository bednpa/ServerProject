find_package(Boost REQUIRED CONFIG)
find_package(nlohmann_json REQUIRED)

add_library(Game)

target_sources(Game
    PRIVATE
        gameroom.cpp
        playersession.cpp
    PUBLIC
        FILE_SET HEADERS
        FILES
            gameroom.h
            playersession.h
)

target_link_libraries(Game
    PUBLIC
        Boost::headers
        ThreadPool
    PRIVATE
        nlohmann_json::nlohmann_json
)