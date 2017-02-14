FROM ubuntu:16.10

###
#   Install dependencies
###
RUN apt-get update && apt-get install git g++ pkg-config libicu-dev libdcmtk-dev libboost-dev libboost-test-dev \
    libboost-system-dev python3 python3-dev libboost-python-dev libboost-filesystem-dev liblog4cpp5-dev libjsoncpp-dev cmake -y


###
#   Get code source and build the lib
###

COPY . /home/odil/src 
RUN mkdir /home/odil/build && \
    cd /home/odil/build && \
    cmake ../src -DBUILD_PYTHON35=ON -DBUILD_TESTING=OFF -DBUILD_EXAMPLES=OFF && \
    make . && \
    make install && \
    ln -s /usr/local/lib/libodil.so.0 /usr/lib/libodil.so.0

