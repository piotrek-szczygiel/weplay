package main

import (
	"github.com/piotrek-szczygiel/raspberry-console/console"
	"log"
)

func main() {
	log.SetPrefix("console | ")
	log.Println("Starting the raspberry console")
	c := console.New()
	c.Start()
}
