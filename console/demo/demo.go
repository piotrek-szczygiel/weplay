package demo

import (
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	"math"
)

const MAX_COLUMNS = 200

type Demo struct {
	heights   [MAX_COLUMNS]float32
	positions [MAX_COLUMNS]rl.Vector3
	colors    [MAX_COLUMNS]rl.Color

	target rl.RenderTexture2D

	camera      rl.Camera
	cameraAngle [2]float64
	pressed     [3]bool
}

func New() *Demo {
	var d Demo
	for i := 0; i < MAX_COLUMNS; i++ {
		d.heights[i] = float32(rl.GetRandomValue(1, 20))
		d.positions[i] = rl.Vector3{
			X: float32(rl.GetRandomValue(-50, 50)),
			Y: d.heights[i] / 2,
			Z: float32(rl.GetRandomValue(-50, 50)),
		}
		d.colors[i] = rl.Color{
			R: uint8(rl.GetRandomValue(20, 255)),
			G: uint8(rl.GetRandomValue(10, 55)),
			B: 30,
			A: 255,
		}
	}

	d.target = rl.LoadRenderTexture(3000, 2000)
	rl.SetTextureFilter(d.target.Texture, rl.FilterAnisotropic16x)

	d.camera.Position = rl.Vector3{0, 5, 0}
	d.camera.Target = rl.Vector3{0, 5, 1}
	d.camera.Up = rl.Vector3{0, 2, 0}
	d.camera.Fovy = 90
	d.camera.Type = rl.CameraPerspective

	rl.SetCameraMode(d.camera, rl.CameraCustom)

	return &d
}

func (d *Demo) Update(events []controller.Event) {
	for _, event := range events {
		switch event.Type {
		case controller.Down:
			d.pressed[event.Key] = true
		case controller.Up:
			d.pressed[event.Key] = false
		}
	}

	var left, front, right, up, down bool

	if d.pressed[controller.Left] || rl.IsKeyDown(rl.KeyLeft) {
		left = true
	}

	if d.pressed[controller.Middle] || rl.IsKeyDown(rl.KeyW) {
		front = true
	}

	if d.pressed[controller.Right] || rl.IsKeyDown(rl.KeyRight) {
		right = true
	}

	if rl.IsKeyDown(rl.KeyUp) {
		up = true
	}

	if rl.IsKeyDown(rl.KeyDown) {
		down = true
	}

	dt := float64(rl.GetFrameTime())
	d.updateCamera(20*dt, left, front, right, up, down)
}

func (d *Demo) Draw() {
	rl.BeginTextureMode(d.target)
	rl.ClearBackground(rl.RayWhite)

	rl.ClearBackground(rl.DarkGray)
	rl.BeginMode3D(d.camera)

	rl.DrawPlane(rl.Vector3{0, 0, 0}, rl.Vector2{100, 100}, rl.LightGray)

	for i := 0; i < MAX_COLUMNS; i++ {
		rl.DrawCube(d.positions[i], 2, d.heights[i], 2, d.colors[i])
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (d *Demo) GetTarget() rl.RenderTexture2D {
	return d.target
}

func (d *Demo) updateCamera(speed float64, left, front, right, up, down bool) {
	const (
		Sensitivity   = 0.2
		FocusDistance = 25
	)

	var f, h, v float64
	if front {
		f = 1
	}

	if left {
		h = 1
	} else if right {
		h = -1
	}

	if up {
		v = 1
	} else if down {
		v = -1
	}

	offsetX := speed * -math.Sin(d.cameraAngle[0]) * f
	offsetY := speed * math.Sin(d.cameraAngle[1]) * f
	offsetZ := speed * -math.Cos(d.cameraAngle[0]) * f

	d.camera.Position.X += float32(offsetX)
	d.camera.Position.Y += float32(offsetY)
	d.camera.Position.Z += float32(offsetZ)

	d.cameraAngle[0] += speed * h * Sensitivity
	d.cameraAngle[1] += speed * v * Sensitivity

	d.camera.Target.X = d.camera.Position.X - float32(math.Sin(d.cameraAngle[0])*FocusDistance)
	d.camera.Target.Y = d.camera.Position.Y + float32(math.Sin(d.cameraAngle[1])*FocusDistance)
	d.camera.Target.Z = d.camera.Position.Z - float32(math.Cos(d.cameraAngle[0])*FocusDistance)
}
