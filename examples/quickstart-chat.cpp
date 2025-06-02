// File: quickstart_chat.cpp
//
// A minimal “Chat” quickstart in C++ using SpacetimeDB’s HTTP & WebSocket APIs directly.
// This example assumes you have a SpacetimeDB server running locally with a “chat” module
// whose schema defines a `messages` table and a `SendMessage` reducer.
//
// messages table (example):
//   CREATE TABLE messages (
//     id            ALPHANUM PRIMARY KEY,
//     sender        STRING,
//     content       STRING,
//     created_at    TIMESTAMP DEFAULT NOW()
//   );
//
// SendMessage reducer (example):
//   CREATE OR REPLACE FUNCTION SendMessage(::sender STRING, ::content STRING) AS $$
//     INSERT INTO messages (id, sender, content) VALUES (GENERATE_ID(), ::sender, ::content);
//   $$ LANGUAGE SQL;
//

// -----------------------------------------------------------------------------
// 1. Include SDK‐style headers (HTTP + WebSocket + JSON utilities).
//    We assume the SDK layout defined in SpacetimeClientSDKReference.md has been applied.
// -----------------------------------------------------------------------------

#include "Utils/HttpClient.hpp"
#include "Identity/IdentityClient.hpp"
#include "WebSocket/WebSocketClient.hpp"
#include "Database/DatabaseClient.hpp"
#include "Utils/Json.hpp"

#include <iostream>
#include <string>

// using namespace SpacetimeDB;

// -----------------------------------------------------------------------------
// 2. Define a simple struct to hold chat messages.
//    Since SpacetimeDB does not generate C++ bindings automatically, we map fields by hand.
// -----------------------------------------------------------------------------

struct ChatMessage {
    std::string id;
    std::string sender;
    std::string content;
    std::string created_at;
};

static ChatMessage chatMessageFromJson(const SpacetimeDB::Utils::Json& j) {
    ChatMessage msg;
    msg.id         = j.at("id").get<std::string>();
    msg.sender     = j.at("sender").get<std::string>();
    msg.content    = j.at("content").get<std::string>();
    msg.created_at = j.at("created_at").get<std::string>();
    return msg;
}

// -----------------------------------------------------------------------------
// 3. Main entry point: authenticate, subscribe to incoming messages, and allow
//    the user to send chat messages via the SendMessage reducer.
// -----------------------------------------------------------------------------

