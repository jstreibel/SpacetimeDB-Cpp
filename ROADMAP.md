# SpacetimeClientSDKReference

Below is a language-agnostic blueprint of the core components and required functionality for any client-side SpacetimeDB SDK. Every SDK must implement:

* **Identity Management**: creating and verifying identities/tokens.
* **HTTP Layer**: issuing REST calls (e.g., raw SQL, database info).
* **WebSocket Layer**: maintaining a live subscription channel for real-time updates and reducer calls.
* **Data Models**: type definitions that mirror server tables and reducer argument/return schemas.
* **Utility/Error Handling**: JSON (de)serialization, header injection, error decoding.

Taken together, these components cover all canonical protocols (HTTP and WebSocket) defined by SpacetimeDB’s API.

## 1. Identity Management

Any client SDK must let applications generate and manage Spacetime identities and tokens via the `/v1/identity` endpoints.

### 1.1 POST /v1/identity

* **Purpose**: Generate a new public “identity” (UUID-like) and a secret JWT “token”.
* **Request**:

  ```json
  {}
  ```

  (An empty JSON object.)
* **Response**:

  ```json
  {
    "identity": "<string>",
    "token": "<string>"
  }
  ```

    * `identity`: long-lived public identifier.
    * `token`: short-lived JWT used in `Authorization: Bearer <token>`.

### 1.2 Other Endpoints (optional, but recommended)

* **POST /v1/identity/websocket-token** (issue a short-lived token for untrusted contexts).
* **GET /v1/identity/\:identity/databases** (list databases owned by an identity).
* **GET /v1/identity/public-key** (fetch server’s public key to verify JWTs).

### 1.3 SDK Responsibilities

* Wrap these endpoints in an `IdentityClient` (or similar) class.
* Store and refresh the `token` as needed.
* Expose methods like `createIdentity()` and `getIdentityInfo(identityId)` that return a struct with both `identity` and `token` fields.

## 2. HTTP Layer (RESTful Database Calls)

After obtaining a valid token, every SDK must permit raw SQL and database metadata calls via HTTP.

### 2.1 Authorization Header

* All database-related HTTP calls require:

  ```http
  Authorization: Bearer <token>
  ```

  where `<token>` comes from `/v1/identity`.

### 2.2 POST /v1/database/\:name\_or\_identity/sql

* **Purpose**: Execute arbitrary SQL (SELECT, INSERT, UPDATE, DELETE) against a published database.
* **Request Body**:

  ```json
  {
    "sql": "<SQL string>",
    "params": [ /* array of bound values */ ]
  }
  ```

    * `sql`: a valid SQL string.
    * `params`: optional array of parameter values.
* **Response**:

    * For `SELECT`:

      ```json
      {
        "rows": [
          { "col1": value1, "col2": value2, … },
          …
        ]
      }
      ```
    * For mutations:

      ```json
      {
        "rows_affected": <integer>
      }
      ```

### 2.3 GET /v1/database/\:name\_or\_identity/schema (optional)

* Fetches the database schema (table definitions, column types).

### 2.4 Other HTTP Endpoints (metadata)

* **GET /v1/database/\:name\_or\_identity**: Return JSON description of a database.
* **DELETE /v1/database/\:name\_or\_identity**: Delete a database.

### 2.5 SDK Responsibilities

* Provide an `HttpClient` abstraction (e.g., `get(path, headers)` and `post(path, body, headers)`) that:

    * Concatenates `base_url + path`.
    * Sets timeouts, retries, and injects `Authorization` and `Content-Type: application/json` headers.
* Wrap each of the above routes in methods of a `DatabaseClient` (or `RestClient`) class:

    * `executeSql(moduleName, sql, params)` → returns a parsed JSON or typed result.
    * `getSchema(moduleName)` → returns a type-safe schema descriptor (if desired).
* Handle errors: throw or return errors if HTTP status ≠ 200.

## 3. WebSocket Layer (Subscriptions & Reducers)

Every SDK must also support real-time subscriptions (row replication) and optional reducer invocation over WebSocket, using the `/v1/database/:name_or_identity/ws` endpoint.

### 3.1 WebSocket URL

* **Format**:

  ```
  ws://<host>:<port>/v1/database/<module_or_db_id>/ws?token=<jwt>
  ```

  or `wss://` if using TLS.

### 3.2 WebSocket Handshake

* Client dials the above URL; the server verifies the `token` and replies with:

  ```json
  { "type": "Connected", "connection_id": "<uuid>" }
  ```

### 3.3 Subscription Frame

* **Client→Server**:

  ```json
  {
    "type": "Subscribe",
    "query": "<SELECT projection FROM table WHERE predicate>"
  }
  ```

    * `type`: `"Subscribe"`.
    * `query`: SELECT statement restricted to the subscription SQL subset (no INSERT/UPDATE/DELETE).
