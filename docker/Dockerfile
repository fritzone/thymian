#Download base image ubuntu 20.04
FROM ubuntu:20.04
ARG DEBIAN_FRONTEND=noninteractive
RUN apt update
RUN apt install -y libz-dev libssl-dev libtool cmake autoconf g++ libsqlite3-dev libpython3-dev libpython2-dev git libboost-dev libsasl2-dev libgsasl7-dev \
                   sendmail sendmail-bin libgnutls28-dev mc vim sudo nano python-dev python-setuptools libboost-filesystem-dev zip

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

# RUN mkdir -p /thymian/build/server/theme && ln -s /thymian/src/server/current /thymian/build/server/theme/
# RUN ln -s /thymian/src/server/recipes /thymian/build/server/
# RUN ln -s /thymian/src/server/pdfs /thymian/build/server/
# RUN cp /thymian/src/server/lang.db /thymian/build/server/

COPY log_conf.xml /thymian/build/server/
CMD cd /thymian/build/server/ && ./thymian_exe
EXPOSE 8999
