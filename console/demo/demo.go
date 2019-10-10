package demo

import (
	"math"

	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

const maxColumns = 2000

type Demo struct {
	heights   [maxColumns]float32
	positions [maxColumns]rl.Vector3
	colors    [maxColumns]rl.Color

	target rl.RenderTexture2D

	camera      rl.Camera
	cameraAngle float64
	pressed     [3]bool

	renderDistance float32

	forwardSpeed float64
	turningSpeed float64
}

func New() *Demo {
	var demo Demo
	for i := 0; i < maxColumns; i++ {
		demo.heights[i] = float32(rl.GetRandomValue(3, 20))
		demo.positions[i] = rl.Vector3{
			X: float32(rl.GetRandomValue(50, 10000)),
			Y: demo.heights[i] / 2,
			Z: float32(rl.GetRandomValue(-20, 20)),
		}
		demo.colors[i] = rl.Color{
			R: uint8(rl.GetRandomValue(20, 255)),
			G: uint8(rl.GetRandomValue(10, 55)),
			B: 30,
			A: 255,
		}
	}

	demo.target = rl.LoadRenderTexture(1920, 1080)
	rl.SetTextureFilter(demo.target.Texture, rl.FilterAnisotropic16x)

	demo.camera.Position = rl.Vector3{25, 5, 0}
	demo.camera.Target = rl.Vector3{0, 0, 0}
	demo.camera.Up = rl.Vector3{0, 1, 0}
	demo.camera.Fovy = 60
	demo.camera.Type = rl.CameraPerspective

	demo.cameraAngle = -math.Pi / 2

	demo.renderDistance = 500

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

	dt := float64(rl.GetFrameTime())

	var left, front, right bool
	if demo.pressed[controller.Left] || rl.IsKeyDown(rl.KeyLeft) {
		left = true
	}

	if demo.pressed[controller.Middle] || rl.IsKeyDown(rl.KeyW) {
		front = true
	}

	if demo.pressed[controller.Right] || rl.IsKeyDown(rl.KeyRight) {
		right = true
	}

	const (
		forwardAcc = 75
		turningAcc = 3
		maxTurn    = 1
	)

	if front {
		demo.forwardSpeed += dt * forwardAcc
		if demo.forwardSpeed > forwardAcc {
			demo.forwardSpeed = forwardAcc
		}
	} else {
		demo.forwardSpeed -= dt * 10
		if demo.forwardSpeed < 0 {
			demo.forwardSpeed = 0
		}
	}

	if left && !right {
		demo.turningSpeed += dt * turningAcc
		if demo.turningSpeed > maxTurn {
			demo.turningSpeed = maxTurn
		}
	} else if right && !left {
		demo.turningSpeed -= dt * turningAcc
		if demo.turningSpeed < -maxTurn {
			demo.turningSpeed = -maxTurn
		}
	} else if demo.turningSpeed < -dt*turningAcc*2 {
		demo.turningSpeed += dt * turningAcc * 2
	} else if demo.turningSpeed > dt*turningAcc*2 {
		demo.turningSpeed -= dt * turningAcc * 2
	}

	if math.Abs(demo.turningSpeed) < dt*turningAcc {
		demo.turningSpeed = 0
	}

	demo.updateCamera(dt)
}

func (demo *Demo) Draw() {
	rl.SetCameraMode(demo.camera, rl.CameraCustom)
	rl.BeginTextureMode(demo.target)
	rl.ClearBackground(rl.RayWhite)

	rl.ClearBackground(rl.DarkGray)
	rl.BeginMode3D(demo.camera)

	rl.DrawPlane(rl.Vector3{5000, 0, 0}, rl.Vector2{10000, 40}, rl.LightGray)

	columnsDrew := 0
	for i := 0; i < maxColumns; i++ {
		if demo.positions[i].X > demo.camera.Position.X &&
			demo.positions[i].X-demo.camera.Position.X < demo.renderDistance {

			rl.DrawCube(demo.positions[i], 2, demo.heights[i], 2, demo.colors[i])
			columnsDrew++
		}

		if columnsDrew%100 == 0 {
			rl.EndMode3D()
			rl.BeginMode3D(demo.camera)
		}
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (demo *Demo) GetTarget() rl.RenderTexture2D {
	return demo.target
}

func (demo *Demo) updateCamera(dt float64) {
	const (
		sensitivity   = 1.5
		focusDistance = 25
	)

	offsetX := demo.forwardSpeed * dt * -math.Sin(demo.cameraAngle)
	offsetZ := demo.forwardSpeed * dt * -math.Cos(demo.cameraAngle)

	demo.camera.Position.X += float32(offsetX)
	demo.camera.Position.Z += float32(offsetZ)

	demo.cameraAngle += sensitivity * dt * demo.turningSpeed

	if demo.cameraAngle > 2*math.Pi {
		demo.cameraAngle -= 2 * math.Pi
	} else if demo.cameraAngle < 2*math.Pi {
		demo.cameraAngle += 2 * math.Pi
	}

	demo.camera.Target.X = demo.camera.Position.X - float32(math.Sin(demo.cameraAngle)*focusDistance)
	demo.camera.Target.Z = demo.camera.Position.Z - float32(math.Cos(demo.cameraAngle)*focusDistance)

	dir := rl.Vector3{
		X: (demo.camera.Target.X - demo.camera.Position.X) / focusDistance,
		Y: (demo.camera.Target.Y - demo.camera.Position.Y) / focusDistance,
		Z: (demo.camera.Target.Z - demo.camera.Position.Z) / focusDistance,
	}

	a := float32(demo.turningSpeed * math.Pi / 16)

	demo.camera.Up.X = a*dir.Z - dir.X*dir.Y
	demo.camera.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	demo.camera.Up.Z = -dir.Y*dir.Z - a*dir.X
}