* **Server→Client Acknowledgment**:

  ```json
  {
    "type": "Subscribed",
    "subscription_id": "<uuid>"
  }
  ```

  Indicates the server will stream all matching row events.

### 3.4 Row-Change Event Frames

* **InsertEvent**:

  ```json
  {
    "type": "InsertEvent",
    "subscription_id": "<uuid>",
    "table": "<table_name>",
    "row": { /* column:value map for the new row */ }
  }
  ```
* **UpdateEvent**:

  ```json
  {
    "type": "UpdateEvent",
    "subscription_id": "<uuid>",
    "table": "<table_name>",
    "old_row": { … },
    "new_row": { … }
  }
  ```
* **DeleteEvent**:

  ```json
  {
    "type": "DeleteEvent",
    "subscription_id": "<uuid>",
    "table": "<table_name>",
    "row": { … }
  }
  ```

### 3.5 Reducer Call Frame (over WebSocket)

* **Client→Server**:

  ```json
  {
    "type": "CallReducer",
    "reducer": "<reducer_name>",
    "args": { /* JSON object matching reducer argument schema */ }
  }
  ```
* **Server→Client**:

  ```json
  {
    "type": "ReducerResult",
    /* result fields depend on reducer’s return schema */
  }
  ```

  or, on error:

  ```json
  {
    "type": "Error",
    "code": <int>,
    "message": "<error_message>"
  }
  ```

### 3.6 Unsubscribe Frame

* **Client→Server**:

  ```json
  {
    "type": "Unsubscribe",
    "subscription_id": "<uuid>"
  }
  ```
* **Server→Client Acknowledgment**:

  ```json
  {
    "type": "Unsubscribed",
    "subscription_id": "<uuid>"
  }
  ```

### 3.7 SDK Responsibilities

* Provide a `WsClient` abstraction that:

    * Opens and maintains a WebSocket connection.
    * Sends typed JSON frames (`send(jsonObj)`).
    * Registers a callback or event handler for incoming frames, parses raw text into JSON, then dispatches to handlers based on `type`.
    * Handles reconnection logic, ping/pong (if required), and closing.
* Expose `subscribe(query, onEvent)` where `onEvent` is invoked for each `InsertEvent`, `UpdateEvent`, or `DeleteEvent`.
* Expose `callReducer(reducerName, args)` which sends a `CallReducer` frame and returns (or asynchronously resolves) the `ReducerResult`.
* Expose `unsubscribe(subscriptionId)` to stop real-time updates.

## 4. Data Models

To ensure type safety and easier JSON (de)serialization, an SDK should include language-specific model definitions that mirror server tables, reducers, and event frames.

### 4.1 Table Row Models

* For each table in a published database module, generate a struct/class that contains one field per column. E.g., if your module has a `Player` table with `(id: UUID, username: String, score: Int)`, produce:

  ```pseudo
  struct Player {
    UUID id;
    String username;
    Integer score;
    // (De)serialization annotations as needed)
  }
  ```

### 4.2 Reducer Argument/Return Models

* For each reducer function (e.g., `add_score(player_id: UUID, delta: Int) → Void`), generate:

  ```pseudo
  struct AddScoreArgs {
    UUID player_id;
    Integer delta;
  }
  ```

  and, if the reducer returns data, a `struct AddScoreResult { … }`.

### 4.3 Event Models

* Define generic event-wrapper structs for subscription events, e.g.:

  ```pseudo
  struct InsertEvent<RowType> {
    String type;            // “InsertEvent”
    UUID subscription_id;
    String table;           // table name
    RowType row;            // e.g., Player
  }

  struct UpdateEvent<RowType> {
    String type;            // “UpdateEvent”
    UUID subscription_id;
    String table;
    RowType old_row;
    RowType new_row;
  }

  struct DeleteEvent<RowType> {
    String type;            // “DeleteEvent”
    UUID subscription_id;
    String table;
    RowType row;
  }
  ```

### 4.4 SDK Responsibilities

* Provide model definitions (classes/structs) with proper annotations or methods to convert JSON objects to/from native types.
* Use code generation (e.g., via `spacetime generate --lang <lang>`) or manual templates to ensure models match exactly the server’s SATS-JSON schema.
* Validate at compile time (where possible) that field names and types align with the published database schema.

## 5. Utility & Error-Handling

All SDKs need foundational utilities for JSON processing, header management, and error interpretation.

### 5.1 JSON (De)Serialization

