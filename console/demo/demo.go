package demo

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

const MAX_COLUMNS = 200

type Demo struct {
	heights   [MAX_COLUMNS]float32
	positions [MAX_COLUMNS]rl.Vector3
	colors    [MAX_COLUMNS]rl.Color

	target rl.RenderTexture2D

	camera      rl.Camera
	cameraAngle [3]float64
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

	d.target = rl.LoadRenderTexture(1920, 1080)
	rl.SetTextureFilter(d.target.Texture, rl.FilterAnisotropic16x)

	d.camera.Position = rl.Vector3{X: 0, Y: 5, Z: 0}
	d.camera.Target = rl.Vector3{X: 0, Y: 0, Z: 0}
	d.camera.Up = rl.Vector3{X: 0, Y: 1, Z: 0}
	d.camera.Fovy = 60
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

	dt := float64(rl.GetFrameTime())
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

	d.updateCamera(dt, left, front, right, up, down)
}

func (d *Demo) Draw() {
	rl.BeginTextureMode(d.target)
	rl.ClearBackground(rl.RayWhite)

	rl.ClearBackground(rl.DarkGray)
	rl.BeginMode3D(d.camera)

	rl.DrawPlane(rl.Vector3{X: 0, Y: 0, Z: 0}, rl.Vector2{X: 100, Y: 100}, rl.LightGray)

	for i := 0; i < MAX_COLUMNS; i++ {
		rl.DrawCube(d.positions[i], 2, d.heights[i], 2, d.colors[i])
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (d *Demo) GetTarget() rl.RenderTexture2D {
	return d.target
}

func (d *Demo) updateCamera(dt float64, left, front, right, up, down bool) {
	const (
		Sensitivity   = 0.4
		FocusDistance = 25
		TiltSpeed     = 3
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

	offsetX := 20 * dt * -math.Sin(d.cameraAngle[0]) * f
	offsetY := 20 * dt * math.Sin(d.cameraAngle[1]) * f
	offsetZ := 20 * dt * -math.Cos(d.cameraAngle[0]) * f

	d.camera.Position.X += float32(offsetX)
	d.camera.Position.Y += float32(offsetY)
	d.camera.Position.Z += float32(offsetZ)

	d.cameraAngle[0] += 10 * dt * h * Sensitivity
	d.cameraAngle[1] += 10 * dt * v * Sensitivity

	if d.cameraAngle[0] > 2*math.Pi {
		d.cameraAngle[0] -= 2 * math.Pi
	} else if d.cameraAngle[0] < 2*math.Pi {
		d.cameraAngle[0] += 2 * math.Pi
	}

	if d.cameraAngle[1] > math.Pi/3 {
		d.cameraAngle[1] = math.Pi / 3
	} else if d.cameraAngle[1] < -math.Pi/3 {
		d.cameraAngle[1] = -math.Pi / 3
	}

	d.camera.Target.X = d.camera.Position.X - float32(math.Sin(d.cameraAngle[0])*FocusDistance)
	d.camera.Target.Y = d.camera.Position.Y + float32(math.Sin(d.cameraAngle[1])*FocusDistance)
	d.camera.Target.Z = d.camera.Position.Z - float32(math.Cos(d.cameraAngle[0])*FocusDistance)

	dir := rl.Vector3{
		X: (d.camera.Target.X - d.camera.Position.X) / FocusDistance,
		Y: (d.camera.Target.Y - d.camera.Position.Y) / FocusDistance,
		Z: (d.camera.Target.Z - d.camera.Position.Z) / FocusDistance,
	}

	tiltOffset := dt * TiltSpeed

	if left {
		d.cameraAngle[2] -= tiltOffset
	} else if right {
		d.cameraAngle[2] += tiltOffset
	} else {
		if d.cameraAngle[2] > tiltOffset {
			d.cameraAngle[2] -= tiltOffset
		} else if d.cameraAngle[2] < -tiltOffset {
			d.cameraAngle[2] += tiltOffset
		}
	}

	if d.cameraAngle[2] > 1.0 {
		d.cameraAngle[2] = 1.0
	} else if d.cameraAngle[2] < -1.0 {
		d.cameraAngle[2] = -1.0
	}

	a := float32(d.cameraAngle[2] * -math.Pi / 8)

	d.camera.Up.X = a*dir.Z - dir.X*dir.Y
	d.camera.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	d.camera.Up.Z = -dir.Y*dir.Z - a*dir.X
}