int main() {
    try {
        SpacetimeDB::Utils::HttpClient Http("http://localhost:3000", /*timeoutMs=*/30000);


        const SpacetimeDB::IdentityClient IdClient(Http);


        const auto IdentityResult = IdClient.CreateIdentity();
        if (!SpacetimeDB::Utils::IsValid(IdentityResult))
        {
            const auto Error = SpacetimeDB::Utils::GetErrorMessage(IdentityResult);
            std::cerr << "[Identity] failed to create identity: " << Error << "\n";
            return 1;
        }
        const auto Identity = SpacetimeDB::Utils::GetResult(IdentityResult);
        std::cout << "[Identity] generated ID: " << Identity.Id << "\n";


        auto PublicKeyResult = IdClient.GetPublicKey();
        if (!SpacetimeDB::Utils::IsValid(PublicKeyResult))
        {
            const auto Error = SpacetimeDB::Utils::GetErrorMessage(PublicKeyResult);
            std::cerr << "[Identity] failed to get public key: " << Error << "\n";
            return 1;
        }
        const auto [Data] = SpacetimeDB::Utils::GetResult(PublicKeyResult);
        std::cout << "[Identity] received public key <application/pem-certificate-chain>:\n" << Data << "\n";


        const auto ClientDatabasesResult = IdClient.GetDatabases(Identity.Id);
        if (!SpacetimeDB::Utils::IsValid(ClientDatabasesResult))
        {
            const auto Error = SpacetimeDB::Utils::GetErrorMessage(ClientDatabasesResult);
            std::cerr << "[Identity] failed to get databases: " << Error << "\n";
            return 1;
        }
        const auto Databases = SpacetimeDB::Utils::GetResult(ClientDatabasesResult);
        std::cout << "[Identity] received databases addresses for generated ID: ";
        if (Databases.Addresses.empty())
        {
            std::cout << "none\n";
        }
        else for (const auto& Address : Databases.Addresses)
        {
            std::cout << "'" << Address << "'; ";
        }


        /*
        auto EmailResponse = IdClient.SetEmail(
            Identity.Id,
            SpacetimeDB::SetEmailRequest{"johnny@home.com", Identity.Token});
        if (!SpacetimeDB::Utils::IsValid(EmailResponse))
        {
            const auto Error = SpacetimeDB::Utils::GetErrorMessage(EmailResponse);
            std::cerr << "[Identity] failed to set email: " << Error << "\n";
            return 1;
        }
        const auto Email = SpacetimeDB::Utils::GetResult(EmailResponse);
        std::cout << "[Identity] received email for generated ID. " << "\n";
        */


        auto IdVerificationResult = IdClient.VerifyIdentity(SpacetimeDB::VerifyIdentityRequest{Identity});
        if (!SpacetimeDB::Utils::IsValid(IdVerificationResult))
        {
            const auto Error = SpacetimeDB::Utils::GetErrorMessage(IdVerificationResult);
            std::cerr << "[Identity] failed to verify identity: " << Error << "\n";
            return 1;
        }
        const auto [Status] = SpacetimeDB::Utils::GetResult(IdVerificationResult);
        switch (Status)
        {
            case SpacetimeDB::VerifyIdentityResponse::ValidMatch:
                std::cout << "[Identity] identity verification succeeded: ID and token match.\n";
                break;
            case SpacetimeDB::VerifyIdentityResponse::ValidMismatch:
                std::cout << "[Identity] identity verification succeeded: valid token, ID mismatch.\n";
                break;
            case SpacetimeDB::VerifyIdentityResponse::InvalidOrNoAuthorizationToken:
                std::cout << "[Identity] identity verification failed: invalid or no authorization token.\n";
                break;
        }


        return -1;

        // ---- 3.4 Prepare a WebSocket client and a DatabaseClient for the “chat” module
        SpacetimeDB::WebSocketClient WebSocketClient;
        auto Token = Identity.Token;
        SpacetimeDB::DatabaseClient  DatabaseClient(Http, WebSocketClient, Token);

        // ---- 3.5 Subscribe to new rows in the `messages` table over WebSocket.
        //         We listen for InsertEvent frames and print each incoming message.
        //
        //   The subscription query orders by created_at so we see messages in chronological order.

        DatabaseClient.Subscribe(
            "quickstart-chat",
            "SELECT * FROM messages ORDER BY created_at",
            [](const SpacetimeDB::Utils::Json& EventJson)
            {
                // Only handle InsertEvent frames
                if (EventJson.value("type", "") == "InsertEvent") {
                // The “row” field contains the new chat message as a JSON object.
                const SpacetimeDB::Utils::Json& Row = EventJson.at("row");
                ChatMessage Message = chatMessageFromJson(Row);

                // Print in format: [timestamp] sender: content
                std::cout << "[" << Message.created_at << "] "
                    << Message.sender << ": "
                    << Message.content << "\n";
                }
            }
        );

        // ---- 3.6 Prompt the user for their name (the `sender` field).
        std::cout << "\nEnter your chat name: ";
        std::string Sender;
        std::getline(std::cin, Sender);

        std::cout << "Hello, " << Sender << "! You can now enter chat messages.\n"
                  << "(Submit an empty line to quit.)\n\n";

        // ---- 3.7 In a loop, read lines from stdin and send them as chat messages.
        //         Each message is sent by calling the “SendMessage” reducer.
        //
        while (true) {
            std::cout << "> ";
            std::string Line;
            std::getline(std::cin, Line);

            // Empty line means exit
            if (Line.empty()) {
                std::cout << "Exiting chat. Goodbye!\n";
                break;
            }

            // Build reducer arguments manually into JSON
            SpacetimeDB::Utils::Json Args = SpacetimeDB::Utils::Json();
            Args["sender"]  = Sender;
            Args["content"] = Line;

            // CallReducer will send:
            //   { "type": "CallReducer", "reducer": "SendMessage", "args": { "sender": ..., "content": ... } }
            // and wait for { "type": "ReducerResult" } or an { "type": "Error", ... } frame.

            // Optionally, inspect the reducerResult for errors or returns:
            auto ReducerResult = DatabaseClient.CallReducer("chat", "SendMessage", Args);
            // if (!SpacetimeDB::Utils::IsValid(ReducerResult))
            {
                auto ReducerError = SpacetimeDB::Utils::GetResult(ReducerResult);
                if (ReducerError.value("type", "") == "Error")
                {
                    int  Code    = ReducerError.value("code", 0);
                    std::string Message = ReducerError.value("message", "");
                    std::cerr << "[SendMessage error: code " << Code << "] " << Message << "\n";
                }

                const auto Error = SpacetimeDB::Utils::GetErrorMessage(ReducerResult);
                std::cerr << "[SendMessage error: " << Error << "]\n";
                continue;
            }
            // Otherwise, SendMessage has succeeded (it has no return payload in this example).
        }

        // ---- 3.8 Clean up: close the WebSocket before exiting
        WebSocketClient.Close();
    }
    catch (const std::exception& Exception) {
        std::cerr << "[Fatal Error] " << Exception.what() << "\n";
        return 1;
    }

    return 0;
}
