package main

import (
	"github.com/piotrek-szczygiel/raspberry-console/console"
)

func main() {
	c := console.New(3000, 2000)
	c.Start()
}
