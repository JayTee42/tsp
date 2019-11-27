# minimal Dockerfile for testing the tsp solver

FROM gcc:latest

RUN apt update && apt install -y \
    cmake

COPY . /tsp
RUN cd /tsp \
    && mkdir build \
    && cd build \
    && cmake -DCMAKE_BUILD_TYPE=Release .. \
    && make \
    && make install

CMD ["tsp"]