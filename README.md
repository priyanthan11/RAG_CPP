Using vcpkg (Recommended on Windows)

vcpkg automates installing libraries like curl, zlib, OpenSSL, etc., and integrates easily with CMake.

Step 1: Install vcpkg
git clone https://github.com/microsoft/vcpkg.git
cd vcpkg
.\bootstrap-vcpkg.bat

Step 2: Install curl
.\vcpkg install curl:x64-windows

This installs libcurl and dependencies (OpenSSL, zlib) for 64-bit Windows.

Step 3: Integrate vcpkg with CMake
.\vcpkg integrate install

This allows CMake to find curl automatically.

Step 4: Update your CMakeLists.txt

# Find curl package

find_package(CURL REQUIRED)

target_link_libraries(rag_app PRIVATE CURL::libcurl)

Now #include <curl/curl.h> works, and linking is automatic.

Manual Prebuilt libcurl

If you prefer manual setup, you can download prebuilt binaries:

Go to https://curl.se/windows/
→ download a Windows 64-bit Visual Studio build.

Extract to external/libcurl/ folder:

rag-cpp/
├─ external/
│ └─ libcurl/
│ ├─ include/ (contains curl/\*.h)
│ └─ lib/ (contains curl.lib and dlls)

Update CMakeLists.txt:

# Add include path

target_include_directories(rag_app PUBLIC ${PROJECT_SOURCE_DIR}/external/libcurl/include)

# Add lib path

link_directories(${PROJECT_SOURCE_DIR}/external/libcurl/lib)

# Link libcurl

target_link_libraries(rag_app PRIVATE libcurl.lib ws2_32.lib winmm.lib wldap32.lib)

Note: On Windows, libcurl depends on ws2_32.lib, winmm.lib, wldap32.lib — you must link them.

Make sure libcurl.dll is in your executable folder or PATH.

# Retrieval-Augmented Generation System in C++

A fully local Retrieval-Augmented Generation (RAG) system implemented in C++ from the ground up.
This project integrates a custom C++ HTTP client, a vector search pipeline, a Python embedding server, and a local LLM running through Ollama.
The entire request flow — from ingestion to retrieval to generation — is implemented with no external cloud dependencies.

This is a pure C++ RAG pipeline, built for performance, full control, and transparency.

## Key Features

## Core RAG Engine (C++)

- Full RAG pipeline implemented in modern C++
- Custom HTTP communication using libcurl
- JSON handling with nlohmann::json
- Chunked text ingestion
- Embedding retrieval via FastAPI backend
- Vector similarity search (cosine similarity)
- Local LLM generation through Ollama's HTTP API
- Proper error handling and fail-safes
- Clean, reusable C++ class architecture

# Embeddings (Python API)

- Python FastAPI microservice providing embeddings
- Arctic-Embed (or any SentenceTransformer) as the embedding model
- Vector output returned as raw float arrays to C++
- Designed to run locally and respond at high throughput

Local LLM Inference (Ollama)

- Uses `http://127.0.0.1:11434/api/generate`
- Supports any Ollama-compatible model (Llama, Mistral, Gemma, etc)
- API requests fully handled in C++
- Response text streamed and parsed safely

## System Architecture

```
          ┌─────────────────────┐
          │      User Query     │
          └──────────┬──────────┘
                     │
                     ▼
 ┌─────────────────────────────────────────┐
 │         C++ RAG Engine (main.cpp)       │
 │  - Preprocess query                     │
 │  - Request embedding from Python API    │
 │  - Vector similarity search             │
 │  - Construct augmented prompt           │
 │  - Send to Ollama                       │
 │  - Parse and return final answer        │
 └─────────────────────────────────────────┘
                     │
       ┌─────────────┴─────────────┐
       ▼                           ▼
┌───────────────┐         ┌────────────────┐
│ Python API    │         │  Vector Store  │
│ Arctic-Embed  │         │   (C++/JSON)   │
│ FastAPI server│         │ Cosine Search  │
└───────────────┘         └────────────────┘
                     │
                     ▼
          ┌─────────────────────┐
          │    Ollama LLM       │
          └─────────────────────┘
```

## Folder Structure

```
RAG_v.2/
│   main.cpp
│   CMakeLists.txt
│   README.md
│
├── api/
│   └── embed_api.py         (FastAPI embedding service)
│
├── include/
│   ├── embedding_http.h
│   ├── rag_engine.h
│   ├── vector_math.h
│   └── utils.h
│
├── src/
│   ├── embedding_http.cpp
│   ├── rag_engine.cpp
│   ├── vector_math.cpp
│   └── utils.cpp
│
├── data/
│   ├── chunks.json          (preprocessed text chunks)
│   ├── vectors.json         (embeddings of chunks)
│   └── documents/           (raw files)
│
├── temp/
│
├── .gitignore               (configured to exclude build, temp, venv etc)
└── build/                   (ignored)

```

## Ignored via `.gitignore:`

_`build/`
_`llama.cpp/`
_`temp/`
_`arctic-embed/` \*`.vscode/`

## Dependencies

- C++
- CMake
- libcurl
- nlohmann/json

## Python (Embedding API)

- Python 3.10+
- FastAPI
- Uvicorn
- sentence-transformers or Arctic-Embed
- NumPy

## Local LLM

- Ollama
  (Start server automatically by running ollama serve)

## Building

```
- mkdir build
- cd build
- cmake ..
- cmake --build . --config Release

```

## Running the Full Pipeline

# Step 1: Start embedding server

`cd api
uvicorn embed_api:app --port 8000`

# Step 2: Start Ollama

`ollama serve`

# Step 3: Run the C++ app

`./RAG_v2`

## Important C++ Code Behavior

# Embedding Request in C++

```
nlohmann::json j = nlohmann::json::parse(readBuffer);

if (j.contains("embedding"))
    return j["embedding"].get<std::vector<float>>();

throw std::runtime_error("Invalid embedding response");
```

# Ollama LLM Request in C++

```
if (j.contains("text"))
    return j["text"].get<std::string>();

if (j.contains("outputs") && j["outputs"].is_array() && !j["outputs"].empty())
    return j["outputs"][0].get<std::string>();

throw std::runtime_error("invalid LLM response");
```

# Error Handling

Errors are printed cleanly:

```
catch (const std::exception& e)
{
    std::cout << "[SYS]: Exception at Generate method: " << e.what() << std::endl;
}
```

How Retrieval Works

1. User query → embed using Python server
2. Compare query embedding with stored chunk embeddings using cosine similarity
3. Choose top-K relevant chunks
4. Build augmented prompt
5. Send to Ollama for final answer
6. Stream and parse response in C++
7. Return final answer to user

```

```
