#pragma once

// SDK version
#include "Version.hpp"

// Core client & config
#include "Client.hpp"
#include "Config.hpp"

// Identity service
#include "Http/Endpoints/Identity/IdentityClient.hpp"
#include "Http/Endpoints/Identity/IdentityModels.hpp"

// Database service
#include "Http/Endpoints/Database/DatabaseClient.hpp"
#include "Http/Endpoints/Database/DatabaseModels.hpp"

// Utilities
#include "Http/HttpClient.hpp"
#include "Http/Json.hpp"
#include "Http/Error.hpp"

// Schema codegen/runtime
// #include "Schema/SchemaParser.hpp"

// (Optionally bring in generated schema types here)
// #include "Schema/Generated/MySchema.hpp"
