cmake_minimum_required(VERSION 3.15)
project(QuickstartChat CXX)

set(CMAKE_CXX_STANDARD 17)

add_executable(quickstart-chat
        quickstart-chat.cpp
        # src/HttpClient.cpp
        # src/IdentityClient.cpp
        # src/WsClient.cpp
        # src/DatabaseClient.cpp
        # If your models produce .cpp files, list them here as well
        # src/models/Player.cpp
        # src/models/CreatePlayerArgs.cpp
)

target_include_directories(quickstart-chat PUBLIC ${CMAKE_SOURCE_DIR}/include/SpacetimeDB)

target_link_libraries(quickstart-chat
        PRIVATE
        SpacetimeDBSdk
        # cpr::cpr
        # nlohmann_json::nlohmann_json
        # Websockets     # or Boost::beast, Boost::asio, etc.
        # For WebSocket++, you may need to link with -lssl -lcrypto and pthread
)
