FROM sostheim/gcc-dev-env:latest AS build
COPY . ./src/
RUN set -ex; \
    cd src; \
    make static; \
    make install

FROM debian:stretch-slim
EXPOSE 8092
COPY --from=build /usr/local/crt/bin/crt /usr/local/crt/bin/crt
COPY --from=build /var/log/crt/ /var/log/crt/
ENTRYPOINT [ "/usr/local/crt/bin/crt", "-d", "-r", "-l information" ]
