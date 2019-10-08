package config

import (
	"github.com/BurntSushi/toml"
	"io/ioutil"
	"log"
)

type Config struct {
	Console    Console
	Controller Controller
}

type Console struct {
	Resolution [2]int32
	Fullscreen bool
	Stretch    bool
	VSync      bool
}

type Controller struct {
	Address string
}

func New(filename string) (config Config) {
	configData, err := ioutil.ReadFile(filename)
	if err != nil {
		log.Println("Unable to read", filename)
		return
	}

	if _, err := toml.Decode(string(configData), &config); err != nil {
		log.Println("Unable to decode", filename)
		return
	}

	log.Printf("Loaded config: %+v\n", config)
	return
}
