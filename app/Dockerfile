# From base drogon framework
FROM drogonframework/drogon

# Default drogon port
EXPOSE 8848

# Installs the libraries to enable compilation
RUN apt update && \
    apt install --no-install-recommends -y \
    libmariadb-dev \
    autoconf \
    automake \
    libtool \
    curl \
    make \
    g++ \
    unzip && \
    apt-get clean && \
    rm -rf /var/lib/apt/lists/*

# Set workdir of project
RUN mkdir -p /drogon/app/build
WORKDIR /drogon/app/build
COPY . /drogon/app

# Compiling drogon app
RUN cmake .. \
    -DCMAKE_CXX_COMPILER=/usr/bin/g++ \
    -DCMAKE_CXX_FLAGS='-pthread -lpthread -fcoroutines -std=c++20' && \
    make

# Init main application
CMD echo "[+] Starting drogon app..." && \
    ./app
