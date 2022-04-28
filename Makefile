default: c

all: c go

c: client-c-sol server-c-sol # server-c client-c

go: client-go-sol server-go-sol #client-go server-go

client-c: client-c.c
	gcc-9 client-c.c -o client-c

server-c: server-c.c
	gcc-9 server-c.c -o server-c

client-c-sol: client-c-sol.c
	gcc-9 client-c-sol.c -o client-c
server-c-sol: server-c-sol.c
	gcc-9 server-c-sol.c -o server-c
	gcc-9 server-c-sol.c -o server-c-Copy
	gcc-9 router-c.c -o router-c

client-go: client-go.go
	go build client-go.go

server-go: server-go.go
	go build server-go.go

client-go-sol: client-go-sol.go
	go build -o client-go client-go-sol.go

server-go-sol: server-go-sol.go
	go build -o server-go server-go-sol.go

clean:
	rm -f server-c client-c server-go client-go router-c server-c-Copy *.o
