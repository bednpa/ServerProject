# ─── Stage 1: Builder ────────────────────────────────────────────────────────
# Installs the full toolchain and compiles the project. This stage is large
# but is discarded after the binary is extracted — it never ships to production.
FROM ubuntu:24.04 AS builder

ENV DEBIAN_FRONTEND=noninteractive

RUN apt-get update && apt-get install -y \
    cmake \
    ninja-build \
    clang \
    libboost-all-dev \
    nlohmann-json3-dev \
    && rm -rf /var/lib/apt/lists/*

WORKDIR /src

# Copy the source code into the image
COPY . .

# Configure using the release_without_tests preset.
# -DCLANG_TIDY=OFF disables clang-tidy — it is a development tool and is not
# needed (or desired) in a production build pipeline.
RUN cmake --preset release_without_tests -DCLANG_TIDY=OFF \
    && cmake --build build/release_without_tests --parallel "$(nproc)"


# ─── Stage 2: Runtime ────────────────────────────────────────────────────────
# Minimal image that contains only the compiled binary and its runtime
# dependencies. No compiler, no headers, no build tools.
FROM ubuntu:24.04 AS runtime

WORKDIR /app

# Copy only the compiled binary from the builder stage
COPY --from=builder /src/build/release_without_tests/ServerProject ./ServerProject

EXPOSE 8080

CMD ["./ServerProject"]