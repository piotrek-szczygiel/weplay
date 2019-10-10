package demo

import (
	math "github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
)

const maxColumns = 1000

type Demo struct {
	heights   [maxColumns]float32
	positions [maxColumns]rl.Vector3
	colors    [maxColumns]rl.Color

	target rl.RenderTexture2D

	camera      rl.Camera
	cameraAngle rl.Vector3
	pressed     [3]bool

	renderDistance float32
}

func New() *Demo {
	var demo Demo

	for i := 0; i < maxColumns; i++ {
		demo.heights[i] = float32(rl.GetRandomValue(6, 30))
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

	demo.target = rl.LoadRenderTexture(int32(rl.GetScreenWidth()), int32(rl.GetScreenHeight()))
	rl.SetTextureFilter(demo.target.Texture, rl.FilterAnisotropic4x)

	demo.camera.Position = rl.Vector3{X: 25, Y: 5}
	demo.camera.Target = rl.Vector3{}
	demo.camera.Up = rl.Vector3{Y: 1}
	demo.camera.Fovy = 60
	demo.camera.Type = rl.CameraPerspective

	demo.cameraAngle.X = -math.Pi / 2

	demo.renderDistance = 500
	rl.SetCameraMode(demo.camera, rl.CameraCustom)

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
		demo.cameraAngle.X += rotationSpeed
	}

	if rl.IsKeyDown(rl.KeyRight) {
		demo.cameraAngle.X -= rotationSpeed
	}

	if rl.IsKeyDown(rl.KeyUp) {
		demo.cameraAngle.Y += rotationSpeed
	}

	if rl.IsKeyDown(rl.KeyDown) {
		demo.cameraAngle.Y -= rotationSpeed
	}

	if rl.IsKeyDown(rl.KeyComma) {
		demo.cameraAngle.Z += rotationSpeed
	}

	if rl.IsKeyDown(rl.KeyPeriod) {
		demo.cameraAngle.Z -= rotationSpeed
	}

	demo.updateCamera(back, front, left, right, up, down)
}

func (demo *Demo) Draw() {
	rl.BeginTextureMode(demo.target)
	rl.ClearBackground(rl.Black)
	rl.BeginMode3D(demo.camera)

	rl.DrawPlane(rl.Vector3{X: 5000}, rl.Vector2{X: 10000, Y: 50}, rl.DarkGray)

	columnsDrew := 0
	for i := 0; i < maxColumns; i++ {
		if math.Abs(demo.positions[i].X-demo.camera.Position.X) < demo.renderDistance {
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

func (demo *Demo) updateCamera(back, front, left, right, up, down float32) {
	const (
		focusDistance = 25
		maxCameraY    = 1.48
		minCameraY    = -1.48
		fullRotation  = 2 * math.Pi
	)

	if demo.cameraAngle.Y > maxCameraY {
		demo.cameraAngle.Y = maxCameraY
	} else if demo.cameraAngle.Y < minCameraY {
		demo.cameraAngle.Y = minCameraY
	}

	if demo.cameraAngle.X > fullRotation {
		demo.cameraAngle.X -= fullRotation
	} else if demo.cameraAngle.X < -fullRotation {
		demo.cameraAngle.X += fullRotation
	}

	demo.camera.Position.X +=
		math.Sin(demo.cameraAngle.X)*back -
			math.Sin(demo.cameraAngle.X)*front -
			math.Cos(demo.cameraAngle.X)*left +
			math.Cos(demo.cameraAngle.X)*right

	demo.camera.Position.Y +=
		math.Sin(demo.cameraAngle.Y)*front -
			math.Sin(demo.cameraAngle.Y)*back + up - down

	demo.camera.Position.Z +=
		math.Cos(demo.cameraAngle.X)*back -
			math.Cos(demo.cameraAngle.X)*front +
			math.Sin(demo.cameraAngle.X)*left -
			math.Sin(demo.cameraAngle.X)*right

	demo.camera.Target.X = demo.camera.Position.X - math.Sin(demo.cameraAngle.X)*focusDistance
	demo.camera.Target.Y = demo.camera.Position.Y + math.Sin(demo.cameraAngle.Y)*focusDistance
	demo.camera.Target.Z = demo.camera.Position.Z - math.Cos(demo.cameraAngle.X)*focusDistance

	dir := rl.Vector3{
		X: (demo.camera.Target.X - demo.camera.Position.X) / focusDistance,
		Y: (demo.camera.Target.Y - demo.camera.Position.Y) / focusDistance,
		Z: (demo.camera.Target.Z - demo.camera.Position.Z) / focusDistance,
	}

	alpha := demo.cameraAngle.Z * -math.Pi / 8

	demo.camera.Up.X = alpha*dir.Z - dir.X*dir.Y
	demo.camera.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	demo.camera.Up.Z = -dir.Y*dir.Z - alpha*dir.X
}
