package console

import (
	rl "github.com/gen2brain/raylib-go/raylib"
	"math"
)

type Console struct {
	width   int32
	height  int32
	target  rl.RenderTexture2D
	stretch bool
}

func New(width, height int32) Console {
	var c Console
	c.width, c.height = 800, 600
	c.stretch = false

	rl.SetConfigFlags(rl.FlagFullscreenMode)
	rl.InitWindow(width, height, "Raspberry Console")

	c.target = rl.LoadRenderTexture(c.width, c.height)
	rl.SetTextureFilter(c.target.Texture, rl.FilterPoint)

	return c
}

func (c *Console) Start() {
	for !rl.WindowShouldClose() {
		c.update()
		c.draw()
	}
}

func (c *Console) update() {

}

func (c *Console) draw() {
	rl.BeginDrawing()
	rl.ClearBackground(rl.Black)

	rl.BeginTextureMode(c.target)
	rl.ClearBackground(rl.RayWhite)
	{
		rl.DrawFPS(10, 10)
	}
	rl.EndTextureMode()

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

	rl.DrawTexturePro(
		c.target.Texture,
		rl.NewRectangle(0, 0, float32(c.target.Texture.Width), float32(-c.target.Texture.Height)),
		dest,
		rl.Vector2{0, 0},
		0,
		rl.White)
	rl.EndDrawing()
}
