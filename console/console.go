package console

import (
	"log"
	"math/rand"
	"time"

	"github.com/piotrek-szczygiel/raspberry-console/console/pong"

	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/config"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

type Console struct {
	config  config.Config
	stretch bool
	events  chan controller.Event
	state   State
}

type State interface {
	Update([]controller.Event)
	Draw()
	GetTarget() rl.RenderTexture2D
}

func New() (c Console) {
	c.config = config.New("config.toml")

	c.stretch = c.config.Console.Stretch

	if c.config.Console.Fullscreen {
		rl.SetConfigFlags(rl.FlagFullscreenMode)
	} else {
		rl.SetConfigFlags(rl.FlagWindowResizable)
	}

	if c.config.Console.VSync {
		rl.SetConfigFlags(rl.FlagVsyncHint)
	}

	w := c.config.Console.Resolution[0]
	h := c.config.Console.Resolution[1]
	rl.InitWindow(w, h, "Raspberry Console")

	log.Println("Initialized window")

	rand.Seed(time.Now().UnixNano())

	c.events = make(chan controller.Event, 10)
	c.state = pong.New()
	return c
}

func (c *Console) Start() {
	con := controller.New(c.config.Controller)
	go con.Start(c.events)
	c.loop()
}

func (c *Console) loop() {
	for !rl.WindowShouldClose() {
		c.update()
		c.draw()
	}
}

func (c *Console) update() {
	var events []controller.Event
loop:
	for {
		select {
		case event := <-c.events:
			events = append(events, event)
		default:
			break loop
		}
	}

	c.state.Update(events)
}

func (c *Console) draw() {
	rl.BeginDrawing()
	rl.ClearBackground(rl.Black)

	c.state.Draw()

	target := c.state.GetTarget()

	var dest rl.Rectangle
	if c.stretch {
		dest = rl.NewRectangle(0, 0, float32(rl.GetScreenWidth()), float32(rl.GetScreenHeight()))
	} else {
		scale := math32.Min(
			float32(rl.GetScreenWidth())/float32(target.Texture.Width),
			float32(rl.GetScreenHeight())/float32(target.Texture.Height),
		)

		dest = rl.NewRectangle(
			(float32(rl.GetScreenWidth())-float32(target.Texture.Width)*scale)*0.5,
			(float32(rl.GetScreenHeight())-float32(target.Texture.Height)*scale)*0.5,
			float32(target.Texture.Width)*scale,
			float32(target.Texture.Height)*scale,
		)
	}

	rl.DrawTexturePro(
		target.Texture,
		rl.NewRectangle(0, 0, float32(target.Texture.Width), float32(-target.Texture.Height)),
		dest,
		rl.Vector2{},
		0,
		rl.White,
	)

	if c.config.Console.ShowFPS {
		rl.DrawFPS(5, 5)
	}

	rl.EndDrawing()
}
