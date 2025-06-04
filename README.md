# SpacetimeDB C++ SDK

> ⚠️ **Early-stage**

A modern, header-only (plus a small .cpp core) C++ client for SpacetimeDB, with HTTP, JSON and WebSocket 
support out of the box.

## Status

- Version: **0.1.0**  
- Library: `SpacetimeDbSdk` (static)  
- Dependencies: [libcpr](https://github.com/libcpr/cpr) (HTTP), [nlohmann/json](https://github.com/nlohmann/json) (JSON)  
- ⚠️ **This SDK is very early-stage** many pieces still need implementation, wiring and testing.

## 👹 Backlog

### Dev

- [x] **Project layout** (`include/`, `src/`, `tools/CodegenTool/`, `examples/`, `tests/`)
- [x] **HttpClient** wrapper around `cpr::cpr` (`Get`/`Post`, headers overload)
- [x] **Json** alias (`using Json = nlohmann::json;`)
- [ ] **Config** & **Client** classes with Builder pattern
- [X] **Umbrella header** (`SpacetimeDB.hpp`) and `version.hpp`
  
- ### Database Client
- [ ] `POST /v1/database`
- [ ] `POST /v1/database/:name_or_identity`
- [X] `GET /v1/database/:name_or_identity`
- [ ] `DELETE /v1/database/:name_or_identity`
- [ ] `GET /v1/database/:name_or_identity/names`
- [ ] `POST /v1/database/:name_or_identity/names`
- [ ] `PUT /v1/database/:name_or_identity/names`
- [ ] `GET /v1/database/:name_or_identity/identity`
- [ ] `GET /v1/database/:name_or_identity/subscribe`
- [X] `POST /v1/database/:name_or_identity/call/:reducer`
- [X] `GET /v1/database/:name_or_identity/schema`
- [ ] `GET /v1/database/:name_or_identity/logs`
- [ ] `POST /v1/database/:name_or_identity/sql`

### Spacetime Native, Schema codegen integration 
- [ ] Code Generation from Schema (`GET /v1/database/{database}/schema`)
- [ ] Hook `tools/CodegenTool` into the build so that `include/Schema/Generated/` is always up-to-date
- [ ] Generate Native Coding from Schemas
- [ ] **WebSocket client**: Keep Spacetime Algebraic Types Fed From Subscriptions
- [x] **Identity Client + IdentityModels**: All Requests parsed to Native C++

### Complete Examples
- [ ] **Examples**: `quickstart.cpp`
- [ ] **Examples**: `subscribe.cpp`)

### Unit tests & coverage
- [X] **Tests**: Identity
- [ ] **Tests**: Database


### Other
- [ ] **Internal APIs**
    - Move implementation-only helpers into `SpacetimeDb::detail` namespace
    - Consider **Pimpl** in public classes for ABI stability
- [ ] **Asynchronous support**
    - Add `async` overloads returning promises (e.g. `std::future<Json>`) or callbacks
- [ ] **CMake install & export**
    - Add `install(TARGETS … EXPORT …)`
    - Generate `SpacetimeDbSdkConfig.cmake` for `find_package(SpacetimeDbSdk CONFIG)`
- [ ] **Unit tests & coverage**
    - Mock `HttpClient` to verify request paths, payloads, headers, and JSON marshalling
- [ ] **Documentation**
    - Populate `docs/api.md` with class/method overviews and usage snippets
    - Add Doxygen comments in headers
- [ ] **CI/CD**
    - Set up GitHub Actions (or similar) for build, test, code-style, and codegen checks

---

Feel free to pick any of the “Next Steps” and submit a PR. Let’s get this SDK from prototype to production!  
