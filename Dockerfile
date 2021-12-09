FROM debian:bullseye-slim as build
RUN apt-get update && apt-get install -y build-essential qtbase5-dev
COPY . .
WORKDIR /src
RUN qmake; make

FROM debian:bullseye-slim
RUN apt-get update && apt-get install -y libqt5gui5 libqt5widgets5 libqt5opengl5 && rm -rf /var/lib/apt/lists/*
COPY --from=build /src/DGLshow /usr/bin/DGLshow

CMD ["DGLshow"]