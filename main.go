package main

import (
	"log"

	"github.com/piotrek-szczygiel/raspberry-console/console"
)

func main() {
	log.SetPrefix("console | ")
	log.Println("Starting the raspberry console")
	c := console.New()
	c.Start()
}
