package controller

import (
	"bufio"
	"log"
	"net"
	"os"
	"strings"

	"github.com/piotrek-szczygiel/raspberry-console/console/config"
)

type Controller struct {
	config config.Controller
}

type Event struct {
	Type EventType
	Key  EventKey
}

type EventType int

const (
	Up EventType = iota
	Down
)

type EventKey int

const (
	Left EventKey = iota
	Middle
	Right
)

func New(config config.Controller) (controller Controller) {
	controller.config = config

	return
}

func (c *Controller) Start(event chan Event) {
	log.Println("Starting controller server")

	l, err := net.Listen("tcp", c.config.Address)
	if err != nil {
		log.Println("Unable to start controller server:", err)
		os.Exit(1)
	}

	defer l.Close()

	log.Println("Controller server listening on", c.config.Address)

	for {
		conn, err := l.Accept()
		if err != nil {
			log.Println("Error accepting", err)
			continue
		}

		go handleController(conn, event)
	}
}

func handleController(conn net.Conn, event chan Event) {
	log.Println("Controller connected", conn.RemoteAddr())
	r := bufio.NewReader(conn)

	for {
		bytes, err := r.ReadBytes('\n')
		if err != nil {
			log.Println("Error while reading:", err)
			break
		}

		s := strings.Split(strings.Trim(string(bytes), "\r\n"), " ")
		if len(s) != 2 {
			log.Println("Invalid message:", bytes)
			break
		}

		var eventType EventType
		switch s[1] {
		case "up":
			eventType = Up
		case "down":
			eventType = Down
		}

		var eventKey EventKey
		switch s[0] {
		case "left":
			eventKey = Left
		case "middle":
			eventKey = Middle
		case "right":
			eventKey = Right
		}

		// TODO: separate controller ids
		event <- Event{eventType, eventKey}
	}
}