* Leverage a mature JSON library (e.g., `nlohmann/json` in C++, `encoding/json` in Go, `Newtonsoft.Json` in C#) to:

    * Parse incoming HTTP/WebSocket payloads into generic JSON or typed models.
    * Serialize request objects into JSON strings.

### 5.2 Header Injection & Configuration

* Abstract away adding `Content-Type: application/json` and `Authorization: Bearer <token>` on each HTTP call.
* Provide configuration options for base URL, timeouts, and TLS validation.

### 5.3 Error Wrapping

* On HTTP calls: if `status_code` ≠ 200, parse error JSON (if any) or return a generic HTTP error.
* On WebSocket frames: handle `{ "type":"Error", "code": <int>, "message": "<string>" }` by raising exceptions or invoking an error callback.

### 5.4 Concurrency & Thread Safety

* WebSocket receive loops often run on separate threads. Ensure that model callbacks and state mutations are thread-safe (e.g., using locks, queues, or event dispatchers).

## 6. Code Generation & Schema Discovery

Rather than hard-coding model definitions, most SDKs use the Spacetime CLI to auto-generate language-specific bindings.

### 6.1 Command

```
spacetime generate --lang <language> --module <module_name> --out <output_folder>
```

* Produces model files (table rows, reducer args/returns) and basic API stubs.
* Officially supported for Rust, C#, TypeScript; community SDKs (e.g., Go) often manually translate from these.

### 6.2 Use Cases

* Ensures every SDK has bit-accurate table types and reducer schemas.
* Minimizes drift between server schema changes and client models.

## 7. End-to-End Workflow Summary

Below is a high-level flow that any SDK must support, regardless of implementation language:

1. **Initialization**

    * SDK user instantiates a `Client` object with configuration (e.g., `baseUrl`, optional certificates, timeouts).

2. **Identity Generation / Login**

    * Call `createIdentity()` (POST to `/v1/identity`) → get `{ identity, token }`.
    * Store the returned `identity` and `token`.

3. **(Optional) Database Discovery**

    * Call `getDatabaseList(identity)` (GET `/v1/identity/:identity/databases`) to list accessible databases.

4. **Raw SQL over HTTP**

    * Call `executeSql(moduleName, sql, params)` (POST `/v1/database/:module/sql`) to run ad-hoc queries or mutations.

5. **Subscription Setup**

    * Generate WebSocket URL:

      ```
      ws://<host>:<port>/v1/database/<module>/ws?token=<token>
      ```
    * Dial WebSocket and await `{ "type":"Connected", "connection_id": ... }` frame.
    * Send:

      ```json
      { "type":"Subscribe", "query":"SELECT * FROM <table>" }
      ```
    * On receiving `{ "type":"Subscribed", "subscription_id": ... }`, begin read loop.
    * As each `InsertEvent`/`UpdateEvent`/`DeleteEvent` arrives, unmarshal into typed model and invoke user callback.

6. **Reducer Calls**

    * Over the same WebSocket, send:

      ```json
      {
        "type": "CallReducer",
        "reducer": "<name>",
        "args": { … }
      }
      ```
    * Wait (or register a callback) for a `{"type":"ReducerResult", …}` frame.

7. **Unsubscribe & Cleanup**

    * Send:

      ```json
      { "type":"Unsubscribe", "subscription_id":"<uuid>" }
      ```
    * Optionally, close or re-use WebSocket for new subscriptions. Finally, call `close()` on HTTP and WebSocket layers.

## 8. Canonical References

Below are the authoritative sources to consult when implementing a custom client SDK:

1. **SpacetimeDB HTTP Identity API**

    * `POST /v1/identity` → Returns `{identity, token}`.
    * Full docs: [Identity HTTP API](https://spacetimedb.com/docs/http/identity).

2. **SpacetimeDB HTTP Database API**

    * `POST /v1/database/:db/sql` → Run SQL; `GET /v1/database/:db/schema`.
    * Full docs: [Database HTTP API](https://spacetimedb.com/docs/http/database).

3. **SpacetimeDB WebSocket Subscription Reference**

    * Frame types: `Subscribe`, `Subscribed`, `InsertEvent`, `UpdateEvent`, `DeleteEvent`, `Unsubscribe`, `Unsubscribed`, `CallReducer`, `ReducerResult`, `Error`.
    * Full docs: [Subscriptions API](https://spacetimedb.com/docs/subscriptions).

4. **Official Client SDKs for Reference**

    * **Rust**: `spacetimedb-sdk` crate and docs at docs.rs.
    * **C#**: `com.clockworklabs.spacetimedbsdk` GitHub repo (NuGet package).
    * **TypeScript**: `@clockworklabs/spacetimedb-sdk` on npm.

5. **Client API Message Schemas (Rust Crate)**

    * JSON types for WebSocket frames (`IdentityToken`, `InsertEvent`, etc.) in `client-api-messages/src/websocket.rs`.

6. **Spacetime CLI “Generate” Tool**

    * `spacetime generate --lang <lang>` produces language-specific models. Use this to bootstrap model definitions rather than manually copying them.
