#
# The entire thing should be run as:
#
# sudo docker run -d --name thymian --privileged -p 8999:8999 -v /home/ferencd/work/thymian/src/server/recipes:/thymian/build/server/recipes \
#                                                                       -v /home/ferencd/work/thymian/server/pdfs:/thymian/build/server/pdfs \
#                                                                       -v /home/ferencd/work/thymian/src/server/current:/thymian/build/server/theme/current \
#                                                                       -v /home/ferencd/work/thymian/src/server/lang.db:/thymian/build/server/lang.db 
#                                                                       -v /var/run/docker.sock:/var/run/docker.sock 
#                                                                       thymian:latest
#
# Or add -it --entrypoint /bin/bash (and remove -d, --name) to run it as a interactive terminal
#

#Download base image ubuntu 20.04
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y libz-dev libssl-dev libtool cmake autoconf g++ libsqlite3-dev libpython3-dev libpython2-dev git libboost-dev libsasl2-dev libgsasl7-dev \
                   sendmail sendmail-bin libgnutls28-dev mc vim sudo nano python-dev python-setuptools libboost-filesystem-dev zip ssh openssh-server

RUN  systemctl ssh start
RUN  systemctl ssh enable

RUN  NUM_CPUS=`nproc --all` && git clone https://github.com/fritzone/tntbinary.git                                   && \
     cd tntbinary                                                                                                    && \
     tar xf cxxtools-2.2.1.tar.gz                                                                                    && \
     cd cxxtools-2.2.1                                                                                               && \
     ./configure && make -j$NUM_CPUS && make install                                                                 && \
     cd ..                                                                                                           && \
     ldconfig                                                                                                        && \
     tar xf tntdb-1.3.tar.gz                                                                                         && \
     cd tntdb-1.3                                                                                                    && \
     ./configure --with-sqlite --without-mysql --without-postgresql                                                  && \
     make -j$NUM_CPUS && make install                                                                                && \
     cd ..                                                                                                           && \
     tar xf tntnet-2.2.1.tar.gz                                                                                      && \
     cd tntnet-2.2.1                                                                                                 && \
     ./configure && make -j$NUM_CPUS && make install                                                                 && \
     ldconfig                                                                                                        


RUN cd / && git clone https://github.com/fritzone/thymian.git                                                                         && \
    cd thymian/src/3rdparty/vmime && mkdir build && cd build && cmake .. -DVMIME_BUILD_SAMPLES=OFF -DCMAKE_BUILD_TYPE=Release && \
    make && make install && ldconfig && cd /                                                                                  && \
    cd thymian && mkdir build && cd build && cmake ../src -DENABLE_TESTS=OFF && make

# Recipes, html/js/img and PDFS are hosted on the host
RUN rm -rf /thymian/src/server/recipes
RUN rm -rf /thymian/src/server/pdfs
RUN rm -rf /thymian/src/server/current

# this enables the logging of the server
COPY log_conf.xml /thymian/build/server/

# this 
CMD cd /thymian/build/server/ && ./thymian_exe
EXPOSE 8999
