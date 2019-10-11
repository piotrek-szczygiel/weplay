package spaceships

import (
	"github.com/chewxy/math32"
	rl "github.com/gen2brain/raylib-go/raylib"
	"github.com/piotrek-szczygiel/raspberry-console/console/controller"
	u "github.com/piotrek-szczygiel/raspberry-console/console/utils"
)

type Spaceships struct {
	ship    ship
	columns []column

	pressed        [3]bool
	renderDistance float32

	target rl.RenderTexture2D
}

func New() *Spaceships {
	var demo Spaceships

	demo.ship = newShip(5, 5, 0, 1)
	demo.columns = generateColumns(1_000, 10_000, 40)

	demo.renderDistance = 500

	demo.target = rl.LoadRenderTexture(int32(rl.GetScreenWidth()), int32(rl.GetScreenHeight()))
	rl.SetTextureFilter(demo.target.Texture, rl.FilterAnisotropic4x)

	return &demo
}

func (s *Spaceships) Update(events []controller.Event) {
	for _, event := range events {
		switch event.Type {
		case controller.Down:
			s.pressed[event.Key] = true
		case controller.Up:
			s.pressed[event.Key] = false
		}
	}

	dt := rl.GetFrameTime()

	s.ship.forward = rl.IsKeyDown(rl.KeyW) || s.pressed[controller.Middle]
	s.ship.back = rl.IsKeyDown(rl.KeyS)
	s.ship.left = rl.IsKeyDown(rl.KeyA) || s.pressed[controller.Left]
	s.ship.right = rl.IsKeyDown(rl.KeyD) || s.pressed[controller.Right]
	s.ship.up = rl.IsKeyDown(rl.KeyUp)
	s.ship.down = rl.IsKeyDown(rl.KeyDown)

	s.ship.update(dt)
}

func (s *Spaceships) Draw() {
	rl.BeginTextureMode(s.target)
	rl.ClearBackground(rl.Black)
	rl.BeginMode3D(s.ship.getCamera())

	rl.DrawPlane(u.Vec3(5000, 0, 0), u.Vec2(10_000, 50), rl.DarkGray)

	columnsDrew := 0
	for i := range s.columns {
		if math32.Abs(s.columns[i].position.X-s.ship.position.X) < s.renderDistance {
			s.columns[i].draw()
			columnsDrew++
		}

		if columnsDrew%100 == 0 {
			rl.EndMode3D()
			rl.BeginMode3D(s.ship.getCamera())
		}
	}

	rl.EndMode3D()
	rl.EndTextureMode()
}

func (s *Spaceships) GetTarget() rl.RenderTexture2D {
	return s.target
}
