package demo

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

const maxColumns = 100

type Demo struct {
	heights   [maxColumns]float32
	positions [maxColumns]rl.Vector3
	colors    [maxColumns]rl.Color

	target rl.RenderTexture2D

	camera      rl.Camera
	cameraAngle [3]float64
	pressed     [3]bool
}

func New() *Demo {
	var d Demo
	for i := 0; i < maxColumns; i++ {
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

	d.target = rl.LoadRenderTexture(1920, 1080)
	rl.SetTextureFilter(d.target.Texture, rl.FilterAnisotropic16x)

	d.camera.Position = rl.Vector3{0, 5, 0}
	d.camera.Target = rl.Vector3{0, 0, 0}
	d.camera.Up = rl.Vector3{0, 1, 0}
	d.camera.Fovy = 60
	d.camera.Type = rl.CameraPerspective

	rl.SetCameraMode(d.camera, rl.CameraCustom)

	return &d
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

	dt := float64(rl.GetFrameTime())
	var left, front, right, up, down bool

	if demo.pressed[controller.Left] || rl.IsKeyDown(rl.KeyLeft) {
		left = true
	}

	if demo.pressed[controller.Middle] || rl.IsKeyDown(rl.KeyW) {
		front = true
	}

	if demo.pressed[controller.Right] || rl.IsKeyDown(rl.KeyRight) {
		right = true
	}

	if rl.IsKeyDown(rl.KeyUp) {
		up = true
	}

	if rl.IsKeyDown(rl.KeyDown) {
		down = true
	}

	demo.updateCamera(dt, left, front, right, up, down)
}

func (demo *Demo) Draw() {
	rl.BeginTextureMode(demo.target)
	rl.ClearBackground(rl.RayWhite)

	rl.ClearBackground(rl.DarkGray)
	rl.BeginMode3D(demo.camera)

	rl.DrawPlane(rl.Vector3{}, rl.Vector2{X: 100, Y: 100}, rl.LightGray)

	for i := 0; i < maxColumns; i++ {
		rl.DrawCube(demo.positions[i], 2, demo.heights[i], 2, demo.colors[i])
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (demo *Demo) GetTarget() rl.RenderTexture2D {
	return demo.target
}

func (demo *Demo) updateCamera(dt float64, left, front, right, up, down bool) {
	const (
		sensitivity   = 0.4
		focusDistance = 25
		tiltSpeed     = 3
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

	offsetX := 20 * dt * -math.Sin(demo.cameraAngle[0]) * f
	offsetY := 20 * dt * math.Sin(demo.cameraAngle[1]) * f
	offsetZ := 20 * dt * -math.Cos(demo.cameraAngle[0]) * f

	demo.camera.Position.X += float32(offsetX)
	demo.camera.Position.Y += float32(offsetY)
	demo.camera.Position.Z += float32(offsetZ)

	demo.cameraAngle[0] += 10 * dt * h * sensitivity
	demo.cameraAngle[1] += 10 * dt * v * sensitivity

	if demo.cameraAngle[0] > 2*math.Pi {
		demo.cameraAngle[0] -= 2 * math.Pi
	} else if demo.cameraAngle[0] < 2*math.Pi {
		demo.cameraAngle[0] += 2 * math.Pi
	}

	if demo.cameraAngle[1] > math.Pi/3 {
		demo.cameraAngle[1] = math.Pi / 3
	} else if demo.cameraAngle[1] < -math.Pi/3 {
		demo.cameraAngle[1] = -math.Pi / 3
	}

	demo.camera.Target.X = demo.camera.Position.X - float32(math.Sin(demo.cameraAngle[0])*focusDistance)
	demo.camera.Target.Y = demo.camera.Position.Y + float32(math.Sin(demo.cameraAngle[1])*focusDistance)
	demo.camera.Target.Z = demo.camera.Position.Z - float32(math.Cos(demo.cameraAngle[0])*focusDistance)

	dir := rl.Vector3{
		X: (demo.camera.Target.X - demo.camera.Position.X) / focusDistance,
		Y: (demo.camera.Target.Y - demo.camera.Position.Y) / focusDistance,
		Z: (demo.camera.Target.Z - demo.camera.Position.Z) / focusDistance,
	}

	tiltOffset := dt * tiltSpeed

	if left {
		demo.cameraAngle[2] -= tiltOffset
	} else if right {
		demo.cameraAngle[2] += tiltOffset
	} else {
		if demo.cameraAngle[2] > tiltOffset {
			demo.cameraAngle[2] -= tiltOffset
		} else if demo.cameraAngle[2] < -tiltOffset {
			demo.cameraAngle[2] += tiltOffset
		}
	}

	if demo.cameraAngle[2] > 1.0 {
		demo.cameraAngle[2] = 1.0
	} else if demo.cameraAngle[2] < -1.0 {
		demo.cameraAngle[2] = -1.0
	}

	a := float32(demo.cameraAngle[2] * -math.Pi / 8)

	demo.camera.Up.X = a*dir.Z - dir.X*dir.Y
	demo.camera.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	demo.camera.Up.Z = -dir.Y*dir.Z - a*dir.X
}
