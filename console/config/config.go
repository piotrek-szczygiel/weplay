package config

import (
	"io/ioutil"
	"log"
	"os"

	"github.com/BurntSushi/toml"
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
	ShowFPS    bool
}

type Controller struct {
	Address string
}

func New(filename string) Config {
	config, err := Load(filename)
	if err != nil {
		config = Config{
			Console{
				Resolution: [2]int32{1920, 1080},
				Fullscreen: false,
				Stretch:    false,
				VSync:      true,
				ShowFPS:    true,
			},
			Controller{
				Address: "127.0.0.1:1984",
			},
		}

		config.Save(filename)
	}

	return config
}

func Load(filename string) (Config, error) {
	configData, err := ioutil.ReadFile(filename)
	if err != nil {
		log.Println("Unable to read", filename)
		return Config{}, err
	}

	var config Config
	if _, err := toml.Decode(string(configData), &config); err != nil {
		log.Println("Unable to decode", filename)
		return Config{}, err
	}

	log.Printf("Loaded config: %+v\n", config)
	return config, nil
}

func (config Config) Save(filename string) {
	f, err := os.Create(filename)
	if err != nil {
		log.Printf("Error while saving config to '%s': %v\n", filename, err)
		return
	}

	t := toml.NewEncoder(f)
	if err := t.Encode(config); err != nil {
		log.Println("Error while encoding config:", err)
	}
}
