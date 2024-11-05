FROM malusevd99/php-ext-dev:8.1

ARG CPP_DRIVER_VERSION=2.17.1

ENV EXT_CASSANDRA_VERSION=master
ENV LDFLAGS="-L/usr/local/lib"
ENV LIBS="-lssl -lz -luv -lm -lgmp -lstdc++"

RUN php -r "copy('https://getcomposer.org/installer', 'composer-setup.php');" && \
    php composer-setup.php && php -r "unlink('composer-setup.php');" \
    mv composer.phar /bin/composer

COPY --from=mlocati/php-extension-installer /usr/bin/install-php-extensions /usr/local/bin

RUN docker-php-source extract \
    && apt update -y \
    && mkdir -p /cpp-driver \
    && apt install \
        cmake \
        unzip \
        mlocate \
        build-essential \
        git \
        libuv1-dev \
        libssl-dev \
        libgmp-dev \
        zlib1g-dev \
        openssl \
        libpcre3-dev -y \
    && git clone --recursive https://github.com/datastax/cpp-driver /cpp-driver \
    && cd /cpp-driver && git checkout tags/$CPP_DRIVER_VERSION -b v$CPP_DRIVER_VERSION \
    && mkdir -p build && cd build \
    && cmake \
         -DCASS_CPP_STANDARD=17 \
         -DCASS_BUILD_STATIC=ON \
         -DCASS_BUILD_SHARED=ON \
         -DCASS_USE_TIMERFD=ON \
         -DCASS_USE_LIBSSH2=ON \
         -DCASS_USE_ZLIB=ON \
         -DCMAKE_C_FLAGS="-fPIC" \
         -DCMAKE_CXX_FLAGS="-fPIC \
         -Wno-error=redundant-move" \
         -DCMAKE_BUILD_TYPE="RelWithInfo" \
         -DCMAKE_INSTALL_PREFIX=/usr/local \
         -DCMAKE_INSTALL_LIBDIR=lib 
         -DCASS_INSTALL_PKG_CONFIG=ON .. \
    && make -j8 && make install \
    && install-php-extensions intl zip pcntl gmp ast xdebug

CMD ["bash"]
