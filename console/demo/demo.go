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

	var left, front, right, rotateLeft, rotateRight bool

	if d.pressed[controller.Left] {
		rotateLeft = true
	}

	if d.pressed[controller.Middle] {
		front = true
	}

	if d.pressed[controller.Right] {
		rotateRight = true
	}

	dt := float64(rl.GetFrameTime())
	d.updateCamera(20*dt, left, front, right, rotateLeft, rotateRight)
}

func (d *Demo) Draw() {
	rl.ClearBackground(rl.DarkGray)
	rl.BeginMode3D(d.camera)

	rl.DrawPlane(rl.Vector3{0, 0, 0}, rl.Vector2{100, 100}, rl.LightGray)

	for i := 0; i < MAX_COLUMNS; i++ {
		rl.DrawCube(d.positions[i], 2, d.heights[i], 2, d.colors[i])
	}

	rl.EndMode3D()
}

func (d *Demo) updateCamera(speed float64, left, front, right, rotateLeft, rotateRight bool) {
	var l, f, r, rotate float64

	if left {
		l = 1
	}

	if front {
		f = 1
	}

	if right {
		r = 1
	}

	if rotateLeft {
		rotate = 1
	}

	if rotateRight {
		rotate = -1
	}

	offsetX := speed * (-math.Sin(d.cameraAngle[0])*f - math.Cos(d.cameraAngle[0])*l + math.Cos(d.cameraAngle[0])*r)
	offsetZ := speed * (-math.Cos(d.cameraAngle[0])*f + math.Sin(d.cameraAngle[0])*l - math.Sin(d.cameraAngle[0])*r)

	d.camera.Position.X += float32(offsetX)
	d.camera.Position.Z += float32(offsetZ)

	d.cameraAngle[0] += speed * rotate * 0.1

	d.camera.Target.X = d.camera.Position.X - float32(math.Sin(d.cameraAngle[0])*25)
	d.camera.Target.Z = d.camera.Position.Z - float32(math.Cos(d.cameraAngle[0])*25)
}
