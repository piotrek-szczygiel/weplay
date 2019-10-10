package demo

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

type Demo struct {
	ship    ship
	camera  camera
	columns []column

	pressed        [3]bool
	renderDistance float32

	target rl.RenderTexture2D
}

func New() *Demo {
	var demo Demo

	demo.ship = ship{}
	demo.camera = newCamera()
	demo.columns = generateColumns(1_000, 10_000, 40)

	demo.renderDistance = 500

	demo.target = rl.LoadRenderTexture(int32(rl.GetScreenWidth()), int32(rl.GetScreenHeight()))
	rl.SetTextureFilter(demo.target.Texture, rl.FilterAnisotropic4x)

	return &demo
}

func (demo *Demo) Update(events []controller.Event) {
	for _, event := range events {
		switch event.Type {
		case controller.Down:
			demo.pressed[event.Key] = true
		case controller.Up:
			demo.pressed[event.Key] = false
		}
	}

	dt := rl.GetFrameTime()
	horizontalSpeed := dt * 50
	verticalSpeed := dt * 25
	rotationSpeed := dt * 2

	var back, front, left, right, up, down float32

	if rl.IsKeyDown(rl.KeyS) {
		back = horizontalSpeed
	}

	if demo.pressed[controller.Middle] || rl.IsKeyDown(rl.KeyW) {
		front = horizontalSpeed
	}

	if demo.pressed[controller.Left] || rl.IsKeyDown(rl.KeyA) {
		left = horizontalSpeed
	}

	if demo.pressed[controller.Right] || rl.IsKeyDown(rl.KeyD) {
		right = horizontalSpeed
	}

	if rl.IsKeyDown(rl.KeyZ) {
		up = verticalSpeed
	}

	if rl.IsKeyDown(rl.KeyX) {
		down = verticalSpeed
	}

	if rl.IsKeyDown(rl.KeyLeft) {
		demo.camera.rotateHorizontal(rotationSpeed)
	}

	if rl.IsKeyDown(rl.KeyRight) {
		demo.camera.rotateHorizontal(-rotationSpeed)
	}

	if rl.IsKeyDown(rl.KeyUp) {
		demo.camera.rotateVertical(rotationSpeed)
	}

	if rl.IsKeyDown(rl.KeyDown) {
		demo.camera.rotateVertical(-rotationSpeed)
	}

	if rl.IsKeyDown(rl.KeyComma) {
		demo.camera.rotateDiagonal(-rotationSpeed)
	}

	if rl.IsKeyDown(rl.KeyPeriod) {
		demo.camera.rotateDiagonal(rotationSpeed)
	}

	demo.camera.update(back, front, left, right, up, down)
}

func (demo *Demo) Draw() {
	rl.BeginTextureMode(demo.target)
	rl.ClearBackground(rl.Black)
	rl.BeginMode3D(demo.camera.c)

	rl.DrawPlane(rl.Vector3{X: 5000}, rl.Vector2{X: 10000, Y: 50}, rl.DarkGray)

	columnsDrew := 0
	for i := range demo.columns {
		if math32.Abs(demo.columns[i].position.X-demo.camera.c.Position.X) < demo.renderDistance {
			demo.columns[i].draw()
			columnsDrew++
		}

		if columnsDrew%100 == 0 {
			rl.EndMode3D()
			rl.BeginMode3D(demo.camera.c)
		}
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (demo *Demo) GetTarget() rl.RenderTexture2D {
	return demo.target
}
