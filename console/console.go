package console

import (
	"github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/config"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	"log"
	"math"
)

type Console struct {
	config  config.Config
	width   int32
	height  int32
	target  rl.RenderTexture2D
	stretch bool
	events  chan controller.Event
}

func New(width, height int32) (c Console) {
	c.config = config.New("config.toml")

	c.width, c.height = 800, 600
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

	c.target = rl.LoadRenderTexture(c.width, c.height)
	rl.SetTextureFilter(c.target.Texture, rl.FilterPoint)

	log.Println("Initialized window")

	c.events = make(chan controller.Event)
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
		c.beginDraw()
		c.draw()
		c.endDraw()
	}
}

func (c *Console) update() {
	select {
	case event, ok := <-c.events:
		if ok {
			log.Printf("Received event: %+v\n", event)
		}
	default:

	}
}

func (c *Console) draw() {
	rl.DrawFPS(10, 10)
}

func (c *Console) beginDraw() {
	rl.BeginDrawing()
	rl.ClearBackground(rl.Black)
	rl.BeginTextureMode(c.target)
	rl.ClearBackground(rl.RayWhite)
}

func (c *Console) endDraw() {
	var dest rl.Rectangle
	if c.stretch {
		dest = rl.NewRectangle(0, 0, float32(rl.GetScreenWidth()), float32(rl.GetScreenHeight()))
	} else {
		scale := float32(math.Min(
			float64(rl.GetScreenWidth())/float64(c.width),
			float64(rl.GetScreenHeight())/float64(c.height)))

		dest = rl.NewRectangle(
			(float32(rl.GetScreenWidth())-float32(c.width)*scale)*0.5,
			(float32(rl.GetScreenHeight())-float32(c.height)*scale)*0.5,
			float32(c.width)*scale,
			float32(c.height)*scale)
	}

	rl.EndTextureMode()
	rl.DrawTexturePro(
		c.target.Texture,
		rl.NewRectangle(0, 0, float32(c.target.Texture.Width), float32(-c.target.Texture.Height)),
		dest,
		rl.Vector2{0, 0},
		0,
		rl.White)
	rl.EndDrawing()
}
