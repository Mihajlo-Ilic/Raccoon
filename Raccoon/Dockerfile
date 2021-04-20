FROM ubuntu:latest

ARG    DEBIAN_FRONTEND=noninteractive

RUN apt-get update -y && \
    apt-get install build-essential -y && \
    apt-get install gcc g++ -y && \
    apt-get install cmake -y && \
    apt-get install qt5-default -y && \
    apt install libqt5charts5 libqt5charts5-dev -y && \
    apt install libqt5datavisualization5 libqt5datavisualization5-dev -y 


COPY . /usr/src/raccoon
WORKDIR /usr/src/raccoon

RUN mkdir build && \
    cd build && \
    cmake .. && \
    make

CMD [ "./build/raccoon"]



