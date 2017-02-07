FROM ubuntu:16.10

###
#   Install dependencies
###
RUN apt-get update && apt-get install git g++ pkg-config libicu-dev libdcmtk-dev libboost-dev libboost-test-dev \
    libboost-system-dev libboost-python-dev libboost-filesystem-dev python liblog4cpp5-dev libjsoncpp-dev cmake -y

###
#   Get code source and build the lib
###
RUN git clone https://github.com/lamyj/odil.git /home/odil/src && \
    mkdir /home/odil/build && \
    cd /home/odil/build && \
    cmake ../src && \
    make . && \
    make install && \
    ln -s /usr/local/lib/libodil.so.0 /usr/lib/libodil.so.0