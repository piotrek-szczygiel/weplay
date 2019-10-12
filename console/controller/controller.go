package controller

import (
	"bufio"
	"log"
	"net"
	"os"

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

	defer func() {
		if err := l.Close(); err != nil {
			log.Println("Unable to close server")
		}
	}()

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

	greeting, err := r.ReadBytes('\n')
	if err != nil {
		log.Println("Error while reading greeting:", err)
		return
	}

	if greeting[0] != 0x19 || greeting[1] != 0x84 {
		log.Println("Invalid controller greeting:")
		return
	}

	if greeting[2] != 0x01 {
		log.Println("Invalid controller protocol version:", greeting[2])
		return
	}

	for {
		bytes, err := r.ReadBytes('\n')
		if err != nil {
			log.Println("Error while reading:", err)
			return
		}

		var eventType EventType
		var eventKey EventKey

		if bytes[0] == 'b' {
			id := bytes[1]
			state := bytes[2]

			switch id {
			case '<':
				eventKey = Left
			case '^':
				eventKey = Middle
			case '>':
				eventKey = Right
			default:
				log.Println("Invalid button id:", id)
				return
			}

			switch state {
			case 'd':
				eventType = Down
			case 'u':
				eventType = Up
			default:
				log.Println("Invalid button state:", state)
				return
			}
		}

		// TODO: separate controller ids
		event <- Event{eventType, eventKey}
	}
}
