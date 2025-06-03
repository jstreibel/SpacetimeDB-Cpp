# SpacetimeDB C++ SDK

> ⚠️ **Early-stage — not yet functional!**

A modern, header-only (plus a small .cpp core) C++ client for SpacetimeDB, with HTTP and JSON support out of the box.

## Status

- Version: **0.1.0**  
- Library: `SpacetimeDbSdk` (static)  
- Dependencies: [libcpr](https://github.com/libcpr/cpr) (HTTP), [nlohmann/json](https://github.com/nlohmann/json) (JSON)  
- ⚠️ **This SDK is very early-stage** and **not yet functional**—many pieces still need implementation, wiring and testing.

## 👹 Backlog

- [x] **Project layout** (`include/`, `src/`, `tools/CodegenTool/`, `examples/`, `tests/`)
- [x] **HttpClient** wrapper around `cpr::cpr` (`Get`/`Post`, headers overload)
- [x] **Json** alias (`using Json = nlohmann::json;`)
- [x] **Config** & **Client** classes with Builder pattern
- [x] **Umbrella header** (`SpacetimeDB.hpp`) and `version.hpp`
- [x] **IdentityClient** + **IdentityModels** (`CreateIdentityRequest`, `IdentityInfo`, etc)
- [ ] **DatabaseClient** + **DatabaseModels** (`ExecuteSqlRequest`, `ExecuteSqlResponse`, `PublishDocumentRequest`)
- [ ] **Examples** (`quickstart.cpp`, `subscribe.cpp`)
- [ ] **Tests** (`IdentityTests.cpp`, `DatabaseTests.cpp`, `UtilsTests.cpp`)
- [ ] **Internal APIs**
    - Move implementation-only helpers into `SpacetimeDb::detail` namespace
    - Consider **Pimpl** in public classes for ABI stability
- [ ] **Asynchronous support**
    - Add `async` overloads returning `std::future<Json>` (or callbacks)
- [ ] **CMake install & export**
    - Add `install(TARGETS … EXPORT …)`
    - Generate `SpacetimeDbSdkConfig.cmake` for `find_package(SpacetimeDbSdk CONFIG)`
- [ ] **Schema codegen integration**
    - Add `tools/CodegenTool` to generate `include/Schema/Generated/` from `GET /v1/database/{database}/schema`
- [ ] **Schema codegen integration**
    - Hook `tools/CodegenTool` into the build so that `include/Schema/Generated/` is always up-to-date
- [ ] **WebSocket client**
    - Implement `WebSocketClient` service stub under `include/SpacetimeDb/WebSocket/`
- [ ] **Reducer client**
    - Add `ReducerClient` + `ReducerModels` for the `CallReducer` API
- [ ] **Complete examples**
    - Flesh out `quickstart.cpp` and `subscribe.cpp` with real endpoints and CLI flags
- [ ] **Unit tests & coverage**
    - Mock `HttpClient` to verify request paths, payloads, headers, and JSON marshalling
- [ ] **Documentation**
    - Populate `docs/api.md` with class/method overviews and usage snippets
    - Add Doxygen comments in headers
- [ ] **CI/CD**
    - Set up GitHub Actions (or similar) for build, test, code-style, and codegen checks

---

Feel free to pick any of the “Next Steps” and submit a PR. Let’s get this SDK from prototype to production!  
