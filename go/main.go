package main

import (
    "bufio"
	"net"
    "strconv"
)

func main() {
        listen, err := net.Listen("tcp", ":"+strconv.Itoa(12345))
        if err == nil {
            conn, err := listen.Accept()
            if err == nil {
                for {
                    var r = bufio.NewReader(conn)
                    input, _, _ := r.ReadRune()
                    output := input + 1
                    conn.Write([]byte(string(output) + "\n"))
                }
            }
        }
}