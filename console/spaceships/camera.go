package spaceships

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
)

type camera struct {
	c     rl.Camera
	angle rl.Vector3
}

func newCamera() (camera camera) {
	camera.c.Position = vec3(25, 5, 0)
	camera.c.Target = vec3(0, 0, 0)
	camera.c.Up = vec3(0, 1, 0)
	camera.c.Fovy = 60
	camera.c.Type = rl.CameraPerspective

	camera.angle.X = -math32.Pi / 2
	return
}

func (camera *camera) update() {
	const (
		focusDistance = 25
		maxCameraY    = 1.48
		minCameraY    = -1.48
		fullRotation  = 2 * math32.Pi
	)

	if camera.angle.Y > maxCameraY {
		camera.angle.Y = maxCameraY
	} else if camera.angle.Y < minCameraY {
		camera.angle.Y = minCameraY
	}

	if camera.angle.X > fullRotation {
		camera.angle.X -= fullRotation
	} else if camera.angle.X < -fullRotation {
		camera.angle.X += fullRotation
	}

	//camera.c.Position.X +=
	//	math32.Sin(camera.angle.X)*back -
	//		math32.Sin(camera.angle.X)*front -
	//		math32.Cos(camera.angle.X)*left +
	//		math32.Cos(camera.angle.X)*right
	//
	//camera.c.Position.Y +=
	//	math32.Sin(camera.angle.Y)*front -
	//		math32.Sin(camera.angle.Y)*back + up - down
	//
	//camera.c.Position.Z +=
	//	math32.Cos(camera.angle.X)*back -
	//		math32.Cos(camera.angle.X)*front +
	//		math32.Sin(camera.angle.X)*left -
	//		math32.Sin(camera.angle.X)*right

	camera.c.Target.X = camera.c.Position.X - math32.Sin(camera.angle.X)*focusDistance
	camera.c.Target.Y = camera.c.Position.Y + math32.Sin(camera.angle.Y)*focusDistance
	camera.c.Target.Z = camera.c.Position.Z - math32.Cos(camera.angle.X)*focusDistance

	dir := vec3(
		(camera.c.Target.X-camera.c.Position.X)/focusDistance,
		(camera.c.Target.Y-camera.c.Position.Y)/focusDistance,
		(camera.c.Target.Z-camera.c.Position.Z)/focusDistance,
	)

	alpha := camera.angle.Z * -math32.Pi / 8

	camera.c.Up.X = alpha*dir.Z - dir.X*dir.Y
	camera.c.Up.Y = dir.X*dir.X + dir.Z*dir.Z
	camera.c.Up.Z = -dir.Y*dir.Z - alpha*dir.X
}
