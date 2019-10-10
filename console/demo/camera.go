package demo

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
)

type camera struct {
	c           rl.Camera
	cameraAngle rl.Vector3
}

func newCamera() (camera camera) {
	camera.c.Position = rl.Vector3{X: 25, Y: 5}
	camera.c.Target = rl.Vector3{}
	camera.c.Up = rl.Vector3{Y: 1}
	camera.c.Fovy = 60
	camera.c.Type = rl.CameraPerspective

	camera.cameraAngle.X = -math32.Pi / 2
	return
}

func (camera *camera) update(back, front, left, right, up, down float32) {
	const (
		focusDistance = 25
		maxCameraY    = 1.48
		minCameraY    = -1.48
		fullRotation  = 2 * math32.Pi
	)

	if camera.cameraAngle.Y > maxCameraY {
		camera.cameraAngle.Y = maxCameraY
	} else if camera.cameraAngle.Y < minCameraY {
		camera.cameraAngle.Y = minCameraY
	}

	if camera.cameraAngle.X > fullRotation {
		camera.cameraAngle.X -= fullRotation
	} else if camera.cameraAngle.X < -fullRotation {
		camera.cameraAngle.X += fullRotation
	}

	camera.c.Position.X +=
		math32.Sin(camera.cameraAngle.X)*back -
			math32.Sin(camera.cameraAngle.X)*front -
			math32.Cos(camera.cameraAngle.X)*left +
			math32.Cos(camera.cameraAngle.X)*right

	camera.c.Position.Y +=
		math32.Sin(camera.cameraAngle.Y)*front -
			math32.Sin(camera.cameraAngle.Y)*back + up - down

	camera.c.Position.Z +=
		math32.Cos(camera.cameraAngle.X)*back -
			math32.Cos(camera.cameraAngle.X)*front +
			math32.Sin(camera.cameraAngle.X)*left -
			math32.Sin(camera.cameraAngle.X)*right

	camera.c.Target.X = camera.c.Position.X - math32.Sin(camera.cameraAngle.X)*focusDistance
	camera.c.Target.Y = camera.c.Position.Y + math32.Sin(camera.cameraAngle.Y)*focusDistance
	camera.c.Target.Z = camera.c.Position.Z - math32.Cos(camera.cameraAngle.X)*focusDistance

	dir := rl.Vector3{
		X: (camera.c.Target.X - camera.c.Position.X) / focusDistance,
		Y: (camera.c.Target.Y - camera.c.Position.Y) / focusDistance,
		Z: (camera.c.Target.Z - camera.c.Position.Z) / focusDistance,
	}

	alpha := camera.cameraAngle.Z * -math32.Pi / 8

	camera.c.Up.X = alpha*dir.Z - dir.X*dir.Y
	camera.c.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	camera.c.Up.Z = -dir.Y*dir.Z - alpha*dir.X
}

func (camera *camera) rotateHorizontal(offset float32) {
	camera.cameraAngle.X += offset
}

func (camera *camera) rotateVertical(offset float32) {
	camera.cameraAngle.Y += offset
}

func (camera *camera) rotateDiagonal(offset float32) {
	camera.cameraAngle.Z += offset
}
